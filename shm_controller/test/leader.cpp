#include "irsl/shm_controller.h"

#include <iostream>

using namespace irsl_shm_controller;

int main(int argc, char **argv)
{
    std::cout << "ShmDataHeader: " << sizeof(ShmDataHeader) << std::endl;

    ShmSettings ss;
    ss.numJoints = 18;
    ss.numForceSensors = 0;
    ss.numImuSensors   = 0;
    ss.hash    = 8888;
    ss.shm_key = 8888;
    //ss.extraDataSize = 0;
    //ss.extraDataSize = 96;
    ss.jointType = ShmSettings::PositionCommand | ShmSettings::PositionGains;

    std::cout << "TotalSize: "  << ss.calcTotalSize() << std::endl;
    std::cout << "SingleSize: " << ss.getSizeOfSingleJointData() << std::endl;

    std::vector<uint8_t> data;
    ss.setHeaderData(data);

    ////
    ShmManager sm(ss);

    bool res;
    res = sm.openSharedMemory();
    std::cout << "open: " << res << std::endl;

    res = sm.writeHeader();
    std::cout << "writeHeader: " << res << std::endl;

    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    // polling
    return 0;
}
