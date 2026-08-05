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

    std::shared_ptr<std::any>& NewCreateInfo(std::string deviceName);

    void ConstructAllControlObjects();

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<std::any>>> createInfoVectors;
    std::vector<std::shared_ptr<std::any>> controlObjectVector;

    std::unordered_map<std::string, std::function<void(std::shared_ptr<std::any>&)>> createInfoOperate;
    std::unordered_map<std::string, std::function<void(std::shared_ptr<std::any>&, std::vector<std::shared_ptr<std::any>>&)>> ControlObjOperate;
};

}; // namespace: dlnk