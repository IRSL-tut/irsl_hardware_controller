#ifndef __IRSL_SHM_CONTROLLER__
#define __IRSL_SHM_CONTROLLER__

#include <cstdint>
#include <string>
#include <vector>

namespace irsl_shm_controller
{

//USE_DOUBLE_PRECISION
typedef double irsl_float_type;
//typedef float irsl_float_type;

#define SIZE_OF_FORCE_SENSOR 6
#define SIZE_OF_IMU_SENSOR   13
#define SIZE_OF_HEADER       24

#define def_offset_method(fname) \
    int getOffset##fname()

struct ShmSettings {
    ShmSettings() : shm_key(0), hash(0), numJoints(0), numForceSensors(0), numImuSensors(0), totalSize(0), extraDataSize(0), jointType(0)
    {}

    ////
    enum JointType { // 16bit
        PositionCommand = 1 << 0,
        PositionGains   = 1 << 1,
        VelocityCommand = 1 << 2,
        VelocityGains   = 1 << 3,
        TorqueCommand   = 1 << 4,
        TorqueGains     = 1 << 5,
        MotorTemperature = 1 << 6,
        MotorCurrent    = 1 << 7,
    };

    //offset query
    uint32_t shm_key;
    uint64_t hash;
    //
    int numJoints;
    int numForceSensors;
    int numImuSensors;
    //
    uint64_t totalSize;
    uint64_t extraDataSize;
    //
    uint16_t jointType;

    // "" is no-name
    std::vector<std::string> jointNames;
    std::vector<std::string> forceSensorNames;
    // "*" in imuSensor is combined
    std::vector<std::string> imuSensorNames;
    std::vector<std::string> accelSensorNames; // part of IMU
    std::vector<std::string> gyroSensorNames;  // part of IMU
    std::vector<std::string> geomagneticSensorNames; // part of IMU
    std::vector<std::string> orientationSensorNames; // part of IMU

    std::vector<irsl_float_type> initialPositionPGains;
    std::vector<irsl_float_type> initialPositionDGains;
    std::vector<irsl_float_type> initialVelocityPGains;
    std::vector<irsl_float_type> initialVelocityDGains;
    std::vector<irsl_float_type> initialTorquePGains;
    std::vector<irsl_float_type> initialTorqueDGains;

    uint64_t calcTotalSize();
    uint64_t getSizeOfSingleJointData();
    void setHeaderData(std::vector<uint8_t> &data);
    bool equal(const ShmSettings &settings);

    ////
    def_offset_method(Data);
    def_offset_method(Status);
    def_offset_method(PositionCurrent);
    def_offset_method(PositionCurrentAux);
    def_offset_method(PositionCommand);
    def_offset_method(PositionPgain);
    def_offset_method(PositionDgain);
    def_offset_method(VelocityCurrent);
    def_offset_method(VelocityCommand);
    def_offset_method(VelocityPgain);
    def_offset_method(VelocityDgain);
    def_offset_method(TorqueCurrent);
    def_offset_method(TorqueCommand);
    def_offset_method(TorquePgain);
    def_offset_method(TorqueDgain);
    def_offset_method(ForceSensor);
    def_offset_method(ImuSensor);
    def_offset_method(ExtraData);
};

#undef def_offset_method

struct ShmDataHeader {
    uint8_t  header[SIZE_OF_HEADER]; // hash(8) | size(4), extraSize(4) | sizeInfo(8){joint(2), jointType(2), force(1), imu(1), reserve(2)}
    uint64_t frame;
    int32_t  sec;
    int32_t  nsec;
    //// getter
    uint64_t getHash();
    int getTotalSize();
    int getExtraDataSize();
    int getNumJoints();
    uint16_t getJointType();
    int getNumForceSensors();
    int getNumImuSensors();
    void writeSettings(ShmSettings &result);
};

#define define_read_write_method(fname,vartype)         \
    bool read##fname (std::vector<vartype> &res);       \
    bool write##fname (const std::vector<vartype> &res)

class ShmManager {

public:
    ShmManager();
    ShmManager(const std::string &fname);
    ShmManager(const ShmSettings &settings);

public:
    bool readSettings(const std::string &fname);
    bool openSharedMemory(bool create = true, uint16_t permission = 0777);

    bool hasSettings();
    const ShmSettings &settings();
    void setSettings(const ShmSettings &settings);
    bool isOpen();

    bool writeHeader();
    bool checkHeader();
    bool readFromHeader(ShmSettings &settings);

    bool setFrame(uint64_t _frame);
    uint64_t getFrame();
    uint64_t resetFrame();
    uint64_t incrementFrame();

    bool getTime(int32_t &sec, int32_t &nsec);
    bool setTime(const int32_t sec, const int32_t nsec);
    ////
    define_read_write_method(Status,uint64_t);
    define_read_write_method(PositionCurrent,irsl_float_type);
    define_read_write_method(PositionCurrentAux,irsl_float_type);
    define_read_write_method(PositionCommand,irsl_float_type);
    define_read_write_method(PositionPgain,irsl_float_type);
    define_read_write_method(PositionDgain,irsl_float_type);
    //
    define_read_write_method(VelocityCurrent,irsl_float_type);
    define_read_write_method(VelocityCommand,irsl_float_type);
    define_read_write_method(VelocityPgain,irsl_float_type);
    define_read_write_method(VelocityDgain,irsl_float_type);
    //
    define_read_write_method(TorqueCurrent,irsl_float_type);
    define_read_write_method(TorqueCommand,irsl_float_type);
    define_read_write_method(TorquePgain,irsl_float_type);
    define_read_write_method(TorqueDgain,irsl_float_type);

    bool readForceSensor(int id, std::vector<irsl_float_type> &res);
    bool writeForceSensor(int id, const std::vector<irsl_float_type> &res);

    bool readImuSensor(int id, std::vector<irsl_float_type> &res);
    bool writeImuSensor(int id, const std::vector<irsl_float_type> &res);

    ShmDataHeader *getHeader();
    bool copyHeader(ShmDataHeader &result);

    bool copyData(std::vector<uint8_t> &buffer);
    bool copyData(uint8_t *buffer);
private:
    class Impl;
    Impl *impl;
};

#undef define_read_write_method

//
void *open_shared_memory(const uint32_t _key, const uint64_t _size, int &shm_id, bool create = true, uint16_t permission = 0777);

}

#endif //__IRSL_SHM_CONTROLLER__
