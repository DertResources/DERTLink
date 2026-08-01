#pragma once

#include <deque>
#include "DataEntry.h"
#include <vector>
#include <cstdint>
#include <string>
#include "../include/Device.h"
#include "../include/SerializeHelper.h"
#include <any>
#include <functional>
#include <unordered_map>

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
    inline Device& AddDevice(Args... _args);

    void Print(uint8_t tabs = 0);

    void WriteToBuffer(ByteVector& _byteVector);

    void ReadFromBuffer(const Byte*& cur);

    std::deque<Device>& GetDeviceVector();

    std::unordered_map<std::string, std::function<void(std::unique_ptr<std::any>)>> GetCreateInfoOperateMap();
    
    std::unordered_map<std::string, std::function<void(std::unique_ptr<std::any>, std::vector<std::unique_ptr<std::any>>)>> GetControlObjectOperateMap();

    static bool CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2);

    static void FillShortDevVector(DeviceDictonary& DD, ShortDevVector& sdv);

    static void SortShortDevVector(ShortDevVector& sdv);

    static bool FindShortDevice(ShortDevVector& sdv, ShortDev& sdr, std::string targetName);

    static bool FindShortDataEntry(ShortDev& sdr, DataEntryPtr& dep, std::string targetName);

    static int32_t FindDataEntryIndex(ShortDev& sdr, std::string targetName);

private:
    std::unordered_map<std::string, std::function<void(std::unique_ptr<std::any>)>> createInfoOperate;
    std::unordered_map<std::string, std::function<void(std::unique_ptr<std::any>, std::vector<std::unique_ptr<std::any>>)>> ControlObjOperate;
    std::deque<Device> Devices;
};

}; // namespace: dlnk

#include "../cpp/DeviceDictonary.tpp"
