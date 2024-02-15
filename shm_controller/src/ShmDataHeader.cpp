#include "irsl/shm_controller.h"

using namespace irsl_shm_controller;

uint64_t ShmDataHeader::getHash()
{
    uint64_t *res = (uint64_t *)((void *)header + 0);
    return *res;
}
int ShmDataHeader::getTotalSize()
{
    uint32_t *res = (uint32_t *)((void *)header + 8);
    return *res;
}
int ShmDataHeader::getExtraDataSize()
{
    uint32_t *res = (uint32_t *)((void *)header + 12);
    return *res;
}
int ShmDataHeader::getNumJoints()
{
    uint16_t *res = (uint16_t *)((void *)header + 16);
    return *res;
}
uint16_t ShmDataHeader::getJointType()
{
    uint16_t *res = (uint16_t *)((void *)header + 18);
    return *res;
}
int ShmDataHeader::getNumForceSensor()
{
    uint8_t *res = (uint8_t *)((void *)header + 20);
    return *res;
}
int ShmDataHeader::getNumImuSensor()
{
    uint8_t *res = (uint8_t *)((void *)header + 21);
    return *res;
}
