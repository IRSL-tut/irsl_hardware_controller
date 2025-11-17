# %autoindent
import sys
sys.path.append('.')
import irsl_shm

sm_client = irsl_shm.ShmManager()
sm_client.settings().shm_key = 9999
sm_client.settings().hash = 6666
sm_client.openSharedMemory(False)

sm_client.isOpen()

# sm_host.writePositionCurrent([1., 2., 3., 4., 5.])
# sm_host.writeVelocityCurrent([10., 20., 30., 40., 50.])
# sm_host.writeTorqueCurrent([100., 200., 300., 400., 500.])

# sm_host.writePositionCommand([-0.1, -0.2, -0.3, -0.4, -0.5])
# sm_host.writeVelocityCommand([-10., -20., -30., -40., -50.])

sm_client.readPositionCurrent()
sm_client.readVelocityCurrent()
sm_client.readTorqueCurrent()

sm_client.readPositionCommand()
sm_client.readVelocityCommand()
sm_client.readTorqueCommand() ## fail
