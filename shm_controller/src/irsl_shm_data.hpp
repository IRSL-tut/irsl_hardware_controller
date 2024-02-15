#ifndef __IRSL_SHM_DATA__
#define __IRSL_SHM_DATA__

#include "irsl/shm_controller.h"

namespace irsl_shm_controller
{

#define define_joint_method(fname,vartype)                              \
    inline bool read##fname (std::vector<vartype> &res) {               \
        int offset_ = offset_##fname;                                   \
        if (offset_ < 0) { return false; }                              \
        if(res.size() != joint_size) {                                  \
            res.resize(joint_size);                                     \
        }                                                               \
        vartype *dst = (vartype *)res.data();                           \
        const vartype *src = (const vartype *)(ptr + offset_);          \
        for(int i = 0; i < joint_size; i++) {                           \
            dst[i] = src[i];                                            \
        }                                                               \
        return true;                                                    \
    }                                                                   \
    inline bool write##fname (const std::vector<vartype> &res) {        \
        int offset_ = offset_##fname;                                   \
        if (offset_ < 0) { return false; }                              \
        int num = res.size() > joint_size ? joint_size : res.size();    \
        vartype *dst = (vartype *)(ptr + offset_);                      \
        const vartype *src = (const vartype *)res.data();               \
        for(int i = 0; i < num; i++) {                                  \
            dst[i] = src[i];                                            \
        }                                                               \
        return true;                                                    \
    }

#define set_offset_func(fname) \
    offset_##fname = settings.getOffset##fname()

struct ShmData {
    int joint_size;
    int offset_Data;
    int offset_Status;
    int offset_PositionCurrent;
    int offset_PositionCurrentAux;
    int offset_PositionCommand;
    int offset_PositionPgain;
    int offset_PositionDgain;
    //
    int offset_Velocity;//
    int offset_VelocityCurrent;
    int offset_VelocityCommand;
    int offset_VelocityPgain;
    int offset_VelocityDgain;
    //
    int offset_Torque;//
    int offset_TorqueCurrent;
    int offset_TorqueCommand;
    int offset_TorquePgain;
    int offset_TorqueDgain;
    //
    int force_size;
    int offset_ForceSensor;
    //
    int imu_size;
    int offset_ImuSensor;
    //
    int extra_size;
    int offset_ExtraData;

    int shm_id;
    void *ptr;
    uint64_t *frame_ptr;

    bool initialized;

    ShmData()
    {
        //
        joint_size = 0;
        offset_Data = -1;
        offset_PositionCurrent    = -1;
        offset_PositionCurrentAux = -1;
        offset_PositionCommand = -1;
        offset_PositionDgain   = -1;
        offset_PositionPgain   = -1;
        offset_Status          = -1;
        //
        offset_Velocity = -1;//
        offset_VelocityCurrent = -1;
        offset_VelocityCommand = -1;
        offset_VelocityPgain   = -1;
        offset_VelocityDgain   = -1;
        //
        offset_Torque = -1;
        offset_TorqueCurrent = -1;
        offset_TorqueCommand = -1;
        offset_TorquePgain   = -1;
        offset_TorqueDgain   = -1;
        //
        force_size = 0;
        offset_ForceSensor = -1;
        //
        imu_size = 0;
        offset_ImuSensor = -1;
        //
        extra_size = 0;
        offset_ExtraData = -1;
        //
        ptr = nullptr;
        frame_ptr = nullptr;
        //
        initialized = false;
    }

    inline void setPointer(void *newp)
    {
        ptr = newp;
        frame_ptr = (uint64_t *)(newp + SIZE_OF_HEADER);
    }

    inline bool isInitialized()
    {
        return (initialized && ptr != nullptr);
    }

    //inline writeTime(sec, nsec);
    //inline getTime(sec, nsec);

    inline bool setFrame(uint64_t _frame)
    {
        *frame_ptr = _frame;
        return true;
    }
    inline uint64_t getFrame()
    {
        return *frame_ptr;
    }
    inline uint64_t resetFrame()
    {
        uint64_t res = *frame_ptr;
        *frame_ptr = 0;
        return res;
    }
    inline uint64_t incrementFrame()
    {
        (*frame_ptr)++;
        return *frame_ptr;
    }

    inline void updateBySettings(ShmSettings &settings)
    {
        initialized = true;

        // add same name of def_offset_method in ShmSettings/shm_controller.h
        set_offset_func(Data);
        set_offset_func(Status);
        set_offset_func(PositionCurrent);
        set_offset_func(PositionCurrentAux);
        set_offset_func(PositionCommand);
        set_offset_func(PositionPgain);
        set_offset_func(PositionDgain);
        set_offset_func(VelocityCurrent);
        set_offset_func(VelocityCommand);
        set_offset_func(VelocityPgain);
        set_offset_func(VelocityDgain);
        set_offset_func(TorqueCurrent);
        set_offset_func(TorqueCommand);
        set_offset_func(TorquePgain);
        set_offset_func(TorqueDgain);
        set_offset_func(ForceSensor);
        set_offset_func(ImuSensor);
        set_offset_func(ExtraData);
    }

    inline bool writeHeader(ShmSettings &settings)
    {
        updateBySettings(settings);
        if (!!ptr) {
            std::vector<uint8_t> data;
            settings.setHeaderData(data);
            uint8_t *dst = (uint8_t *)ptr;
            for(int i = 0; i < SIZE_OF_HEADER; i++) {
                dst[i] = data[i];
            }
            resetFrame();
            // writeTime(0, 0);
        }
        return this->isInitialized();
    }
    //
    // Methods for reading/writing joints
    //
    define_joint_method(Status,uint64_t);
    define_joint_method(PositionCurrent,irsl_float_type);
    define_joint_method(PositionCurrentAux,irsl_float_type);
    define_joint_method(PositionCommand,irsl_float_type);
    define_joint_method(PositionPgain,irsl_float_type);
    define_joint_method(PositionDgain,irsl_float_type);
    //
    define_joint_method(VelocityCurrent,irsl_float_type);
    define_joint_method(VelocityCommand,irsl_float_type);
    define_joint_method(VelocityPgain,irsl_float_type);
    define_joint_method(VelocityDgain,irsl_float_type);
    //
    define_joint_method(TorqueCurrent,irsl_float_type);
    define_joint_method(TorqueCommand,irsl_float_type);
    define_joint_method(TorquePgain,irsl_float_type);
    define_joint_method(TorqueDgain,irsl_float_type);

    inline bool readForceSensor(int id, std::vector<irsl_float_type> &res)
    {
        //TODO: size-check
        if (res.size() != SIZE_OF_FORCE_SENSOR) {
            res.resize(SIZE_OF_FORCE_SENSOR);
        }
        irsl_float_type *cur_ptr = (irsl_float_type *)(ptr + offset_ForceSensor + id * sizeof(irsl_float_type) * SIZE_OF_FORCE_SENSOR);
        for(int i = 0; i < SIZE_OF_FORCE_SENSOR; i++) {
            res[i] = cur_ptr[i];
        }
        return true;
    }
    inline bool writeForceSensor(int id, const std::vector<irsl_float_type> &res)
    {
        //TODO: size-check
        int num = res.size() > SIZE_OF_FORCE_SENSOR ? SIZE_OF_FORCE_SENSOR : res.size();
        irsl_float_type *cur_ptr = (irsl_float_type *)(ptr + offset_ForceSensor + id * sizeof(irsl_float_type) * SIZE_OF_FORCE_SENSOR);
        for(int i = 0; i < num; i++) {
            cur_ptr[i] = res[i];
        }
        return true;
    }
    inline bool readImuSensor(int id, std::vector<irsl_float_type> &res)
    {
        //TODO: size-check
        if (res.size() != SIZE_OF_IMU_SENSOR) {
            res.resize(SIZE_OF_IMU_SENSOR);
        }
        irsl_float_type *cur_ptr = (irsl_float_type *)(ptr + offset_ImuSensor + id * sizeof(irsl_float_type) * SIZE_OF_IMU_SENSOR);
        for(int i = 0; i < SIZE_OF_IMU_SENSOR; i++) {
            res[i] = cur_ptr[i];
        }
        return true;
    }
    inline bool writeImuSensor(int id, const std::vector<irsl_float_type> &res)
    {
        //TODO: size-check
        int num = res.size() > SIZE_OF_IMU_SENSOR ? SIZE_OF_IMU_SENSOR : res.size();
        irsl_float_type *cur_ptr = (irsl_float_type *)(ptr + offset_ImuSensor + id * sizeof(irsl_float_type) * SIZE_OF_IMU_SENSOR);
        for(int i = 0; i < num; i++) {
            cur_ptr[i] = res[i];
        }
        return true;
    }

    //inline void readExtraAsJoints
};

#undef set_offset_func
#undef define_joint_method

}

#endif //__IRSL_SHM_DATA__
