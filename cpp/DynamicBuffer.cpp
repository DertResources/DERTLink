#include "../include/DynamicBuffer.h"

namespace dlnk
{
// Allocators
void DynamicBuffer::AllocateDouble(double*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(double), byteOffset);
    ptr = reinterpret_cast<double*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::DOUBLE);
}

void DynamicBuffer::AllocateFloat(float*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(float), byteOffset);
    ptr = reinterpret_cast<float*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::FLOAT);
}

void DynamicBuffer::AllocateUInt8(uint8_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(uint8_t), byteOffset);
    ptr = reinterpret_cast<uint8_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::UINT8);
}

void DynamicBuffer::AllocateUInt16(uint16_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(uint16_t), byteOffset);
    ptr = reinterpret_cast<uint16_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::UINT16);
}

void DynamicBuffer::AllocateUInt32(uint32_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(uint32_t), byteOffset);
    ptr = reinterpret_cast<uint32_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::UINT32);
}

void DynamicBuffer::AllocateUInt64(uint64_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(uint64_t), byteOffset);
    ptr = reinterpret_cast<uint64_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::UINT64);
}

void DynamicBuffer::AllocateInt8(int8_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(int8_t), byteOffset);
    ptr = reinterpret_cast<int8_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::INT8);
}

void DynamicBuffer::AllocateInt16(int16_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(int16_t), byteOffset);
    ptr = reinterpret_cast<int16_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::INT16);
}

void DynamicBuffer::AllocateInt32(int32_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(int32_t), byteOffset);
    ptr = reinterpret_cast<int32_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::INT32);
}

void DynamicBuffer::AllocateInt64(int64_t*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(int64_t), byteOffset);
    ptr = reinterpret_cast<int64_t*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::INT64);
}

void DynamicBuffer::AllocateBool(bool*& ptr)
{
    size_t byteOffset;
    AllocateBytes(sizeof(bool), byteOffset);
    ptr = reinterpret_cast<bool*>(&buffer[byteOffset]);
    bufferDescription.emplace_back(byteOffset, DataType::BOOL);
}

void DynamicBuffer::AllocateString(uint8_t*& ptr)
{
    size_t stringId = string_alloc_buffer.size();
    if (stringId < 256) {
        size_t byteOffset;
        AllocateBytes(sizeof(uint8_t), byteOffset);
        ptr = reinterpret_cast<uint8_t*>(&buffer[byteOffset]);
        bufferDescription.emplace_back(byteOffset, DataType::STRING);
        string_alloc_buffer.emplace_back(byteOffset, std::string(""));
        *ptr = static_cast<uint8_t>(stringId);
    } else {
        std::cout << "String Allocation Failed: 255 Strings already inside buffer" << std::endl;
    }
}

void DynamicBuffer::ExportBytes(ByteVector& expBuff)
{
    // get total size of "heap"
    size_t heapSize = std::accumulate(string_alloc_buffer.begin(), string_alloc_buffer.end(), static_cast<size_t>(0),
        [](size_t sum, const std::pair<size_t, std::string>& sa) {
            return sum + sa.second.size() + 1; // add 1 for size of string
        });

    // clear existing buffer and reserve space for
    uint32_t entireBufferSize = static_cast<uint32_t>(stackSize + heapSize + sizeof(uint8_t));
    expBuff.reserve(entireBufferSize);
    expBuff.clear();
    // Write number of strings
    serial::write_u8_be(expBuff, static_cast<uint8_t>(string_alloc_buffer.size()));

    // write out every stack value
    std::for_each(bufferDescription.begin(), bufferDescription.end(),
        [&](std::pair<size_t, DataType>& entryDescription) {
            switch (entryDescription.second) {
            case DataType::DOUBLE:
                serial::write_double_be(expBuff, *reinterpret_cast<double*>(&buffer[entryDescription.first]));
                break;
            case DataType::FLOAT:
                serial::write_float_be(expBuff, *reinterpret_cast<float*>(&buffer[entryDescription.first]));
                break;
            case DataType::UINT8:
                serial::write_u8_be(expBuff, *reinterpret_cast<uint8_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::UINT16:
                serial::write_u16_be(expBuff, *reinterpret_cast<uint16_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::UINT32:
                serial::write_u32_be(expBuff, *reinterpret_cast<uint32_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::UINT64:
                serial::write_u64_be(expBuff, *reinterpret_cast<uint64_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::INT8:
                serial::write_i8_be(expBuff, *reinterpret_cast<int8_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::INT16:
                serial::write_i16_be(expBuff, *reinterpret_cast<int16_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::INT32:
                serial::write_i32_be(expBuff, *reinterpret_cast<int32_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::INT64:
                serial::write_i64_be(expBuff, *reinterpret_cast<int64_t*>(&buffer[entryDescription.first]));
                break;
            case DataType::BOOL:
                serial::write_bool_be(expBuff, *reinterpret_cast<bool*>(&buffer[entryDescription.first]));
                break;
            case DataType::STRING:
                serial::write_u8_be(expBuff, *reinterpret_cast<uint8_t*>(&buffer[entryDescription.first]));
                break;
            }
        });
    // write all strings
    std::for_each(string_alloc_buffer.begin(), string_alloc_buffer.end(),
        [&](std::pair<size_t, std::string>& se) {
            serial::write_string(expBuff, se.second);
        });
}

} // namespace: dlnk