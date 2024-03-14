#include "irsl/shm_controller.h"
#include "irsl_shm_data.hpp"
#include <cstring> // memcpy
#include <iostream>

using namespace irsl_shm_controller;

class ShmManager::Impl : public ShmData
{
public:
    Impl() {};

public:
    ShmSettings current_settings;
};

//
ShmManager::ShmManager()
{
    impl = new Impl;
}
ShmManager::ShmManager(const std::string &fname) : ShmManager()
{
    this->readSettings(fname);
    // TODO: check result
}
ShmManager::ShmManager(const ShmSettings &settings) : ShmManager()
{
    this->setSettings(settings);
    // TODO: check result
}
//
bool ShmManager::readSettings(const std::string &fname)
{
    return true;
}
bool ShmManager::openSharedMemory(bool create, uint16_t permission)
{
    if(hasSettings() && impl->current_settings.totalSize > 0) {
        void *res = open_shared_memory(impl->current_settings.shm_key, impl->current_settings.totalSize,
                                       impl->shm_id, create, permission);
        if (!!res) {
            impl->setPointer(res);
        } else {
            return false;
        }
        return true;
    }
    // TODO: check result
    return false;
}
bool ShmManager::hasSettings()
{
    if (impl->current_settings.hash != 0) {
        return true;
    }
    return false;
}
const ShmSettings &ShmManager::settings()
{
    return impl->current_settings;
}
void ShmManager::setSettings(const ShmSettings &settings)
{
    impl->current_settings = settings;
    if (impl->current_settings.totalSize <= 0) {
        impl->current_settings.calcTotalSize();

    }
}
bool ShmManager::isOpen()
{
    return impl->isInitialized();
}
//
bool ShmManager::writeHeader()
{
    return impl->writeHeader(impl->current_settings);
}
bool ShmManager::checkHeader()
{
    ShmSettings hdr_settings;
    bool res = readFromHeader(hdr_settings);
    if (!res) return false;
    bool update = hdr_settings.equal(impl->current_settings);
    if (!update) return false;
    impl->updateBySettings(impl->current_settings);
    return true;
}
bool ShmManager::readFromHeader(ShmSettings &settings)
{
    ShmDataHeader *hdr = getHeader();
    if (!hdr) return false;
    hdr->writeSettings(settings);
    return true;
}
//
bool ShmManager::setFrame(uint64_t _frame)
{
    return impl->setFrame(_frame);
}
uint64_t ShmManager::getFrame()
{
    return impl->getFrame();
}
uint64_t ShmManager::resetFrame()
{
    return impl->resetFrame();
}
uint64_t ShmManager::incrementFrame()
{
    return impl->incrementFrame();
}
//
bool ShmManager::getTime(int32_t &sec, int32_t &nsec)
{
    if (!(impl->ptr)) return false;
    ShmDataHeader *pp = (ShmDataHeader *)impl->ptr;
    sec  = pp->sec;
    nsec = pp->nsec;
    return true;
}
bool ShmManager::setTime(const int32_t sec, const int32_t nsec)
{
    if (!(impl->ptr)) return false;
    ShmDataHeader *pp = (ShmDataHeader *)impl->ptr;
    pp->sec  = sec;
    pp->nsec = nsec;
    return true;
}

#define generate_read_write_method(fname,vartype)          \
bool ShmManager::read##fname (std::vector<vartype> &res) \
{                                                        \
    return impl->read##fname(res);                       \
}                                                               \
bool ShmManager::write##fname (const std::vector<vartype> &res) \
{                                                               \
    return impl->write##fname(res);                             \
}

generate_read_write_method(Status,uint64_t);
generate_read_write_method(PositionCurrent,irsl_float_type);
generate_read_write_method(PositionCurrentAux,irsl_float_type);
generate_read_write_method(PositionCommand,irsl_float_type);
generate_read_write_method(PositionPgain,irsl_float_type);
generate_read_write_method(PositionDgain,irsl_float_type);
//
generate_read_write_method(VelocityCurrent,irsl_float_type);
generate_read_write_method(VelocityCommand,irsl_float_type);
generate_read_write_method(VelocityPgain,irsl_float_type);
generate_read_write_method(VelocityDgain,irsl_float_type);
//
generate_read_write_method(TorqueCurrent,irsl_float_type);
generate_read_write_method(TorqueCommand,irsl_float_type);
generate_read_write_method(TorquePgain,irsl_float_type);
generate_read_write_method(TorqueDgain,irsl_float_type);

bool ShmManager::readForceSensor(int id, std::vector<irsl_float_type> &res)
{
    return impl->readForceSensor(id, res);
}
bool ShmManager::writeForceSensor(int id, const std::vector<irsl_float_type> &res)
{
    return impl->writeForceSensor(id, res);
}
bool ShmManager::readImuSensor(int id, std::vector<irsl_float_type> &res)
{
    return impl->readImuSensor(id, res);
}
bool ShmManager::writeImuSensor(int id, const std::vector<irsl_float_type> &res)
{
    return impl->writeImuSensor(id, res);
}
//
ShmDataHeader *ShmManager::getHeader()
{
    return (ShmDataHeader *)impl->ptr;
}
bool ShmManager::copyHeader(ShmDataHeader &result)
{
    if (!impl->ptr) return false;
    void *res = (void *)(&result);
    std::memcpy(res, impl->ptr, sizeof(ShmDataHeader));
    return true;
}
bool ShmManager::copyData(std::vector<uint8_t> &buffer)
{
    if (!impl->ptr) return false;
    buffer.resize(impl->current_settings.totalSize);
    std::memcpy((void *)buffer.data(), impl->ptr, impl->current_settings.totalSize);
    return true;
}
bool ShmManager::copyData(uint8_t *buffer)
{
    if (!impl->ptr) return false;
    std::memcpy((void *)buffer, impl->ptr, impl->current_settings.totalSize);
    return true;
}
