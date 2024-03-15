#include "irsl/shm_controller.h"
#include <vector>

using namespace irsl_shm_controller;

extern "C" {
int open_shm(int key, int num_joints);
int readPositionCurrent(int size, double *vec);
int readPositionCommand(int size, double *vec);
int writePositionCurrent(int size, double *vec);
int writePositionCommand(int size, double *vec);
}

ShmManager *sm;

int open_shm(int key, int num_joints)
{
    ShmSettings ss;
    ss.numJoints = num_joints;
    ss.numForceSensors = 0;
    ss.numImuSensors   = 0;
    ss.hash    = 8888;
    ss.shm_key = key;
    //ss.extraDataSize = 0;
    //ss.extraDataSize = 96;
    ss.jointType = ShmSettings::PositionCommand | ShmSettings::PositionGains;

    sm = nullptr;

    sm = new ShmManager(ss);

    bool res = sm->openSharedMemory();
    if (!res) {
        return 1;
    }

    res = sm->writeHeader();
    if (!res) {
        return 2;
    }

    res = sm->isOpen();
    if (!res) {
        return 3;
    }

    return 0;
}

std::vector<irsl_float_type> _rd_buffer;
int readPositionCurrent(int size, double *vec)
{
    if (!!sm) {
        _rd_buffer.resize(size);
        bool res = sm->readPositionCurrent(_rd_buffer);
        if (!res) {
            return 1;
        }
        int sz = size <= _rd_buffer.size() ? size : _rd_buffer.size();
        for(int i = 0; i < sz; i++) {
            vec[i] = _rd_buffer[i];
        }
        return 0; // success
    }
    return 2;
}
int readPositionCommand(int size, double *vec)
{
    if (!!sm) {
        _rd_buffer.resize(size);
        bool res = sm->readPositionCommand(_rd_buffer);
        if (!res) {
            return 1;
        }
        int sz = size <= _rd_buffer.size() ? size : _rd_buffer.size();
        for(int i = 0; i < sz; i++) {
            vec[i] = _rd_buffer[i];
        }
        return 0; // success
    }
    return 2;
}

std::vector<irsl_float_type> _wt_buffer;
int writePositionCurrent(int size, double *vec)
{
    if (!!sm) {
        _wt_buffer.resize(size);
        for(int i = 0; i < size; i++) {
            _wt_buffer[i] = vec[i];
        }
        bool res = sm->writePositionCurrent(_wt_buffer);
        if (!res) {
            return 1;
        }
        return 0; // success
    }
    return 2;
}
int writePositionCommand(int size, double *vec)
{
    if (!!sm) {
        _wt_buffer.resize(size);
        for(int i = 0; i < size; i++) {
            _wt_buffer[i] = vec[i];
        }
        bool res = sm->writePositionCommand(_wt_buffer);
        if (!res) {
            return 1;
        }
        return 0; // success
    }
    return 2;
}
