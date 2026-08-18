#pragma once
#include "../include/InitalizationGroup.h"
#include "../include/DebugTracer.h"
#include "DERTLink/include/DebugTracer.h"

namespace dlnk 
{
    template<typename T>
    InitalizationGroup& InitalizationGroup::AddDataEntry(std::string _name, DataDirection _direction)
    {
        SCOPE_TRACE("InitalizationGroup::AddDataEntry");
        GroupDataEntries.push_back(std::move(DataEntry<T>(_name, _direction)));
        return *this;
    }

}; // namespace: dlnk