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

template <typename CtrObjType, typename CreateInfoType>
inline Device& DeviceDictonary::AddDevice(std::string deviceName)
{
    Devices.emplace_back(std::forward<std::string>(deviceName));
    Devices.back().SetDeviceDictonaryPtr(*this);

    if constexpr (!std::is_same<CreateInfoType, void>() && !std::is_same<CtrObjType, void>()) {
        createInfoOperate[Devices.back().GetName()] = [](std::shared_ptr<std::any>& anyOpp) {
            anyOpp = std::make_shared<std::any>(std::make_shared<CreateInfoType>());
        };

        ControlObjOperate[Devices.back().GetName()] = [](std::shared_ptr<std::any>& anyOpp, std::vector<std::shared_ptr<std::any>>& createInfoVector) {
            std::vector<CreateInfoType> civ = { };
            for (std::shared_ptr<std::any>& ci : createInfoVector) {
                civ.push_back(std::any_cast<CreateInfoType>(*ci.get()));
            }
            anyOpp = std::make_shared<std::any>(std::make_shared<CtrObjType>(civ));
        };
    }

    return Devices.back();
}

};