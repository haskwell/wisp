#include <cstdint>
#include <vector>
class IDManager{
    uint32_t currentId{2};
    std::vector<uint32_t> freeIds;
    public:
    uint32_t allocateNewId(){
        if(!freeIds.empty()){
            uint32_t id = freeIds.back();
            freeIds.pop_back();
            return id;
        }
        return currentId++;
    }
    void removeId(uint32_t id){
        freeIds.push_back(id);
    }
};