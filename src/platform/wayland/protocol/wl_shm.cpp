#include "wl_shm.hpp"
#include "wl_shm_pool.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<ShmPool> Shm::createShmPool(uint32_t size, int fd){
    auto s = std::make_unique<ShmPool>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    std::vector<int> fds;
    fds.push_back(fd);
    display.sendRequestWithFds(objectId, wl_shm::Request::CREATE_POOL,  fds, id, size);
    return s;
}
