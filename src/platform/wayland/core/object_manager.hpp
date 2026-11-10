//id counter
//map from id to proxy
//allocate id and registry proxy, delete proxy, look up proxy
#pragma once
#include <unordered_map>
#include <cstdint>
#include <vector>

class Proxy;

class ObjectManager{
    uint32_t currentId = 2;
    std::unordered_map<uint32_t, Proxy*> waylandObjects;
    std::vector<uint32_t> freeIds;
    public:
    uint32_t registerProxy(Proxy* p);
    void registerAt(uint32_t id, Proxy* p);
    void unregisterProxy(uint32_t objectId, Proxy* p);
    void deleteProxy(uint32_t objectId);
    Proxy* lookupProxy(uint32_t objectId);
};