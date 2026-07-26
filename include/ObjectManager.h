#pragma once
#include "../include/DeviceDictonary.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include <vector>

namespace dlnk
{

class ObjectManager
{
public:
    ObjectManager(DeviceDictonary dd)
    : createInfoOperate{dd.GetCreateInfoOperateMap()}
    , ControlObjOperate{dd.GetControlObjectOperateMap()}
    {}

    std::any& NewCreateInfo(std::string deviceName);

    void ConstructAllControlObjects();

private:
    std::unordered_map<std::string, std::vector<std::any>> createInfoVectors;
    std::vector<std::any> controlObjectVector;

    std::unordered_map<std::string, std::function<void(std::any&)>> createInfoOperate;
    std::unordered_map<std::string, std::function<void(std::any&, std::vector<std::any>&)>> ControlObjOperate;
};

}; // namespace: dlnk