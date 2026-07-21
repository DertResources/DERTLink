#pragma once

#include <deque>
#include <functional>
#include <string>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <vector>
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

using InitFuncType = std::function<void(std::any& obj, ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db)>;

class InitalizationGroup
{
public:
    InitalizationGroup(std::string _name) 
    : GroupName{_name}
    {}

    template<typename T>
    inline InitalizationGroup& AddDataEntry(std::string _name, DataDirection _direction) { GroupDataEntries.push_back(std::move(DataEntry<T>(_name, _direction))); return *this; }

    InitalizationGroup& SetInitalizationCmd(InitFuncType func)
    {
        InitalizationCmd = std::move(func);
        return *this;
    }

    inline std::deque<DataEntryVariant>& getDataEntryVector() { return GroupDataEntries; }
    
    void Print(uint8_t tabs = 0);

    inline void SetDevicePtr(Device& ref) { devicePtr = &ref; }

    inline Device& ExitInitalizationGroup() { return *devicePtr; }
    inline void RunInitCmd(std::any& obj, ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db) { InitalizationCmd(obj, sd, dbf, dbd, db); }
    inline std::string GetName() { return GroupName; }
private:
    std::deque<DataEntryVariant> GroupDataEntries;
    Device* devicePtr = nullptr;
    InitFuncType InitalizationCmd;
    std::string GroupName;
};

}; // namespace: dlnk