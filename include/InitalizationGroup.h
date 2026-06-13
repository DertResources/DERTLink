#pragma once
#include "../include/DataEntry.h"

#include <deque>
#include <functional>
#include <string>

namespace dlib::communication
{
class Device; // forward declaration

class InitalizationGroup
{
private:
    std::deque<DataEntryVariant> GroupDataEntries;
    Device* devicePtr = nullptr;
    std::function<void()> InitalizationCmd;
    std::string GroupName;
public:
    InitalizationGroup(std::string _name) 
    : GroupName{_name}
    {}

    template<typename T>
    inline InitalizationGroup& AddDataEntry(std::string _name, DataDirection _direction)
    {
        GroupDataEntries.push_back(std::move(DataEntry<T>(_name, _direction)));
        return *this;
    }

    InitalizationGroup& SetInitalizationCmd(std::function<void()> func)
    {
        InitalizationCmd = std::move(func);
        return *this;
    }

    std::deque<DataEntryVariant>& getDataEntryVector()
    {
        return GroupDataEntries;
    }
    
    void Print(uint8_t tabs = 0)
    {
        std::string tabString = std::string(tabs * 2, ' ');
        std::cout << tabString << "\033[96m" << "Initalization Group: \"" << GroupName << "\"" << std::endl;
        std::cout << tabString << "{" << "\033[m" << std::endl;
        for (DataEntryVariant& de : GroupDataEntries)
        {
            std::visit(
                [=](auto& a){
                    a.Print(tabs + 1);
                }, de
            );
        }
        std::cout << tabString << "\033[96m" << "}" << "\033[m" << std::endl;
    }

    void SetDevicePtr(Device& ref)
    {
        devicePtr = &ref;
    }


    Device& ExitInitalizationGroup()
    {
        return *devicePtr;
    }
};

}; // namespace: dlib::communication