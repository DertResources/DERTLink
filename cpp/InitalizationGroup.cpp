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
#include "../include/DebugTracer.h"

namespace dlnk
{

void InitalizationGroup::Print(uint8_t tabs)
{
    SCOPE_TRACE("InitalizationGroup::Print");
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
    SCOPE_TRACE("InitalizationGroup::SetDevicePtr");
    devicePtr = &ref;
}

Device& InitalizationGroup::ExitInitalizationGroup()
{
    SCOPE_TRACE("InitalizationGroup::ExitInitalizationGroup");
    return *devicePtr;
}

void InitalizationGroup::RunInitCmd(std::shared_ptr<std::any>& obj,
                                    ShortDev& sd,
                                    DynamicBuffer& dbf,
                                    DynamicBuffer& dbd,
                                    DeviceBuilder& db)
{
    SCOPE_TRACE("InitalizationGroup::RunInitCmd");
    InitalizationCmd(obj, sd, dbf, dbd, db);
}

std::string InitalizationGroup::GetName()
{
    SCOPE_TRACE("InitalizationGroup::GetName");
    return GroupName;
}

InitalizationGroup& InitalizationGroup::SetInitalizationCmd(InitFuncType func)
{
    SCOPE_TRACE("InitalizationGroup::SetInitalizationCmd");
    InitalizationCmd = std::move(func);
    return *this;
}

std::deque<DataEntryVariant>& InitalizationGroup::getDataEntryVector()
{
    SCOPE_TRACE("InitalizationGroup::getDataEntryVector");
    return GroupDataEntries;
}

}; // namespace: dlnk