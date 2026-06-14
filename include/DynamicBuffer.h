#pragma once

#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"
#include <deque>
#include <algorithm>
#include <numeric>

namespace dlnk
{

class DynamicBuffer
{
private:
    inline void AllocateBytes(size_t byteCount, size_t& byteOffset)
    {
        // idfk some computer bullshit
        stackSize += byteCount;
        size_t aligned = (buffer.size() + 7) & ~7;
        size_t padding = aligned - buffer.size();
        buffer.resize(buffer.size() + padding + byteCount);
        byteOffset = aligned;
    }
    // buffer is a deque so that pointers remain valid on resize
    std::vector<std::pair<size_t, DataType>> bufferDescription;
    std::vector<std::pair<size_t, std::string>> string_alloc_buffer;
    size_t stackSize = 0;
    std::deque<Byte> buffer;

public:
    std::deque<Byte>& GetBuffer() { return buffer; }
    // Allocators
    void AllocateDouble(double*& ptr);
    void AllocateFloat(float*& ptr);
    void AllocateUInt8(uint8_t*& ptr);
    void AllocateUInt16(uint16_t*& ptr);
    void AllocateUInt32(uint32_t *& ptr); 
    void AllocateUInt64(uint64_t *& ptr);
    void AllocateInt8(int8_t*& ptr);
    void AllocateInt16(int16_t*& ptr); 
    void AllocateInt32(int32_t*& ptr); 
    void AllocateInt64(int64_t*& ptr); 
    void AllocateBool(bool*& ptr); 
    void AllocateString(uint8_t*& ptr);
    
    // Special String Handling
    inline std::string ReadString(uint8_t* heapIdPtr) { return string_alloc_buffer[*heapIdPtr].second; }
    inline void WriteString(uint8_t* heapIdPtr, std::string str) { string_alloc_buffer[*heapIdPtr].second = str; }
    // populate Buffer
    void ExportBytes(ByteVector& expBuff);
    inline void ImportBytes(const Byte*& cur)
    {
        size_t stringCount = static_cast<size_t>(serial::read_u8_be(cur));

        std::for_each(bufferDescription.begin(), bufferDescription.end(),
        [&](std::pair<size_t, DataType>& entryDescription) {
            switch (entryDescription.second) {
            case DataType::DOUBLE:
                *reinterpret_cast<double*>(&buffer[entryDescription.first]) = serial::read_double_be(cur);
                break;
            case DataType::FLOAT:
                *reinterpret_cast<float*>(&buffer[entryDescription.first]) = serial::read_float_be(cur);
                break;
            case DataType::UINT8:
                *reinterpret_cast<uint8_t*>(&buffer[entryDescription.first]) = serial::read_u8_be(cur);
                break;
            case DataType::UINT16:
                *reinterpret_cast<uint16_t*>(&buffer[entryDescription.first]) = serial::read_u16_be(cur);
                break;
            case DataType::UINT32:
                *reinterpret_cast<uint32_t*>(&buffer[entryDescription.first]) = serial::read_u32_be(cur);
                break;
            case DataType::UINT64:
                *reinterpret_cast<uint64_t*>(&buffer[entryDescription.first]) = serial::read_u64_be(cur);
                break;
            case DataType::INT8:
                *reinterpret_cast<int8_t*>(&buffer[entryDescription.first]) = serial::read_i8_be(cur);
                break;
            case DataType::INT16:
                *reinterpret_cast<int16_t*>(&buffer[entryDescription.first]) = serial::read_i16_be(cur);
                break;
            case DataType::INT32:
                *reinterpret_cast<int32_t*>(&buffer[entryDescription.first]) = serial::read_i32_be(cur);
                break;
            case DataType::INT64:
                *reinterpret_cast<int64_t*>(&buffer[entryDescription.first]) = serial::read_i64_be(cur);
                break;
            case DataType::BOOL:
                *reinterpret_cast<bool*>(&buffer[entryDescription.first]) = serial::read_bool_be(cur);
                break;
            case DataType::STRING:
                *reinterpret_cast<uint8_t*>(&buffer[entryDescription.first]) = serial::read_u8_be(cur);
                break;
            }
        });
        for (size_t stringId = 0; stringId < stringCount; stringId++)
        {
            string_alloc_buffer[stringId].second = serial::read_string(cur);
        }
    }

};

}; // namespace: dlnk