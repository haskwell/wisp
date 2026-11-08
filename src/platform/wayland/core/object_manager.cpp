#include "object_manager.hpp"
#include "proxy.hpp"

uint32_t ObjectManager::registerProxy(Proxy* p){
    p->objectId = currentId;
    waylandObjects[currentId] = p;
    return currentId++;
}

void ObjectManager::registerAt(uint32_t id, Proxy* p){
    p->objectId = id;
    waylandObjects[id] = p;
}

Proxy* ObjectManager::lookupProxy(uint32_t objectId){
    return waylandObjects[objectId];
}

void ObjectManager::deleteProxy(uint32_t objectId){
    waylandObjects[objectId] = nullptr;
}