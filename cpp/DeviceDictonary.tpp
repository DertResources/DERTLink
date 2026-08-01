#pragma once
#include "../include/DeviceDictonary.h"
#include "../include/Device.h"
#include <type_traits>
#include <utility>
#include <vector>
#include <any>
#include <memory>


namespace dlnk
{

template <typename CtrObjType, typename CreateInfoType, typename... Args>
inline Device& DeviceDictonary::AddDevice(Args... _args)
{
    static_assert(std::is_constructible_v<Device, Args...>,
        "AddDevice: cannot construct Device from the provided arguments. "
        "Check that the argument types match a Device constructor.");

    Devices.emplace_back(std::forward<Args>(_args)...);
    Devices.back().SetDeviceDictonaryPtr(*this);

    // auto w =
    //     [](std::any& createInfoObj, std::function<void(auto&)> func)
    //     {
    //         CreateInfoType& _a1 = std::any_cast<CreateInfoType&>(createInfoObj);
    //         func(_a1);
    //     });
    if constexpr (!std::is_same<CreateInfoType, void>() && !std::is_same<CtrObjType, void>()) {
        createInfoOperate[Devices.back().GetName()] = [](std::unique_ptr<std::any> anyOpp) {
            anyOpp = std::make_any<CreateInfoType>();
        };

        ControlObjOperate[Devices.back().GetName()] = [](std::unique_ptr<std::any> anyOpp, std::vector<std::unique_ptr<std::any>> createInfoVector) {
            std::vector<CreateInfoType> civ = { };
            for (std::unique_ptr<std::any>& ci : createInfoVector) {
                civ.push_back(std::any_cast<CreateInfoType>(*ci.get()));
            }
            anyOpp = std::make_unique<std::any>(std::make_any<CtrObjType>(civ));
        };
    }

    return Devices.back();
}

};