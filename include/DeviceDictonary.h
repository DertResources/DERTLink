#pragma once
#include "../include/Device.h"

#include <utility>
#include <deque>
#include <algorithm>
#include <optional>

#include "../include/SerializeHelper.h"

namespace dlib::communication
{

class DeviceDictonary
{
public:
    template <typename... Args>
    inline Device& AddDevice(Args... _args)
    {
        static_assert(std::is_constructible_v<Device, Args...>,
            "AddDevice: cannot construct Device from the provided arguments. "
            "Check that the argument types match a Device constructor.");

        Devices.emplace_back(std::forward<Args>(_args)...);
        Devices.back().SetDeviceDictonaryPtr(*this);
        return Devices.back();
    }

    inline void Print(uint8_t tabs = 0)
    {
        for (Device& _device : Devices)
        {
            _device.Print(tabs+1);
        }
    }

    inline void WriteToBuffer(ByteVector& _byteVector)
    {
        
        for (Device& _device : Devices)
        {
            _device.WriteToBuffer(_byteVector);
        }  
    }

    inline void ReadFromBuffer(ByteVector& _byteVector)
    {
        const Byte* cur = &_byteVector[0];

        while (cur < &_byteVector.back())
        {
            this->AddDevice("").ReadFromBuffer(cur);
        }
    }

    inline std::deque<Device>& GetDeviceVector() {return this->Devices;}

    static inline bool CompareDictonaries(DeviceDictonary& dd1, DeviceDictonary& dd2)
    {
        //quickly return false if the number of devices are different
        if(dd1.GetDeviceVector().size() != dd2.GetDeviceVector().size()) return false;
        // loop for every device in dd1

        for (Device& dd1_device : dd1.GetDeviceVector())
        {
            // find two matching devices based on name
            std::optional<std::reference_wrapper<Device>> dd2_device;

            std::string dd1_device_name = dd1_device.GetName();
            auto itt = std::find_if(dd2.GetDeviceVector().begin(), dd2.GetDeviceVector().end(),
                [&](Device& device2){
                    return device2.GetName() == dd1_device_name;
                });
            if(itt == dd2.GetDeviceVector().end())
                return false; // return false if device name in dd2 not found
            else
                dd2_device = *itt;

            // allocate two vectors of references to device entries
            std::vector<DataEntryVariant*> dd1_device_entries;
            std::for_each(dd1_device.GetInitaliztionGroupVector().begin(),
                          dd1_device.GetInitaliztionGroupVector().end(),
                          [&](InitalizationGroup& igv){
                            std::for_each(igv.getDataEntryVector().begin(), igv.getDataEntryVector().end(),
                            [&](DataEntryVariant& dev){
                                dd1_device_entries.push_back(&dev);
                            });
                          });

            std::vector<DataEntryVariant*> dd2_device_entries;
            std::for_each(dd2_device->get().GetInitaliztionGroupVector().begin(),
                          dd2_device->get().GetInitaliztionGroupVector().end(),
                          [&](InitalizationGroup& igv) {
                              std::for_each(igv.getDataEntryVector().begin(), igv.getDataEntryVector().end(),
                                  [&](DataEntryVariant& dev) {
                                      dd2_device_entries.push_back(&dev);
                                  });
                          });
            // sort both vectors based on entry name
            std::sort(dd1_device_entries.begin(), dd1_device_entries.end(),
            [](DataEntryVariant* deva, DataEntryVariant* devb){
                return std::visit([&](auto& dea) -> bool{
                    return std::visit([&](auto& deb) -> bool{
                        return dea.GetName() < deb.GetName();
                    }, *devb);
                }, *deva);
            });

            std::sort(dd2_device_entries.begin(), dd2_device_entries.end(),
                [](DataEntryVariant* deva, DataEntryVariant* devb) {
                    return std::visit([&](auto& dea) -> bool {
                        return std::visit([&](auto& deb) -> bool {
                            return dea.GetName() < deb.GetName();
                            }, *devb);
                        }, *deva);
                });
            
            if (!std::equal(dd1_device_entries.begin(), dd1_device_entries.end(),
                dd2_device_entries.begin(), dd2_device_entries.end(),
                [](DataEntryVariant* deva, DataEntryVariant* devb) {
                    return std::visit([&](auto& dea) -> bool {
                        return std::visit([&](auto& deb) -> bool {
                            return (dea.GetDataType() == deb.GetDataType()) && 
                                   (dea.GetDataDirection() == deb.GetDataDirection());
                            }, *devb);
                        }, *deva);
                }))
                return false;
        }
        return true;
    }
private:
    std::deque<Device> Devices;
};

}; // namespace: dlib::communication