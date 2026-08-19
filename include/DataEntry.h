#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include <type_traits>
#include "../include/SerializeHelper.h"

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
static std::string PrintDataType(DataType dt)
{
    switch (dt) {
    case DataType::BOOL:
        return "boolean";
    case DataType::UINT8:
        return "uint8_t";
    case DataType::UINT16:
        return "uint16_t";
    case DataType::UINT32:
        return "uint32_t";
    case DataType::UINT64:
        return "uint64_t";
    case DataType::INT8:
        return "int8_t";
    case DataType::INT16:
        return "int16_t";
    case DataType::INT32:
        return "int32_t";
    case DataType::INT64:
        return "int64_t";
    case DataType::FLOAT:
        return "float";
    case DataType::DOUBLE:
        return "double";
    case DataType::STRING:
        return "string";
    default:
        return "unknown data type";
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

    std::string Print(uint8_t tabs = 0) const;

    template<typename G>
    DataEntry& SetDataType();
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

#include "../cpp/DataEntry.tpp"
