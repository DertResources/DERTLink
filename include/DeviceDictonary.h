#pragma once

#include <deque>
#include "DataEntry.h"
#include <vector>
#include <cstdint>
#include <string>
#include <variant>
#include "../include/Device.h"
#include "../include/SerializeHelper.h"
#include "../include/DynamicBuffer.h"
#include <type_traits>
#include <utility>
#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <initializer_list>

namespace dlnk
{
typedef DataEntryVariant* DataEntryPtr;
typedef std::vector<DataEntryPtr> ShortDEVector;
struct ShortDev {
    std::string DeviceName;
    Device* DevicePtr;
    ShortDEVector DeviceEntryPtrs;
    ShortDEVector& GetShortDEVector() { return DeviceEntryPtrs; }
};
typedef std::vector<ShortDev> ShortDevVector;

class DeviceDictonary
{
public:
    template <typename CtrObjType, typename CreateInfoType, typename... Args>
    inline Device& AddDevice(Args... _args)
    {
        static_assert(std::is_constructible_v<Device, Args...>,
            "AddDevice: cannot construct Device from the provided arguments. "
            "Check that the argument types match a Device constructor.");

        Devices.emplace_back(std::forward<Args>(_args)...);
        Devices.back().SetDeviceDictonaryPtr(*this);

        //auto w = 
        //    [](std::any& createInfoObj, std::function<void(auto&)> func)
        //    {
        //        CreateInfoType& _a1 = std::any_cast<CreateInfoType&>(createInfoObj);
        //        func(_a1);
        //    });
        if constexpr (!std::is_same<CreateInfoType, void>() && !std::is_same<CtrObjType, void>())
        {
            createInfoOperate[Devices.back().GetName()] = [](std::any& anyOpp) {
                anyOpp = std::make_any<CreateInfoType>();
            };

            ControlObjOperate[Devices.back().GetName()] = [](std::any& anyOpp, std::vector<std::any> createInfoVector) {
                std::vector<CreateInfoType> civ = {};
                for (auto& ci : createInfoVector) {
                    civ.push_back(std::any_cast<CreateInfoType>(ci));
                }
                anyOpp = std::make_any<CtrObjType>(civ);
            };
        }
        
        return Devices.back();
    }

    void Print(uint8_t tabs = 0);

    void WriteToBuffer(ByteVector& _byteVector);

    void ReadFromBuffer(ByteVector& _byteVector);

    inline std::deque<Device>& GetDeviceVector() { return this->Devices; }

    
    inline std::unordered_map<std::string, std::function<void(std::any&)>> GetCreateInfoOperateMap()
    {
        return createInfoOperate;
    };
    
    inline std::unordered_map<std::string, std::function<void(std::any&, std::vector<std::any>&)>> GetControlObjectOperateMap()
    {
        return ControlObjOperate;
    };

    static bool CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2);

    static void FillShortDevVector(DeviceDictonary& DD, ShortDevVector& sdv);

    static void SortShortDevVector(ShortDevVector& sdv);

    static bool FindShortDevice(ShortDevVector& sdv, ShortDev& sdr, std::string targetName);

    static bool FindShortDataEntry(ShortDev& sdr, DataEntryPtr& dep, std::string targetName);

    static int32_t FindDataEntryIndex(ShortDev& sdr, std::string targetName);

private:
    std::unordered_map<std::string, std::function<void(std::any&)>> createInfoOperate;
    std::unordered_map<std::string, std::function<void(std::any&, std::vector<std::any>&)>> ControlObjOperate;
    std::deque<Device> Devices;
};

}; // namespace: dlnk