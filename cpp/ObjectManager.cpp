#include "../include/ObjectManager.h"
#include <string>
#include <memory>
#include <any>

namespace dlnk
{

std::unique_ptr<std::any>& ObjectManager::NewCreateInfo(std::string deviceName)
{
    std::unique_ptr<std::any>& ref = createInfoVectors[deviceName].emplace_back();
    createInfoOperate[deviceName](ref);
    return ref;
}

void ObjectManager::ConstructAllControlObjects()
{
    for (auto& [key, value] : createInfoVectors) {
        std::unique_ptr<std::any> ref = controlObjectVector.emplace_back();
        ControlObjOperate[key](ref, value);
    }
}

};