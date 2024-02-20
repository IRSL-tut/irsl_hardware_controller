#include "irsl/shm_controller.h"
#include "irsl/realtime_task.h"

#include <iostream>

using namespace irsl_shm_controller;
using namespace irsl_realtime_task;

int main(int argc, char **argv)
{
    std::cout << "ShmDataHeader: " << sizeof(ShmDataHeader) << std::endl;

    ShmSettings ss;
    ss.numJoints = 3;
    ss.numForceSensors = 1;
    ss.numImuSensors   = 1;
    ss.hash = 8888;
    ss.shm_key = 8889;
    //ss.extraDataSize = 0;
    //ss.extraDataSize = 96;
    // ss.jointType =
    std::cout << "TotalSize: "  << ss.calcTotalSize() << std::endl;
    std::cout << "SingleSize: " << ss.getSizeOfSingleJointData() << std::endl;

    std::vector<uint8_t> data;
    ss.setHeaderData(data);

    ////
    ShmManager sm(ss);

    bool res;
    res = sm.openSharedMemory(true);
    std::cout << "open: " << res << std::endl;

    res = sm.writeHeader();
    std::cout << "writeHeader: " << res << std::endl;

    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    sm.resetFrame();
    IntervalStatistics tm(10000);

    int cntr = 0;
    tm.start();
    while(true) {
        tm.sleepUntil(10000000);
        tm.sync();
        cntr++;
        sm.incrementFrame();
        if (cntr > 100) {
            std::cout << "max: " << tm.getMaxInterval() << std::endl;
            tm.reset();
            cntr = 0;
        }
    }
    // polling
    return 0;
}
