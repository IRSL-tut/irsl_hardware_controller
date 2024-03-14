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
    ss.jointType = ShmSettings::PositionCommand | ShmSettings::PositionGains;

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

    res = sm.isOpen();
    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    if (res) {
        int size = rd_ss.numJoints;
        std::cout << "joints: " << rd_ss.numJoints << std::endl;

        std::cout << "frame: " << sm.getFrame() << std::endl;
        int32_t sec, nsec;
        sm.getTime(sec, nsec);
        std::cout << "time: " << sec << " / " << nsec << std::endl;

        std::vector<irsl_float_type> buf;
        buf.resize(size);
        if (sm.readPositionCurrent(buf)) {
            std::cout << "cur: ";
            for(int i = 0; i < size; i++) {
                std::cout << buf[i];
                if (i != size-1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
        buf.resize(size);
        if (sm.readPositionCommand(buf)) {
            std::cout << "com: ";
            for(int i = 0; i < size; i++) {
                std::cout << buf[i];
                if (i != size-1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
