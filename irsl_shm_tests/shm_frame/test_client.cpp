#include "irsl/shm_controller.h"
#include "irsl/realtime_task.h"

#include <iostream>

using namespace irsl_shm_controller;
using namespace irsl_realtime_task;

int main(int argc, char **argv)
{
    std::cout << "ShmDataHeader: " << sizeof(ShmDataHeader) << std::endl;

    ShmSettings ss;
    //ss.numJoints = 3;
    //ss.numForceSensors = 1;
    //ss.numImuSensors   = 0;
    ss.hash = 8888;
    ss.shm_key = 8889;
    //ss.extraDataSize = 0;
    //ss.extraDataSize = 96;
    //ss.jointType =

    ////
    ShmManager sm(ss);

    bool res;
    res = sm.openSharedMemory(false);
    std::cout << "open: " << res << std::endl;

    res = sm.checkHeader();
    std::cout << "checkHeader: " << res << std::endl;
    if (!res) {
        return -1;
    }
    std::cout << "numJoints: " << sm.settings().numJoints << std::endl;
    std::cout << "numFsensor: " << sm.settings().numForceSensors << std::endl;
    std::cout << "numIsensor: " << sm.settings().numImuSensors << std::endl;
    std::cout << "exdata: " << sm.settings().extraDataSize << std::endl;
    std::cout << "total: " << sm.settings().totalSize << std::endl;
    std::cout << "jointType: " << sm.settings().jointType << std::endl;
    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    RealtimeContext rt(1000000);
    int cntr = 0;
    rt.start();
    while(true) {
        cntr++;
        if (cntr > 1000) {
            std::cout << "max: " << rt.getMaxInterval() << std::endl;
            std::cout << "norm: " << rt.getNorm() << std::endl;
            uint64_t ff = sm.getFrame();
            std::cout << "ff: " << ff << std::endl;
            rt.reset();
            cntr = 0;
        }
        rt.waitNextFrame();
    }
    // polling
    return 0;
}
