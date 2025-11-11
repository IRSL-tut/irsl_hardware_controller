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
    ss.numImuSensors   = 0;
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
    res = sm.openSharedMemory(false);
    std::cout << "open: " << res << std::endl;

    res = sm.checkHeader();
    std::cout << "checkHeader: " << res << std::endl;
    if (!res) {
        return -1;
    }
    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    RealtimeContext rt(0, 1000000, false);
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
    }
    // polling
    return 0;
}
