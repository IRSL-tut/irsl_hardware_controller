#pragma once

#include <unordered_map>

namespace irsl_shm_controller
{

static const std::unordered_map<std::string, ShmSettings::JointType> jointTypeMap = {
    {"PositionCommand",  ShmSettings::JointType::PositionCommand},
    {"PositionGains",    ShmSettings::JointType::PositionGains},
    {"VelocityCommand",  ShmSettings::JointType::VelocityCommand},
    {"VelocityGains",    ShmSettings::JointType::VelocityGains},
    {"TorqueCommand",    ShmSettings::JointType::TorqueCommand},
    {"TorqueGains",      ShmSettings::JointType::TorqueGains},
    {"MotorTemperature", ShmSettings::JointType::MotorTemperature},
    {"MotorCurrent",     ShmSettings::JointType::MotorCurrent},
};

inline ShmSettings::JointType getJointType(const std::string &type)
{
    auto it = jointTypeMap.find(type);
    if (it != jointTypeMap.end()) {
        return it->second;
    }
    return ShmSettings::JointType::INVALID;
}

}
