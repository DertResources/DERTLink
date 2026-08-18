#include "../include/ObjectManager.h"
#include <string>
#include <memory>
#include <any>
#include "../include/DebugTracer.h"

namespace dlnk
{

std::shared_ptr<std::any>& ObjectManager::NewCreateInfo(std::string deviceName)
{
    SCOPE_TRACE("ObjectManager::NewCreateInfo");
    std::shared_ptr<std::any>& ref = createInfoVectors[deviceName].emplace_back();
    createInfoOperate[deviceName](ref);
    return ref;
}

void ObjectManager::ConstructAllControlObjects()
{
    SCOPE_TRACE("ObjectManager::ConstructAllControlObjects");
    for (auto& [key, value] : createInfoVectors) {
        std::shared_ptr<std::any>& ref = controlObjectVector.emplace_back();
        ControlObjOperate[key](ref, value);
    }
}

void ObjectManager::SetupMaps(DeviceDictonary dd)
{
    SCOPE_TRACE("ObjectManager::SetupMaps");
    createInfoOperate = dd.GetCreateInfoOperateMap();
    ControlObjOperate = dd.GetControlObjectOperateMap();
}

};