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
settings.shm_key = 0x8888
settings.hash = 6666

total = settings.calcTotalSize()

sm_host = irsl_shm.ShmManager()
sm_host.setSettings(settings)
sm_host.openSharedMemory(True)

csets = irsl_shm.ShmSettings()
csets.shm_key = 0x8888
csets.hash = 6666
sm_client = irsl_shm.ShmManager()
sm_client.setSettings(csets)
sm_client.openSharedMemory(False)

sm_client.checkHeader()

sm_host.writePositionCurrent([1., 2., 3., 4.])
sm_client.readPositionCurrent()

sm_host.writePositionCurrent([10., 20.], 2)
sm_client.readPositionCurrent()

sm_client.readPositionCurrent(1)
