#include "../include/ObjectManager.h"
#include <string>
#include <any>

namespace dlnk
{

std::any& ObjectManager::NewCreateInfo(std::string deviceName)
{
    std::any& ref = createInfoVectors[deviceName].emplace_back();
    createInfoOperate[deviceName](ref);
    return ref;
}

void ObjectManager::ConstructAllControlObjects()
{
    for (auto& [key, value] : createInfoVectors) {
        std::any& ref = controlObjectVector.emplace_back();
        ControlObjOperate[key](ref, value);
    }
}

};