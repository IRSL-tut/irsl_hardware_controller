#include "irsl/shm_controller.h"

using namespace irsl_shm_controller;

uint64_t ShmSettings::getSizeOfSingleJointData()
{
    // numJoints
    int size = 0;
    size += 1; // position_current
    size += 1; // position_current_aux
    size += 1; // status
    if (jointType & JointType::PositionCommand) {
        size += 1;
    }
    if (jointType & JointType::PositionGains) {
        size += 2;
    }
    size += 1; // velocity_current
    if (jointType & JointType::VelocityCommand) {
        size += 1;
    }
    if (jointType & JointType::VelocityGains) {
        size += 2;
    }
    size += 1; // torque_current
    if (jointType & JointType::TorqueCommand) {
        size += 1;
    }
    if (jointType & JointType::TorqueGains) {
        size += 2;
    }
    return size*sizeof(irsl_float_type);
}

void ShmSettings::setHeaderData(std::vector<uint8_t> &data)
{
    data.resize(SIZE_OF_HEADER);
    void *ptr = (void *)data.data();

    uint64_t *p_hash  = (uint64_t *)(ptr + 0);
    uint32_t *p_size  = (uint32_t *)(ptr + 8);
    uint32_t *p_ex_sz = (uint32_t *)(ptr + 12);
    uint16_t *p_jsize = (uint16_t *)(ptr + 16);
    uint16_t *p_jtype = (uint16_t *)(ptr + 18);
    uint8_t  *p_fsize = (uint8_t  *)(ptr + 20);
    uint8_t  *p_isize = (uint8_t  *)(ptr + 21);

    *p_hash  = hash;
    *p_size  = totalSize;
    *p_ex_sz = extraDataSize;
    *p_jsize = numJoints;
    *p_jtype = jointType;
    *p_fsize = numForceSensors;
    *p_isize = numImuSensors;
}

uint64_t ShmSettings::calcTotalSize()
{
    uint64_t size = sizeof(ShmDataHeader);
    size += (getSizeOfSingleJointData() * numJoints);
    size += (SIZE_OF_FORCE_SENSOR * numForceSensors * sizeof(irsl_float_type));
    size += (SIZE_OF_IMU_SENSOR * numImuSensors * sizeof(irsl_float_type));
    size += extraDataSize;
    totalSize = size;
    return size;
}

//// getOffset defined in ShmSettings/shm_controller.h
int ShmSettings::getOffsetData()
{
    return sizeof(ShmDataHeader);
}
int ShmSettings::getOffsetStatus()
{
    return sizeof(ShmDataHeader) + 0*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetPositionCurrent()
{
    return sizeof(ShmDataHeader) + 1*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetPositionCurrentAux()
{
    return sizeof(ShmDataHeader) + 2*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetPositionCommand()
{
    if (!(jointType & JointType::PositionCommand)) return -1;
    return sizeof(ShmDataHeader) + 3*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetPositionPgain()
{
    if (!(jointType & JointType::PositionGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    return sizeof(ShmDataHeader) + off_*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetPositionDgain()
{
    if (!(jointType & JointType::PositionGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    return sizeof(ShmDataHeader) + (off_ + 1)*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetVelocityCurrent()
{
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    return sizeof(ShmDataHeader) + off_*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetVelocityCommand()
{
    if (!(jointType & JointType::VelocityCommand)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    return sizeof(ShmDataHeader) + (off_ + 1)*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetVelocityPgain()
{
    if (!(jointType & JointType::VelocityGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    return sizeof(ShmDataHeader) + off_*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetVelocityDgain()
{
    if (!(jointType & JointType::VelocityGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    return sizeof(ShmDataHeader) + (off_ + 1)*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetTorqueCurrent()
{
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    if (jointType & JointType::VelocityGains)   off_ += 2;
    return sizeof(ShmDataHeader) + off_*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetTorqueCommand()
{
    if (!(jointType & JointType::TorqueCommand)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    if (jointType & JointType::VelocityGains)   off_ += 2;
    return sizeof(ShmDataHeader) + (off_ + 1)*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetTorquePgain()
{
    if (!(jointType & JointType::VelocityGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    if (jointType & JointType::VelocityGains)   off_ += 2;
    if (jointType & JointType::TorqueCommand)   off_ += 1;
    return sizeof(ShmDataHeader) + off_*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetTorqueDgain()
{
    if (!(jointType & JointType::VelocityGains)) return -1;
    int off_ = 3;
    if (jointType & JointType::PositionCommand) off_ += 1;
    if (jointType & JointType::PositionGains)   off_ += 2;
    if (jointType & JointType::VelocityCommand) off_ += 1;
    if (jointType & JointType::VelocityGains)   off_ += 2;
    if (jointType & JointType::TorqueCommand)   off_ += 1;
    return sizeof(ShmDataHeader) + (off_ + 1)*sizeof(irsl_float_type);
}
int ShmSettings::getOffsetForceSensor()
{
    int size = getSizeOfSingleJointData();
    return sizeof(ShmDataHeader) + (size * numJoints);
}
int ShmSettings::getOffsetImuSensor()
{
    int size = getSizeOfSingleJointData();
    return sizeof(ShmDataHeader) + (size * numJoints) + (SIZE_OF_FORCE_SENSOR * numForceSensors * sizeof(irsl_float_type));
}
int ShmSettings::getOffsetExtraData()
{
    int size = getSizeOfSingleJointData();
    return sizeof(ShmDataHeader) + (size * numJoints) + (SIZE_OF_FORCE_SENSOR * numForceSensors * sizeof(irsl_float_type)) + (SIZE_OF_IMU_SENSOR * numImuSensors * sizeof(irsl_float_type));
}
