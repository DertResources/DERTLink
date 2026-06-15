#pragma once
#include "../include/Device.h"

#include <utility>
#include <deque>
#include <algorithm>
#include <optional>

#include "../include/SerializeHelper.h"

namespace dlnk
{
using DataEntryPtr = DataEntryVariant*;
using ShortDEVector = std::vector<DataEntryPtr>;
using ShortDev = std::pair<std::string, ShortDEVector>;
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

    static bool FindShortDataEntry(ShortDev& sdr, DataEntryPtr& der, std::string targetName);

private:
    
    std::deque<Device> Devices;
};

}; // namespace: dlnk