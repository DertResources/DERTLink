#pragma once

#include <utility>
#include <deque>
#include "DataEntry.h"
#include <vector>
#include <cstdint>
#include <string>
#include "../include/Device.h"
#include "../include/SerializeHelper.h"

namespace dlnk
{
using DataEntryPtr = DataEntryVariant*;
using ShortDEVector = std::vector<DataEntryPtr>;
struct ShortDev {
    std::string DeviceName;
    Device* DevicePtr;
    ShortDEVector DeviceEntryPtrs;
    ShortDEVector& GetShortDEVector() { return DeviceEntryPtrs; }
};
using ShortDevVector = std::vector<ShortDev>;

class DeviceDictonary
{
public:
    template <typename... Args>
    Device& AddDevice(Args... _args);

    void Print(uint8_t tabs = 0);

    void WriteToBuffer(ByteVector& _byteVector);

    void ReadFromBuffer(ByteVector& _byteVector);

    inline std::deque<Device>& GetDeviceVector() {return this->Devices;}

    static bool CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2);

    static void FillShortDevVector(DeviceDictonary& DD, ShortDevVector& sdv);

    static void SortShortDevVector(ShortDevVector& sdv);

    static bool FindShortDevice(ShortDevVector& sdv, ShortDev& sdr, std::string targetName);

    static bool FindShortDataEntry(ShortDev& sdr, DataEntryPtr& dep, std::string targetName);

    static int32_t FindDataEntryIndex(ShortDev& sdr, std::string targetName);

private:
    
    std::deque<Device> Devices;
};

}; // namespace: dlnk