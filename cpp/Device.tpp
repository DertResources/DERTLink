#pragma once
#include "../include/Device.h"
#include "../include/InitalizationGroup.h"
#include <type_traits>
#include <utility>
#include "../include/DebugTracer.h"

namespace dlnk
{

template <typename... Args>
InitalizationGroup& Device::AddIntializationGroup(Args... _args)
{
    SCOPE_TRACE("Device::AddIntializationGroup");
    static_assert(std::is_constructible_v<InitalizationGroup, Args...>,
        "AddIntializationGroup: cannot construct Device from the provided arguments. "
        "Check that the argument types match a Device constructor.");

    initalizationGroupVector.emplace_back(std::forward<Args>(_args)...);
    initalizationGroupVector.back().SetDevicePtr(*this);
    return initalizationGroupVector.back();
}

}; // namespace: dlnk
