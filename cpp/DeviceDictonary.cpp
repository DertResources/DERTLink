#include "../include/DeviceDictonary.h"

namespace dlnk
{

bool DeviceDictonary::CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2)
{
    // Create Short Device Dictonaries
    ShortDevVector sdv1;
    ShortDevVector sdv2;
    // Fill Short Device Dictonaries from Device Dictonaries
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
        if(sd1.first != sd2.first
        || sd1.second.size() != sd2.second.size())
            return false;
        return std::equal(sd1.second.begin(), sd1.second.end(), sd2.second.begin(), sd2.second.end(),
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
        this->AddDevice("").ReadFromBuffer(cur);
    }
}

void DeviceDictonary::Print(uint8_t tabs)
{
    for (Device& _device : Devices)
        _device.Print(tabs + 1);
}

template <typename... Args>
Device& DeviceDictonary::AddDevice(Args... _args)
{
    static_assert(std::is_constructible_v<Device, Args...>,
        "AddDevice: cannot construct Device from the provided arguments. "
        "Check that the argument types match a Device constructor.");

    Devices.emplace_back(std::forward<Args>(_args)...);
    Devices.back().SetDeviceDictonaryPtr(*this);
    return Devices.back();
}

void DeviceDictonary::FillShortDevVector(DeviceDictonary& DD, ShortDevVector& sdv)
{
    std::for_each(DD.GetDeviceVector().begin(), DD.GetDeviceVector().end(),
        [&](Device& device) {
            ShortDev sd;
            sd.first = device.GetName();
            std::for_each(device.GetInitaliztionGroupVector().begin(), device.GetInitaliztionGroupVector().end(),
                [&](InitalizationGroup& ig) {
                    std::for_each(ig.getDataEntryVector().begin(), ig.getDataEntryVector().end(),
                        [&](DataEntryVariant& dev) {
                            sd.second.push_back(&dev);
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
            return sda.first < sdb.first;
        });

    // Sort Data Entries in Each Device
    std::for_each(sdv.begin(), sdv.end(),
    [&](ShortDev& sd) {
        std::sort(sd.second.begin(), sd.second.end(),
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
            return sd.first < target;
        });
    // exit if device not found
    if (devIt == sdv.end() || devIt->first != targetName)
        return false;
    else {
        sdr = *devIt;
        return true;
    }
}

bool DeviceDictonary::FindShortDataEntry(ShortDev& sdr, DataEntryPtr& der, std::string targetName)
{
    auto deMatchIt = std::lower_bound(sdr.second.begin(), sdr.second.end(), targetName,
        [](DataEntryVariant* dev, std::string target) -> bool {
            return std::visit([&](auto& de) {
                return de.GetName() < target;
            },
                *dev);
        });
    // exit if no entry found
    if (deMatchIt == sdr.second.end() || std::visit([&](auto& de) { return de.GetName(); }, **deMatchIt) != targetName)
        return false;
    else {
        der = *deMatchIt;
        return true;
    }
}

}; // namespace: dlnk