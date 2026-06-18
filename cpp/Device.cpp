#include "../include/Device.h"

#include <string>
#include <cstdint>
#include <variant>
#include <numeric>
#include "../include/SerializeHelper.h"
#include "../include/InitalizationGroup.h"
#include "../include/DataEntry.h"
#include "../include/PortabilityHelper.h"

namespace dlnk
{

Device::Device(std::string _devicename)
: deviceName{ _devicename }
, initalizationGroupVector{}
{}

void Device::WriteToBuffer(ByteVector& byteBuffer)
{
    size_t totalEntries = std::accumulate(
        initalizationGroupVector.begin(),
        initalizationGroupVector.end(),
        size_t{ 0 },
        [](size_t sum, InitalizationGroup& group) {
            return sum + group.getDataEntryVector().size();
        }
    );
    serial::write_u16_be(byteBuffer, static_cast<uint16_t>(totalEntries));
    serial::write_string(byteBuffer, deviceName);
    for (InitalizationGroup& initGroupEntry : initalizationGroupVector)
    {
        for(DataEntryVariant& entry : initGroupEntry.getDataEntryVector())
        {
            std::visit([&](auto& a){a.WriteToBuffer(byteBuffer); }, entry);
        }

    }   
}

void Device::ReadFromBuffer(const Byte*& cur)
{
    size_t DataEntryCount = serial::read_u16_be(cur);
    deviceName = serial::read_string(cur);
    initalizationGroupVector.clear();
    this->AddIntializationGroup("Flattened Data Entry List");
    for (size_t pos = 0; pos < DataEntryCount; pos++)
    {
        this->initalizationGroupVector.back().AddDataEntry<bool>("", DataDirection::INIT);

        std::visit([&](auto& ref){
            ref.ReadFromBuffer(cur);
        }
        , this->initalizationGroupVector.back().getDataEntryVector().back()
        );
    }
}

void Device::Print(uint8_t tabs)
{
    std::string tabString = std::string(tabs*2, ' ');
    print_t( tabString + "\033[91mDevice Name: \"" + deviceName + "\"\n");
    print_t( tabString + "[\033[m\n");
    for (auto entry : initalizationGroupVector) {
        entry.Print(tabs+1);
    }
    print_t(tabString + "\033[91m]\033[m\n");
}

}; // namespace dlnk