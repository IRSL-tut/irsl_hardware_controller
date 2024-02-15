#ifndef __irsl_realtime_task_h__
#define __irsl_realtime_task_h__

#include <stdexcept> // c++11
#include <math.h>

#include <stdio.h>
#include <errno.h>

#include <time.h>
// clock_gettime, timespec in <time.h>
// clock_nanosleep require -lrt

#include <sched.h>
// realtime scheduling

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define NSEC_PER_SEC    1000000000L
#define REALTIME_PRIO_MAX 99
#define REALTIME_PRIO_MIN 1
// milsec(-3)/usec(-6)/nano(-9)/pico(-12)

namespace irsl_realtime_task
{


static inline long time_diff(timespec &a, timespec &b) // a - b
{
    long sec  = (a.tv_sec  - b.tv_sec) * NSEC_PER_SEC;
    long nsec =  a.tv_nsec - b.tv_nsec;

    return (sec + nsec);
}

class IntervalStatistics {
    const double m_interval_u; // u_sec
    timespec m_t; // current_time
    double n;
    double norm2;
    double max_interval_u; // u_sec
    double min_interval_u; // u_sec

public:
    IntervalStatistics( const unsigned long interval_us ) :
        m_interval_u( interval_us ), n( 0.0 ), norm2( 0.0 ),
        max_interval_u( - NSEC_PER_SEC ),
        min_interval_u( NSEC_PER_SEC )
    {
        clock_gettime( CLOCK_MONOTONIC, &m_t );
    }
    void sync() // update clock & statistics
    {
        timespec now_t;
        clock_gettime( CLOCK_MONOTONIC, &now_t );

        const double measured_interval_u = ( (now_t.tv_sec - m_t.tv_sec)*NSEC_PER_SEC + (now_t.tv_nsec - m_t.tv_nsec) )/1000.0;
        if (measured_interval_u > max_interval_u) max_interval_u = measured_interval_u;
        if (measured_interval_u < min_interval_u) min_interval_u = measured_interval_u;
        // 前フレームの時刻として保存
        m_t.tv_sec  = now_t.tv_sec;
        m_t.tv_nsec = now_t.tv_nsec;

        const double next_n     = n + 1.0;
        const double rcp_next_n = 1.0 / next_n;
        const double dif        = measured_interval_u - m_interval_u;
        const double next_norm2 = (norm2 * n + dif * dif) * rcp_next_n;

        n = next_n;
        norm2 = next_norm2;
    }
    void start(bool _reset = true) // first time
    {
        clock_gettime( CLOCK_MONOTONIC, &m_t );
        if (_reset) {
            reset();
        }
    }
    double getTimeUsec()
    {
        timespec now_t;
        clock_gettime( CLOCK_MONOTONIC, &now_t );
        double measured_interval_usec = ( (now_t.tv_sec - m_t.tv_sec)*NSEC_PER_SEC + (now_t.tv_nsec - m_t.tv_nsec) )/1000.0;

        return measured_interval_usec;
    }
    long getTimeNsec()
    {
        timespec now_t;
        clock_gettime( CLOCK_MONOTONIC, &now_t );
        long measured_interval_nsec = (now_t.tv_nsec - m_t.tv_nsec);
        measured_interval_nsec += (now_t.tv_sec - m_t.tv_sec)*NSEC_PER_SEC;
        return measured_interval_nsec;
    }
    void reset() // reset statistics
    {
        n     = 0.0;
        norm2 = 0.0;
        max_interval_u = - NSEC_PER_SEC;
        min_interval_u =   NSEC_PER_SEC;
    }
    double getNorm()
    {
        return sqrt( norm2 );
    }
    double getMaxInterval ()
    {
        return max_interval_u;
    }
    double getMinInterval ()
    {
        return min_interval_u;
    }
    void sleepUntil (long time_in_nsec)
    {
        timespec target_t; // target time
        target_t.tv_sec  = m_t.tv_sec;
        target_t.tv_nsec = m_t.tv_nsec;

        target_t.tv_nsec += time_in_nsec;
        while ( target_t.tv_nsec  >= NSEC_PER_SEC ) {
            target_t.tv_nsec -= NSEC_PER_SEC;
            target_t.tv_sec  += 1;
        }
        clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME, &target_t, NULL );
    }
};

class RealtimeContext {
    const int m_interval_n; // n_sec
    timespec m_t;
    void _increment_t(){
        m_t.tv_nsec += m_interval_n;
        while( m_t.tv_nsec >= NSEC_PER_SEC ){
            m_t.tv_nsec -= NSEC_PER_SEC;
            m_t.tv_sec++;
        }
    }
    int latency_fd;
    IntervalStatistics _int_stat;

public:
    RealtimeContext( const int prio, const unsigned long interval_ns = 1000000 , const bool write_latency_fd = true)
        : m_interval_n( interval_ns ), latency_fd(-1), _int_stat( interval_ns/1000 )
    {
        // see cyclictest in rt-tests
        if (write_latency_fd && latency_fd < 0) {
            struct stat st;
            if( stat("/dev/cpu_dma_latency", &st) == 0 ) {
                latency_fd = open("/dev/cpu_dma_latency", O_RDWR);
                if (latency_fd != -1) {
                    int val = 0;
                    int ret;
                    ret = write(latency_fd, &val, 4);
                    if (ret == 0) {
                        fprintf(stderr, "setting /dev/cpu_dma_latency was failed (%d : %s)\n",
                                val, strerror(errno));
                        close(latency_fd);
                    } else {
                        fprintf(stderr, "/dev/cpu_dma_latency set to %d [us]\n", val);
                    }
                } else {
                    fprintf(stderr, "faild to open /dev/cpu_dma_latency (%s)\n",
                            strerror(errno));
                }
            } else {
                fprintf(stderr, "There is no /dev/cpu_dma_latency (%s)\n",
                        strerror(errno));
            }
        }
        //
        sched_param param;
        param.sched_priority = prio;
        if( sched_setscheduler( 0, SCHED_FIFO, &param ) != -1 ) {
            // start real time
            fprintf(stderr, "start as realtime process\n");
        } else {
            //fprintf(stderr, "error(%d)\n", errno);
            //throw std::runtime_error( "sched_setscheduler" );
            fprintf(stderr, "start as non-realtime process\n");
        }
        // set current_time
        clock_gettime( CLOCK_MONOTONIC, &m_t );
    }
    ~RealtimeContext() {}


    void start()
    {
        clock_gettime( CLOCK_MONOTONIC, &m_t );
        _int_stat.start();
    }
    long timeToNext()
    {
        timespec now_t;
        clock_gettime( CLOCK_MONOTONIC, &now_t );
        long diff_n = time_diff(now_t, m_t);
        return (diff_n - m_interval_n);
    }
    void waitNextFrame()
    {
        _increment_t();
        clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME, &m_t, NULL );
        _int_stat.sync();
    }
    void statisticsSync ()
    {
        _int_stat.sync();
    }
    void statisticsStart ()
    {
        _int_stat.start();
    }
    void statisticsReset ()
    {
        _int_stat.reset();
    }
    double statisticsGetNorm ()
    {
        return _int_stat.getNorm();
    }
    double statisticsGetMaxInterval ()
    {
        return _int_stat.getMaxInterval();
    }
    double statisticsGetTimeUsec()
    {
        return _int_stat.getTimeUsec();
    }
    long statisticsGetTimeNsec()
    {
        return _int_stat.getTimeNsec();
    }
};

} // namespace
#endif
