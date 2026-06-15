#pragma once
#include "../include/DataEntry.h"

#include <deque>
#include <functional>
#include <string>

namespace dlnk
{
class Device; // forward declaration

class InitalizationGroup
{
public:
    InitalizationGroup(std::string _name) 
    : GroupName{_name}
    {}

    template<typename T>
    inline InitalizationGroup& AddDataEntry(std::string _name, DataDirection _direction) { GroupDataEntries.push_back(std::move(DataEntry<T>(_name, _direction))); return *this; }

    InitalizationGroup& SetInitalizationCmd(std::function<void()> func) { InitalizationCmd = std::move(func); return *this; }

    inline std::deque<DataEntryVariant>& getDataEntryVector() { return GroupDataEntries; }
    
    void Print(uint8_t tabs = 0);

    inline void SetDevicePtr(Device& ref) { devicePtr = &ref; }

    inline Device& ExitInitalizationGroup() { return *devicePtr; }

private:
    std::deque<DataEntryVariant> GroupDataEntries;
    Device* devicePtr = nullptr;
    std::function<void()> InitalizationCmd;
    std::string GroupName;
};

}; // namespace: dlnk