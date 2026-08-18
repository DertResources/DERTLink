#pragma once

#include <deque>
#include <functional>
#include <string>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <vector>
#include <memory>
#include <any>
#include "../include/DataEntry.h"
#include "../include/DynamicBuffer.h"
#include "../include/DeviceBuilder.h"

namespace dlnk
{
// forward declarations
class Device; 
class ManifestBuilder;
struct ShortDev;

using InitFuncType = std::function<void(std::shared_ptr<std::any>& obj, ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db)>;

class InitalizationGroup
{
public:
    InitalizationGroup(std::string _name) 
    : GroupName{_name}
    {}

    template<typename T>
    inline InitalizationGroup& AddDataEntry(std::string _name, DataDirection _direction);

    InitalizationGroup& SetInitalizationCmd(InitFuncType func);

    std::deque<DataEntryVariant>& getDataEntryVector();
    
    void Print(uint8_t tabs = 0);

    void SetDevicePtr(Device& ref);

    Device& ExitInitalizationGroup();

    void RunInitCmd(std::shared_ptr<std::any>& obj, ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db);

    std::string GetName();

private:
    std::deque<DataEntryVariant> GroupDataEntries;
    Device* devicePtr = nullptr;
    InitFuncType InitalizationCmd;
    std::string GroupName;
};

}; // namespace: dlnk

#include "../cpp/InitalizationGroup.tpp"