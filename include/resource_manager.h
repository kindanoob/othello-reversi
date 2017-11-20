#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <map>
#include <iostream>


template <typename Enum, typename Resource>
class ResourceManager {
public:
    std::map<Enum, Resource> resource_map_;
    Resource& GetResource(Enum name) {
        return resource_map_[name];
    };
protected:
    void AddResource(Enum name, const std::string& filename) {
        Resource res;
        if(!res.loadFromFile(filename)){
            std::cout << "Failed to load resource from file " << filename << std::endl;
            return;
        }
        resource_map_.emplace(name, res);
    };
private:
};


#endif // RESOURCE_MANAGER_H_
