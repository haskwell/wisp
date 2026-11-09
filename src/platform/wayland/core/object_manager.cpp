#include "object_manager.hpp"
#include "proxy.hpp"

uint32_t ObjectManager::registerProxy(Proxy* p){
    if (!freeIds.empty()) {
        uint32_t id = freeIds.back();
        freeIds.pop_back();
        p->objectId = id;
        waylandObjects[id] = p;
        return id;
    }
    p->objectId = currentId;
    waylandObjects[currentId] = p;
    return currentId++;
}

void ObjectManager::registerAt(uint32_t id, Proxy* p){
    p->objectId = id;
    waylandObjects[id] = p;
}

void ObjectManager::unregisterProxy(uint32_t id){
    auto it = waylandObjects.find(id);
    if(it != waylandObjects.end()) it->second = nullptr;
}

void ObjectManager::deleteProxy(uint32_t id){
    waylandObjects.erase(id);
    freeIds.push_back(id);
}

Proxy* ObjectManager::lookupProxy(uint32_t id){
    auto it = waylandObjects.find(id);
    return it == waylandObjects.end() ? nullptr : it->second;
}