%autoindent
import sys
sys.path.append('.')
import irsl_shm

settings = irsl_shm.ShmSettings()
settings.numJoints = 5
settings.numImuSensors = 1
settings.numForceSensors = 1
settings.extraDataSize = 32
settings.jointType = irsl_shm.PositionCommand | irsl_shm.VelocityCommand
#settings.jointType = irsl_shm.PositionCommand | irsl_shm.VelocityCommand | irsl_shm.TorqueCommand | irsl_shm.PositionGains | irsl_shm.VelocityGains | irsl_shm.TorqueGains
settings.shm_key = 9999
settings.hash = 6666

total = settings.calcTotalSize()

sm_host = irsl_shm.ShmManager()
sm_host.setSettings(settings)
sm_host.openSharedMemory(True)

sm_host.writePositionCurrent([1., 2., 3., 4., 5.])
sm_host.writeVelocityCurrent([10., 20., 30., 40., 50.])
sm_host.writeTorqueCurrent([-1., -2., -3., -4., -5.])

sm_host.writePositionCommand([1., 2., 3., 4., 5.])
sm_host.writeVelocityCommand([10., 20., 30., 40., 50.])

sm_host.readPositionCurrent()
sm_host.readVelocityCurrent()
sm_host.readTorqueCurrent()

sm_host.readPositionCommand()
sm_host.readVelocityCommand()
sm_host.readTorqueCommand() ## fail

cur_st = sm_host.settings()
cur_st.getOffsetData()
cur_st.getOffsetStatus()
cur_st.getOffsetPositionCurrent()
cur_st.getOffsetPositionCurrentAux()
cur_st.getOffsetPositionCommand()
cur_st.getOffsetPositionPgain()
cur_st.getOffsetPositionDgain()
cur_st.getOffsetVelocityCurrent()
cur_st.getOffsetVelocityCommand()
cur_st.getOffsetVelocityPgain()
cur_st.getOffsetVelocityDgain()
cur_st.getOffsetTorqueCurrent()
cur_st.getOffsetTorqueCommand()
cur_st.getOffsetTorquePgain()
cur_st.getOffsetTorqueDgain()
cur_st.getOffsetForceSensor()
cur_st.getOffsetImuSensor()
cur_st.getOffsetExtraData()

sm_host.closeSharedMemory()



