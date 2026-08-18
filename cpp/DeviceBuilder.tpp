#pragma once
#include "../include/DeviceBuilder.h"

namespace dlnk
{
    template<typename T, std::enable_if_t<
        is_variant_member_v<T, EntryDataVariant> &&
        !std::is_convertible_v<T, std::string>, int>>
    inline DeviceBuilder & DeviceBuilder::AssignData(std::string DataEntryName, T value)
    { 
        SCOPE_TRACE("DeviceBuilder::AssignData");
        entryManifests.emplace_back(DataEntryName, value);
        return *this;
    }

    // String and const char* 
    template<typename T, std::enable_if_t<
        std::is_convertible_v<T, std::string>, int>>
    inline DeviceBuilder & DeviceBuilder::AssignData(std::string DataEntryName, T value)
    {
        SCOPE_TRACE("DeviceBuilder::AssignData");
        entryManifests.emplace_back(DataEntryName, std::string(value));
        return *this;
    }
}