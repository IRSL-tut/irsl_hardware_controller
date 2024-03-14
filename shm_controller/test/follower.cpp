#include "irsl/shm_controller.h"

#include <iostream>

using namespace irsl_shm_controller;

int main(int argc, char **argv)
{
    std::cout << "ShmDataHeader: " << sizeof(ShmDataHeader) << std::endl;

    ShmSettings ss;
    ss.numJoints = 3;
    ss.numForceSensors = 1;
    ss.numImuSensors   = 1;
    ss.hash    = 8888;
    ss.shm_key = 8888;
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

    ShmSettings rd_ss;
    res = sm.readFromHeader(rd_ss);
    std::cout << "sm.readFromHeader() : " << res << std::endl;

    std::cout << "read data / equal : " << ss.equal(rd_ss) << std::endl;

    std::cout << "checkHeader() : " << sm.checkHeader() << std::endl;

    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    return 0;
}
