#pragma once

#include <string>
#include <cstdint>
#include "../include/DataEntry.h"
#include "../include/SerializeHelper.h"
#include "../include/PortabilityHelper.h"
#include "../include/DebugTracer.h"
namespace dlnk
{
template<typename T>
DataEntry<T>::DataEntry(std::string _name, DataDirection _direction)
: name{_name}
, direction{_direction}
, byteOffset{0}
{
    SCOPE_TRACE("DataEntry<T>::DataEntry");
    SetDataType<T>();
}

template<typename T>
void DataEntry<T>::WriteToBuffer(ByteVector& byteBuffer)
{
    SCOPE_TRACE("DataEntry<T>::WriteToBuffer");
    serial::write_string(byteBuffer, name);

    Byte DataDescription = static_cast<Byte>(direction) << 4 |
                           static_cast<Byte>(dataType)       ;
    serial::write_u8_be(byteBuffer, DataDescription);
}

template<typename T>
void DataEntry<T>::ReadFromBuffer(const Byte*& cur)
{
    SCOPE_TRACE("DataEntry<T>::ReadFromBuffer");
    name = serial::read_string(cur);
    Byte DataDescription = serial::read_u8_be(cur);
    direction = DataDirection(static_cast<uint8_t>(DataDescription >> 4));
    dataType = DataType((DataDescription & 15));
}

template<typename T>
void DataEntry<T>::Print(uint8_t tabs) const
{
    SCOPE_TRACE("DataEntry<T>::Print");
    std::string tabString = std::string(2*tabs, ' ');
    // Name
    print_t(tabString + "\033[92mData Entry: \"" + name + "\""  + "\033[32m\n");
    // Data Type
    print_t(tabString + "  data type: ");
    PrintDataType(dataType);
    print_t("\n");
    //Data Direction
    print_t(tabString + "  data direction: ");
    switch (direction)
    {
    case DataDirection::DESIREDSTATE:
        print_t("desired state"); break;
    case DataDirection::FEEDBACK:
        print_t("sensor feedback"); break;
    case DataDirection::INIT:
        print_t("hardware initalization"); break;
    default:
        print_t("unknown"); break;
    }
    print_t("\n");
}

template<typename T>
template<typename G>
DataEntry<T>& DataEntry<T>::SetDataType()
{
    if      constexpr (std::is_same_v<G, bool       >) dataType = DataType::BOOL   ;
    else if constexpr (std::is_same_v<G, uint8_t    >) dataType = DataType::UINT8  ;
    else if constexpr (std::is_same_v<G, uint16_t   >) dataType = DataType::UINT16 ;
    else if constexpr (std::is_same_v<G, uint32_t   >) dataType = DataType::UINT32 ;
    else if constexpr (std::is_same_v<G, uint64_t   >) dataType = DataType::UINT64 ;
    else if constexpr (std::is_same_v<G, int8_t     >) dataType = DataType::INT8   ;
    else if constexpr (std::is_same_v<G, int16_t    >) dataType = DataType::INT16  ;
    else if constexpr (std::is_same_v<G, int32_t    >) dataType = DataType::INT32  ;
    else if constexpr (std::is_same_v<G, int64_t    >) dataType = DataType::INT64  ;
    else if constexpr (std::is_same_v<G, float      >) dataType = DataType::FLOAT  ;
    else if constexpr (std::is_same_v<G, double     >) dataType = DataType::DOUBLE ;
    else if constexpr (std::is_same_v<G, std::string>) dataType = DataType::STRING ;
    return *this;
}

}; // namespace: dlnk