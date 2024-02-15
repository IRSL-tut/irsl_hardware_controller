// #include "irsl/shm_controller.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

#include <iostream>

namespace irsl_shm_controller
{

void *open_shared_memory(const uint32_t _key, const uint64_t _size, int &shm_id, bool create, uint16_t permission)
{
    void *ptr;
    int err;
    if (create) {
        shm_id = shmget(_key, _size, IPC_CREAT | permission); // add permission???
    } else {
        shm_id = shmget(_key, _size, 0); // just use existing shared-memory
    }
    if(shm_id == -1) {
        err = errno;
        std::cerr << "shmget failed, key=" << _key;
        std::cerr << ", size=" << _size;
        std::cerr << ", errno=" << err << std::endl;
        return nullptr;
    }
    ///
    ptr = (void *)shmat(shm_id, (void *)0, 0);
    if(ptr == (void *)-1) {
        err = errno;
        std::cerr << "shmat failed, key=" << _key;
        std::cerr << ", size=" << _size;
        std::cerr << ", shm_id=" << shm_id;
        std::cerr << ", errno=" << err << std::endl;
        return nullptr;
    }
    return ptr;
}

}
