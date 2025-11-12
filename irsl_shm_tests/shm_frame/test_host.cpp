#include "irsl/shm_controller.h"
#include "irsl/realtime_task.h"

#include <iostream>

namespace isc = irsl_shm_controller;
namespace irt = irsl_realtime_task;

int main(int argc, char **argv)
{
    std::cout << "ShmDataHeader: " << sizeof(isc::ShmDataHeader) << std::endl;

    isc::ShmSettings ss;
    ss.numJoints = 3;
    ss.numForceSensors = 1;
    ss.numImuSensors   = 1;
    ss.hash = 8888;
    ss.shm_key = 8889;
    //ss.extraDataSize = 0;
    ss.extraDataSize = 96;
    //ss.jointType = isc::ShmSettings::JointType::PositionCommand;
    ss.jointType = isc::ShmSettings::JointType::PositionCommand | isc::ShmSettings::JointType::VelocityCommand | isc::ShmSettings::JointType::TorqueCommand;
    std::cout << "TotalSize: "  << ss.calcTotalSize() << std::endl;
    std::cout << "SingleSize: " << ss.getSizeOfSingleJointData() << std::endl;

    std::vector<uint8_t> data; // not required
    ss.setHeaderData(data);    // not required

    ////
    isc::ShmManager sm(ss);

    bool res;
    res = sm.openSharedMemory(true);
    std::cout << "open: " << res << std::endl;
    //res = sm.writeHeader();
    //std::cout << "writeHeader: " << res << std::endl;

    std::cout << "isOpen: " << sm.isOpen() << std::endl;

    sm.resetFrame();
    irt::RealtimeContext rt(1000000);
    int cntr = 0;
    rt.start();
    while(true) {
        cntr++;
        sm.incrementFrame();
        if (cntr > 1000) {
            std::cout << "max: " << rt.getMaxInterval() << std::endl;
            std::cout << "norm: " << rt.getNorm() << std::endl;
            rt.reset();
            cntr = 0;
        }
        rt.waitNextFrame();
    }
    // polling
    return 0;
}
