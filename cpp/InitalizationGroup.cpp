#include "../include/InitalizationGroup.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>
#include "../include/DataEntry.h"
#include <any>
#include "../include/DynamicBuffer.h"
#include "../include/DeviceBuilder.h"
#include <utility>
#include <deque>

namespace dlnk
{

void InitalizationGroup::Print(uint8_t tabs)
{
    std::string tabString = std::string(tabs * 2, ' ');
    std::cout << tabString << "\033[96m" << "Initalization Group: \"" << GroupName << "\"" << std::endl;
    std::cout << tabString << "{" << "\033[m" << std::endl;
    for (DataEntryVariant& de : GroupDataEntries)
    {
        std::visit(
        [=](auto& a) {
            a.Print(tabs + 1);
        }, de);
    }
    std::cout << tabString << "\033[96m" << "}" << "\033[m" << std::endl;
}

void InitalizationGroup::SetDevicePtr(Device& ref)
{
    devicePtr = &ref;
}

Device& InitalizationGroup::ExitInitalizationGroup()
{
    return *devicePtr;
}

void InitalizationGroup::RunInitCmd(std::any& obj,
                                    ShortDev& sd,
                                    DynamicBuffer& dbf,
                                    DynamicBuffer& dbd,
                                    DeviceBuilder& db)
{
    InitalizationCmd(obj, sd, dbf, dbd, db);
}

std::string InitalizationGroup::GetName()
{
    return GroupName;
}

InitalizationGroup& InitalizationGroup::SetInitalizationCmd(InitFuncType func)
{
    InitalizationCmd = std::move(func);
    return *this;
}

std::deque<DataEntryVariant>& InitalizationGroup::getDataEntryVector()
{
    return GroupDataEntries;
}

}; // namespace: dlnk