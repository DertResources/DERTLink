#pragma once
#include <cstdint>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <variant>
#include "../include/SerializeHelper.h"
#include "../include/PortabilityHelper.h"

namespace dlnk
{

typedef enum : Byte {
    DESIREDSTATE  , // 0  Coprocessor --> System-Core | Hardware Commands
    FEEDBACK      , // 1  Coprocessor <-- System-Core | Sensor Feedback
    INIT          , // 2  Coprocessor --> System-Core | Required Hardware Initalization Parameters
} DataDirection;

enum class DataType : Byte {
    BOOL   , // 0
    UINT8  , // 1
    UINT16 , // 2
    UINT32 , // 3 
    UINT64 , // 4 
    INT8   , // 5 
    INT16  , // 6  
    INT32  , // 7  
    INT64  , // 8
    FLOAT  , // 9
    DOUBLE , // 10
    STRING , // 11
};  
static void PrintDataType(DataType dt)
{
    switch (dt)
    {
    case DataType::BOOL:
        print_t("boolean"); break;
    case DataType::UINT8:
        print_t("uint8_t"); break;
    case DataType::UINT16:
        print_t("uint16_t"); break;
    case DataType::UINT32:
        print_t("uint32_t"); break;
    case DataType::UINT64:
        print_t("uint64_t"); break;
    case DataType::INT8:
        print_t("int8_t"); break;
    case DataType::INT16:
        print_t("int16_t"); break;
    case DataType::INT32:
        print_t("int32_t"); break;
    case DataType::INT64:
        print_t("int64_t"); break;
    case DataType::FLOAT:
        print_t("float"); break;
    case DataType::DOUBLE:
        print_t("double"); break;
    case DataType::STRING:
        print_t("string"); break;
    default:
        print_t("unknown"); break;
    }
}

template<typename T>
class DataEntry {
private:
    std::string name;
    DataDirection direction;
    DataType dataType;
    uint16_t byteOffset;
public:
    
    DataEntry(std::string _name, DataDirection _direction);
    inline DataEntry& SetName          (std::string   __name      ) {name       = __name       ; return *this;}
    inline DataEntry& SetDataDirection (DataDirection __direction ) {direction  = __direction  ; return *this;}
    inline DataEntry& SetByteOffset    (uint16_t      __byteOffset) {byteOffset = __byteOffset ; return *this;}

    inline std::string GetName () { return this->name; }
    inline DataDirection GetDataDirection() { return this->direction; }
    inline DataType GetDataType() { return this->dataType; }


    void WriteToBuffer(ByteVector& byteBuffer);
    void ReadFromBuffer(const Byte*& cur);
    void Print(uint8_t tabs = 0) const;
    template<typename G>
    constexpr DataEntry& SetDataType() {
             if constexpr (std::is_same_v<G, bool       >) dataType = DataType::BOOL   ;
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
};

typedef std::variant<DataEntry<bool>, 
                     DataEntry<uint8_t>,
                     DataEntry<uint16_t>,
                     DataEntry<uint32_t>,
                     DataEntry<uint64_t>,
                     DataEntry<int8_t>,
                     DataEntry<int16_t>,
                     DataEntry<int32_t>,
                     DataEntry<int64_t>,
                     DataEntry<float>,
                     DataEntry<double>,
                     DataEntry<std::string>
                     > DataEntryVariant;


}; // namespace: dlnk

#include "../cpp/DataEntry_imp.h"
