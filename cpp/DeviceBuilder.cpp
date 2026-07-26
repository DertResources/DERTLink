#include "../include/DeviceBuilder.h"
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <variant>
#include "../include/SerializeHelper.h"
#include <iostream>
#include "../include/DataEntry.h"



namespace dlnk
{

DeviceBuilder& DeviceBuilder::AssignData(std::string DataEntryName)
{
    entryManifests.emplace_back(DataEntryName, std::nullopt);
    return *this;
}

ManifestBuilder& DeviceBuilder::ExitDeviceBuilder()
{
    return *parentptr;
}

std::vector<EntryManifest>& DeviceBuilder::GetEntryManifests()
{
    return entryManifests;
}

std::string DeviceBuilder::GetDeviceName() const
{
    return deviceName;
}

void DeviceBuilder::WriteBuffer(ByteVector& BV)
{
    serial::write_u8_be(BV, static_cast<uint8_t>(entryManifests.size()));
    for (EntryManifest& em : entryManifests) {
        serial::write_string(BV, em.entryName);
        if (em.entryData.has_value()) {
            serial::write_u8_be(BV, static_cast<uint8_t>(em.entryType.value()));
            std::visit([&](auto& InitData) {
                serial::AutomaticWrite<decltype(InitData)>(BV, InitData);
            },
                em.entryData.value());
        } else {
            serial::write_u8_be(BV, 255);
        }
    }
}

void DeviceBuilder::ReadBuffer(const Byte*& cur)
{
    uint8_t numOfEntries = serial::read_u8_be(cur);
    for(size_t i = 0; i < numOfEntries; i++)
    {
        std::string entryName = serial::read_string(cur);
        uint8_t typeIndex = serial::read_u8_be(cur);
        if(typeIndex != 255)
        {
            switch (typeIndex) {
                case 0:  AssignData(entryName, serial::read_bool_be  (cur)); break;
                case 1:  AssignData(entryName, serial::read_u8_be    (cur)); break;
                case 2:  AssignData(entryName, serial::read_u16_be   (cur)); break;
                case 3:  AssignData(entryName, serial::read_u32_be   (cur)); break;
                case 4:  AssignData(entryName, serial::read_u64_be   (cur)); break;
                case 5:  AssignData(entryName, serial::read_i8_be    (cur)); break;
                case 6:  AssignData(entryName, serial::read_i16_be   (cur)); break;
                case 7:  AssignData(entryName, serial::read_i32_be   (cur)); break;
                case 8:  AssignData(entryName, serial::read_i64_be   (cur)); break;
                case 9:  AssignData(entryName, serial::read_float_be (cur)); break;
                case 10: AssignData(entryName, serial::read_double_be(cur)); break;
                case 11: AssignData(entryName, serial::read_string   (cur)); break;
                default:
                    std::cout << "ManifestBuilder ReadBuffer function caught unknown type" << std::endl;
            }
        }
        else
            AssignData(entryName);
    }
}

void DeviceBuilder::Print(int tabs)
{
    std::string tabString = std::string(tabs * 2, ' ');
    std::cout << tabString << "Device Name: " << deviceName << std::endl;
    for (EntryManifest& em : entryManifests) {
        std::cout << tabString << tabString << "Entry Name: " << em.entryName << std::endl
                  << tabString << tabString << "Type: ";
        if (em.entryType.has_value())
            PrintDataType(em.entryType.value());
        else
            std::cout << "[No Value]";
        std::cout << std::endl;
    }
}

// int literals -> int32_t
DeviceBuilder& DeviceBuilder::AssignData(std::string DataEntryName, int value)
{
    entryManifests.emplace_back(DataEntryName, static_cast<int32_t>(value));
    return *this;
}

// bool explicit overload (prevents bool -> int ambiguity)
DeviceBuilder& DeviceBuilder::AssignData(std::string DataEntryName, bool value)
{
    entryManifests.emplace_back(DataEntryName, value);
    return *this;
}

}; // namespace: dlnk
