#include "../include/DeviceDictonary.h"

#include <algorithm>
#include <variant>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <string>
#include <any>
#include "../include/SerializeHelper.h"
#include "../include/Device.h"
#include "../include/InitalizationGroup.h"
#include "../include/DataEntry.h"



namespace dlnk
{

bool DeviceDictonary::CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2)
{
    // Create Short Device Dictonaries
    ShortDevVector sdv1;
    ShortDevVector sdv2;
    // Fill Short Device Dictonaries from device Dictonaries
    FillShortDevVector(dd1, sdv1);
    FillShortDevVector(dd1, sdv2);
    // Sort Short Device Dictonaries
    SortShortDevVector(sdv1);
    SortShortDevVector(sdv2);
    //
    if (sdv1.size() != sdv2.size())
        return false;
    // all devices map
    return std::equal(sdv1.begin(), sdv1.end(), sdv2.begin(), sdv2.end(),
    [](ShortDev& sd1, ShortDev& sd2){
        // check device names are equal
        if (sd1.DeviceName != sd2.DeviceName
        || sd1.DeviceEntryPtrs.size() != sd2.DeviceEntryPtrs.size())
            return false;
        return std::equal(sd1.DeviceEntryPtrs.begin(), sd1.DeviceEntryPtrs.end(), sd2.DeviceEntryPtrs.begin(), sd2.DeviceEntryPtrs.end(),
        [](DataEntryPtr& dep1, DataEntryPtr& dep2) {
            return std::visit([&](auto& dea) -> bool {
                return std::visit([&](auto& deb) -> bool {
                    return (dea.GetDataType() == deb.GetDataType()) && (dea.GetDataDirection() == deb.GetDataDirection());
                }, *dep1);
            }, *dep1);
        });
    });
}

void DeviceDictonary::WriteToBuffer(ByteVector& _byteVector)
{
    for (Device& _device : Devices) {
        _device.WriteToBuffer(_byteVector);
    }
}

void DeviceDictonary::ReadFromBuffer(ByteVector& _byteVector)
{
    const Byte* cur = &_byteVector[0];

    while (cur < &_byteVector.back()) {
        this->AddDevice<void, void>("").ReadFromBuffer(cur);
    }
}

void DeviceDictonary::Print(uint8_t tabs)
{
    for (Device& _device : Devices)
        _device.Print(tabs + 1);
}

//template <typename CtrObjType, typename CreateInfoType, typename... Args>
//Device& DeviceDictonary::AddDevice(Args... _args)
//{
//    
//}

void DeviceDictonary::FillShortDevVector(DeviceDictonary& DD, ShortDevVector& sdv)
{
    std::for_each(DD.GetDeviceVector().begin(), DD.GetDeviceVector().end(),
        [&](Device& device) {
            ShortDev sd;
            sd.DeviceName = device.GetName();
            sd.DevicePtr = &device;
            std::for_each(device.GetInitaliztionGroupVector().begin(), device.GetInitaliztionGroupVector().end(),
                [&](InitalizationGroup& ig) {
                    std::for_each(ig.getDataEntryVector().begin(), ig.getDataEntryVector().end(),
                        [&](DataEntryVariant& dev) {
                            sd.DeviceEntryPtrs.push_back(&dev);
                        });
                });
            sdv.push_back(sd);
        });
}

void DeviceDictonary::SortShortDevVector(ShortDevVector& sdv)
{
    // Sort Device Names
    std::sort(sdv.begin(), sdv.end(),
        [](ShortDev& sda, ShortDev& sdb) {
            return sda.DeviceName < sdb.DeviceName;
        });

    // Sort Data Entries in Each Device
    std::for_each(sdv.begin(), sdv.end(),
    [&](ShortDev& sd) {
        std::sort(sd.DeviceEntryPtrs.begin(), sd.DeviceEntryPtrs.end(),
        [&](DataEntryVariant* deva, DataEntryVariant* devb) {
            return std::visit([&](auto& dea) -> bool {
                return std::visit([&](auto& deb) -> bool {
                    return dea.GetName() < deb.GetName();
                }, *devb);
            }, *deva);
        });
    });
}

bool DeviceDictonary::FindShortDevice(ShortDevVector& sdv, ShortDev& sdr, std::string targetName)
{
    auto devIt = std::lower_bound(sdv.begin(), sdv.end(), targetName,
        [](ShortDev& sd, std::string target) -> bool {
            return sd.DeviceName < target;
        });
    // exit if device not found
    if (devIt == sdv.end() || devIt->DeviceName != targetName)
        return false;
    else {
        sdr = *devIt;
        return true;
    }
}

bool DeviceDictonary::FindShortDataEntry(ShortDev& sdr, DataEntryPtr& der, std::string targetName)
{
    int32_t idx = DeviceDictonary::FindDataEntryIndex(sdr, targetName);
    if (idx != -1)
    {
        der = sdr.DeviceEntryPtrs[idx];
        return true;
    }
    else
        return false;
}

int32_t DeviceDictonary::FindDataEntryIndex(ShortDev& sdr, std::string targetName)
{
    auto deMatchIt = std::lower_bound(sdr.DeviceEntryPtrs.begin(), sdr.DeviceEntryPtrs.end(), targetName,
        [](DataEntryVariant* dev, std::string target) -> bool {
            return std::visit([&](auto& de) {
                return de.GetName() < target;
            }, *dev);
        });

    if (deMatchIt == sdr.DeviceEntryPtrs.end() || std::visit([&](auto& de) { return de.GetName(); }, **deMatchIt) != targetName)
        return -1;
    else {
        return static_cast<int32_t>(deMatchIt - sdr.DeviceEntryPtrs.begin());
    }
}

}; // namespace: dlnk