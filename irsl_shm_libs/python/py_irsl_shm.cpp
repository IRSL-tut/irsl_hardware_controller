#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "irsl/shm_controller.h"

namespace py = pybind11;
using namespace irsl_shm_controller;

// Small helpers to wrap read/write APIs into Pythonic return values
template <typename T>
static std::vector<T> read_vec(auto &&fn, const char *what)
{
    std::vector<T> out;
    if (!fn(out)) {
        throw std::runtime_error(std::string("Failed to read ") + what);
    }
    return out;
}

template <typename T>
static void write_vec(auto &&fn, const std::vector<T> &data, const char *what)
{
    if (!fn(data)) {
        throw std::runtime_error(std::string("Failed to write ") + what);
    }
}

PYBIND11_MODULE(irsl_shm, m)
{
    m.doc() = "Python bindings for IRSL shared memory controller";

    // Expose some constants
    m.attr("SIZE_OF_FORCE_SENSOR") = py::int_(SIZE_OF_FORCE_SENSOR);
    m.attr("SIZE_OF_IMU_SENSOR") = py::int_(SIZE_OF_IMU_SENSOR);
    m.attr("SIZE_OF_HEADER") = py::int_(SIZE_OF_HEADER);

    py::class_<ShmSettings>(m, "ShmSettings")
        .def(py::init<>())
        .def_readwrite("shm_key", &ShmSettings::shm_key)
        .def_readwrite("hash", &ShmSettings::hash)
        .def_readwrite("numJoints", &ShmSettings::numJoints)
        .def_readwrite("numForceSensors", &ShmSettings::numForceSensors)
        .def_readwrite("numImuSensors", &ShmSettings::numImuSensors)
        .def_readwrite("totalSize", &ShmSettings::totalSize)
        .def_readwrite("extraDataSize", &ShmSettings::extraDataSize)
        .def_readwrite("jointType", &ShmSettings::jointType)
#if 0
        .def_readwrite("jointNames", &ShmSettings::jointNames)
        .def_readwrite("forceSensorNames", &ShmSettings::forceSensorNames)
        .def_readwrite("imuSensorNames", &ShmSettings::imuSensorNames)
        .def_readwrite("accelSensorNames", &ShmSettings::accelSensorNames)
        .def_readwrite("gyroSensorNames", &ShmSettings::gyroSensorNames)
        .def_readwrite("geomagneticSensorNames", &ShmSettings::geomagneticSensorNames)
        .def_readwrite("orientationSensorNames", &ShmSettings::orientationSensorNames)
#endif
        .def_readwrite("initialPositionPGains", &ShmSettings::initialPositionPGains)
        .def_readwrite("initialPositionDGains", &ShmSettings::initialPositionDGains)
        .def_readwrite("initialVelocityPGains", &ShmSettings::initialVelocityPGains)
        .def_readwrite("initialVelocityDGains", &ShmSettings::initialVelocityDGains)
        .def_readwrite("initialTorquePGains", &ShmSettings::initialTorquePGains)
        .def_readwrite("initialTorqueDGains", &ShmSettings::initialTorqueDGains)
        .def("calcTotalSize", &ShmSettings::calcTotalSize)
        .def("getSizeOfSingleJointData", &ShmSettings::getSizeOfSingleJointData)
        .def("equal", &ShmSettings::equal)
        // Offset helpers
        .def("getOffsetData", &ShmSettings::getOffsetData)
        .def("getOffsetStatus", &ShmSettings::getOffsetStatus)
        .def("getOffsetPositionCurrent", &ShmSettings::getOffsetPositionCurrent)
        .def("getOffsetPositionCurrentAux", &ShmSettings::getOffsetPositionCurrentAux)
        .def("getOffsetPositionCommand", &ShmSettings::getOffsetPositionCommand)
        .def("getOffsetPositionPgain", &ShmSettings::getOffsetPositionPgain)
        .def("getOffsetPositionDgain", &ShmSettings::getOffsetPositionDgain)
        .def("getOffsetVelocityCurrent", &ShmSettings::getOffsetVelocityCurrent)
        .def("getOffsetVelocityCommand", &ShmSettings::getOffsetVelocityCommand)
        .def("getOffsetVelocityPgain", &ShmSettings::getOffsetVelocityPgain)
        .def("getOffsetVelocityDgain", &ShmSettings::getOffsetVelocityDgain)
        .def("getOffsetTorqueCurrent", &ShmSettings::getOffsetTorqueCurrent)
        .def("getOffsetTorqueCommand", &ShmSettings::getOffsetTorqueCommand)
        .def("getOffsetTorquePgain", &ShmSettings::getOffsetTorquePgain)
        .def("getOffsetTorqueDgain", &ShmSettings::getOffsetTorqueDgain)
        .def("getOffsetForceSensor", &ShmSettings::getOffsetForceSensor)
        .def("getOffsetImuSensor", &ShmSettings::getOffsetImuSensor)
        .def("getOffsetExtraData", &ShmSettings::getOffsetExtraData)
        ;

    py::enum_<ShmSettings::JointType>(m, "JointType", py::arithmetic())
        .value("PositionCommand", ShmSettings::JointType::PositionCommand)
        .value("PositionGains", ShmSettings::JointType::PositionGains)
        .value("VelocityCommand", ShmSettings::JointType::VelocityCommand)
        .value("VelocityGains", ShmSettings::JointType::VelocityGains)
        .value("TorqueCommand", ShmSettings::JointType::TorqueCommand)
        .value("TorqueGains", ShmSettings::JointType::TorqueGains)
        .value("MotorTemperature", ShmSettings::JointType::MotorTemperature)
        .value("MotorCurrent", ShmSettings::JointType::MotorCurrent)
        .export_values();

    py::class_<ShmManager>(m, "ShmManager")
        .def(py::init<>())
        .def(py::init<const std::string &>())
        .def(py::init<const ShmSettings &>())
        .def("readSettings", &ShmManager::readSettings, py::arg("fname"))
        .def("openSharedMemory", &ShmManager::openSharedMemory, py::arg("create") = true, py::arg("permission") = 0777)
        .def("hasSettings", &ShmManager::hasSettings)
        .def("settings", &ShmManager::settings, py::return_value_policy::reference_internal)
        .def("setSettings", &ShmManager::setSettings)
        .def("isOpen", &ShmManager::isOpen)
        .def("writeHeader", &ShmManager::writeHeader)
        .def("checkHeader", &ShmManager::checkHeader)
        .def("readFromHeader", &ShmManager::readFromHeader)
        .def("setFrame", &ShmManager::setFrame)
        .def("getFrame", &ShmManager::getFrame)
        .def("resetFrame", &ShmManager::resetFrame)
        .def("incrementFrame", &ShmManager::incrementFrame)
        .def("getTime", [](ShmManager &self) {
            int32_t s = 0, ns = 0;
            if (!self.getTime(s, ns)) {
                throw std::runtime_error("Failed to get time");
            }
            return py::make_tuple(s, ns);
        })
        .def("setTime", &ShmManager::setTime, py::arg("sec"), py::arg("nsec"))
        // Data read helpers returning vectors
        .def("readStatus", [](ShmManager &self, int offset) {
            return read_vec<uint64_t>([&](auto &v){ return self.readStatus(v, offset); }, "Status"); }, py::arg("offset")=0)
        .def("readPositionCurrent", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readPositionCurrent(v, offset); }, "PositionCurrent"); }, py::arg("offset")=0)
        .def("readPositionCurrentAux", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readPositionCurrentAux(v, offset); }, "PositionCurrentAux"); }, py::arg("offset")=0)
        .def("readPositionCommand", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readPositionCommand(v, offset); }, "PositionCommand"); }, py::arg("offset")=0)
        .def("readPositionPgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readPositionPgain(v, offset); }, "PositionPgain"); }, py::arg("offset")=0)
        .def("readPositionDgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readPositionDgain(v, offset); }, "PositionDgain"); }, py::arg("offset")=0)
        .def("readVelocityCurrent", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readVelocityCurrent(v, offset); }, "VelocityCurrent"); }, py::arg("offset")=0)
        .def("readVelocityCommand", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readVelocityCommand(v, offset); }, "VelocityCommand"); }, py::arg("offset")=0)
        .def("readVelocityPgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readVelocityPgain(v, offset); }, "VelocityPgain"); }, py::arg("offset")=0)
        .def("readVelocityDgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readVelocityDgain(v, offset); }, "VelocityDgain"); }, py::arg("offset")=0)
        .def("readTorqueCurrent", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readTorqueCurrent(v, offset); }, "TorqueCurrent"); }, py::arg("offset")=0)
        .def("readTorqueCommand", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readTorqueCommand(v, offset); }, "TorqueCommand"); }, py::arg("offset")=0)
        .def("readTorquePgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readTorquePgain(v, offset); }, "TorquePgain"); }, py::arg("offset")=0)
        .def("readTorqueDgain", [](ShmManager &self, int offset) {
            return read_vec<irsl_float_type>([&](auto &v){ return self.readTorqueDgain(v, offset); }, "TorqueDgain"); }, py::arg("offset")=0)
        .def("readForceSensor", [](ShmManager &self, int id) {
            std::vector<irsl_float_type> out;
            if (!self.readForceSensor(id, out)) {
                throw std::runtime_error("Failed to read ForceSensor");
            }
            return out;
        }, py::arg("id"))
        .def("readImuSensor", [](ShmManager &self, int id) {
            std::vector<irsl_float_type> out;
            if (!self.readImuSensor(id, out)) {
                throw std::runtime_error("Failed to read ImuSensor");
            }
            return out;
        }, py::arg("id"))
        // Write helpers consuming Python lists
        .def("writeStatus", [](ShmManager &self, const std::vector<uint64_t> &v, int offset) {
            write_vec<uint64_t>([&](auto &x){ return self.writeStatus(x, offset); }, v, "Status"); }, py::arg("v"), py::arg("offset")=0)
        .def("writePositionCurrent", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writePositionCurrent(x, offset); }, v, "PositionCurrent"); }, py::arg("v"), py::arg("offset")=0)
        .def("writePositionCurrentAux", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writePositionCurrentAux(x, offset); }, v, "PositionCurrentAux"); }, py::arg("v"), py::arg("offset")=0)
        .def("writePositionCommand", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writePositionCommand(x, offset); }, v, "PositionCommand"); }, py::arg("v"), py::arg("offset")=0)
        .def("writePositionPgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writePositionPgain(x, offset); }, v, "PositionPgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writePositionDgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writePositionDgain(x, offset); }, v, "PositionDgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeVelocityCurrent", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeVelocityCurrent(x, offset); }, v, "VelocityCurrent"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeVelocityCommand", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeVelocityCommand(x, offset); }, v, "VelocityCommand"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeVelocityPgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeVelocityPgain(x, offset); }, v, "VelocityPgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeVelocityDgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeVelocityDgain(x, offset); }, v, "VelocityDgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeTorqueCurrent", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeTorqueCurrent(x, offset); }, v, "TorqueCurrent"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeTorqueCommand", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeTorqueCommand(x, offset); }, v, "TorqueCommand"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeTorquePgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeTorquePgain(x, offset); }, v, "TorquePgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeTorqueDgain", [](ShmManager &self, const std::vector<irsl_float_type> &v, int offset) {
            write_vec<irsl_float_type>([&](auto &x){ return self.writeTorqueDgain(x, offset); }, v, "TorqueDgain"); }, py::arg("v"), py::arg("offset")=0)
        .def("writeForceSensor", [](ShmManager &self, int id, const std::vector<irsl_float_type> &v) {
            if (!self.writeForceSensor(id, v)) {
                throw std::runtime_error("Failed to write ForceSensor");
            }
        }, py::arg("id"), py::arg("values"))
        .def("writeImuSensor", [](ShmManager &self, int id, const std::vector<irsl_float_type> &v) {
            if (!self.writeImuSensor(id, v)) {
                throw std::runtime_error("Failed to write ImuSensor");
            }
        }, py::arg("id"), py::arg("values"))
        // Header snapshot as dict
        .def("getHeaderSnapshot", [](ShmManager &self) {
            ShmDataHeader h{};
            if (!self.copyHeader(h)) {
                throw std::runtime_error("Failed to copy header");
            }
            py::dict d;
            d["hash"] = py::int_(h.getHash());
            d["totalSize"] = py::int_(h.getTotalSize());
            d["extraDataSize"] = py::int_(h.getExtraDataSize());
            d["numJoints"] = py::int_(h.getNumJoints());
            d["jointType"] = py::int_(h.getJointType());
            d["numForceSensors"] = py::int_(h.getNumForceSensors());
            d["numImuSensors"] = py::int_(h.getNumImuSensors());
            d["frame"] = py::int_(h.frame);
            d["sec"] = py::int_(h.sec);
            d["nsec"] = py::int_(h.nsec);
            return d;
        })
        ;
}
