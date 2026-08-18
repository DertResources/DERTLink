#include "../include/DynamicBuffer.h"
#include <any>
#include <cassert>
#include <cstdint>
#include <string>
#include <utility>
#include <algorithm>
#include <numeric>
#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"
#include "../include/DebugTracer.h"

namespace dlnk
{
void DynamicBuffer::AllocateBytes(size_t byteCount, size_t& byteOffset)
{
    SCOPE_TRACE("DynamicBuffer::AllocateBytes");
    // idfk some computer bullshit
    stackSize += byteCount;
    size_t aligned = (buffer.size() + 7) & ~7;
    size_t padding = aligned - buffer.size();
    buffer.resize(buffer.size() + padding + byteCount);
    byteOffset = aligned;
}

// Allocators
void DynamicBuffer::AllocateDouble(double*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateDouble");
    buffer.emplace_back(std::make_any<double>(0.0));
    ptr = std::any_cast<double>(&(buffer.back()));
    bufferDescription.push_back({buffer.size(), DataType::DOUBLE});
}

void DynamicBuffer::AllocateFloat(float*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateFloat");
    buffer.emplace_back(std::make_any<float>(0.0f));
    ptr = std::any_cast<float>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::FLOAT });
}

void DynamicBuffer::AllocateUInt8(uint8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt8");
    buffer.emplace_back(std::make_any<uint8_t>(0));
    ptr = std::any_cast<uint8_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::UINT8 });
}

void DynamicBuffer::AllocateUInt16(uint16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt16");
    buffer.emplace_back(std::make_any<uint16_t>(0));
    ptr = std::any_cast<uint16_t>(&(buffer.back()));
    bufferDescription.push_back({buffer.size(), DataType::UINT16});
}

void DynamicBuffer::AllocateUInt32(uint32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt32");
    buffer.emplace_back(std::make_any<uint32_t>(0));
    ptr = std::any_cast<uint32_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::UINT32 });
}

void DynamicBuffer::AllocateUInt64(uint64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt64");
    buffer.emplace_back(std::make_any<uint64_t>(0));
    ptr = std::any_cast<uint64_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::UINT64 });
}

void DynamicBuffer::AllocateInt8(int8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt8");
    buffer.emplace_back(std::make_any<int8_t>(0));
    ptr = std::any_cast<int8_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::INT8 });
}

void DynamicBuffer::AllocateInt16(int16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt16");
    buffer.emplace_back(std::make_any<int16_t>(0));
    ptr = std::any_cast<int16_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::INT16 });
}

void DynamicBuffer::AllocateInt32(int32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt32");
    buffer.emplace_back(std::make_any<int32_t>(0));
    ptr = std::any_cast<int32_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::INT32 });
}

void DynamicBuffer::AllocateInt64(int64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt64");
    buffer.emplace_back(std::make_any<int64_t>(0));
    ptr = std::any_cast<int64_t>(&(buffer.back()));
    bufferDescription.push_back({ buffer.size(), DataType::INT64 });
}

void DynamicBuffer::AllocateBool(bool*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateBool");
    buffer.emplace_back(std::make_any<bool>(false));
    ptr = std::any_cast<bool>(&(buffer.back()));
    bufferDescription.push_back({buffer.size(), DataType::BOOL});
}

void DynamicBuffer::AllocateString(uint8_t& stringId)
{
    SCOPE_TRACE("DynamicBuffer::AllocateString");
    string_alloc_buffer.push_back("");
    stringId = static_cast<uint8_t>(string_alloc_buffer.size());
    bufferDescription.push_back({buffer.size(), DataType::STRING});
}

void DynamicBuffer::ExportBytes(ByteVector& expBuff)
{
    SCOPE_TRACE("DynamicBuffer::ExportBytes");
    // get total size of "heap"
    size_t heapSize = std::accumulate(string_alloc_buffer.begin(), string_alloc_buffer.end(), static_cast<size_t>(0),
        [](size_t sum, const std::string& sa) {
            return sum + sa.size() + 1; // add 1 for size of string
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
            serial::write_double_be(expBuff, std::any_cast<double>(buffer[entryDescription.first])); break;
        case DataType::FLOAT:
            serial::write_float_be(expBuff, std::any_cast<float>(buffer[entryDescription.first])); break;
        case DataType::UINT8:
            serial::write_u8_be(expBuff, std::any_cast<uint8_t>(buffer[entryDescription.first])); break;
        case DataType::UINT16:
            serial::write_u16_be(expBuff, std::any_cast<uint16_t>(buffer[entryDescription.first])); break;
        case DataType::UINT32:
            serial::write_u32_be(expBuff, std::any_cast<uint32_t>(buffer[entryDescription.first])); break;
        case DataType::UINT64:
            serial::write_u64_be(expBuff, std::any_cast<uint64_t>(buffer[entryDescription.first])); break;
        case DataType::INT8:
            serial::write_i8_be(expBuff, std::any_cast<int8_t>(buffer[entryDescription.first])); break;
        case DataType::INT16:
            serial::write_i16_be(expBuff, std::any_cast<int16_t>(buffer[entryDescription.first])); break;
        case DataType::INT32:
            serial::write_i32_be(expBuff, std::any_cast<int32_t>(buffer[entryDescription.first])); break;
        case DataType::INT64:
            serial::write_i64_be(expBuff, std::any_cast<int64_t>(buffer[entryDescription.first])); break;
        case DataType::BOOL:
            serial::write_bool_be(expBuff, std::any_cast<bool>(buffer[entryDescription.first])); break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Export Bytes unknown type");
        }
    });
    // write all strings
    std::for_each(string_alloc_buffer.begin(), string_alloc_buffer.end(),
        [&](std::string& se) {
            serial::write_string(expBuff, se);
        });
}

void DynamicBuffer::ImportBytes(const Byte*& cur)
{
    SCOPE_TRACE("DynamicBuffer::ImportBytes");
    size_t stringCount = static_cast<size_t>(serial::read_u8_be(cur));
    std::for_each(bufferDescription.begin(), bufferDescription.end(),
    [&](std::pair<size_t, DataType>& entryDescription) {
        switch (entryDescription.second) {
        case DataType::DOUBLE:
            *std::any_cast<double*>(buffer[entryDescription.first]) = serial::read_double_be(cur);
            break;
        case DataType::FLOAT:
            *std::any_cast<float*>(buffer[entryDescription.first]) = serial::read_float_be(cur);
            break;
        case DataType::UINT8:
            *std::any_cast<uint8_t*>(buffer[entryDescription.first]) = serial::read_u8_be(cur);
            break;
        case DataType::UINT16:
            *std::any_cast<uint16_t*>(buffer[entryDescription.first]) = serial::read_u16_be(cur);
            break;
        case DataType::UINT32:
            *std::any_cast<uint32_t*>(buffer[entryDescription.first]) = serial::read_u32_be(cur);
            break;
        case DataType::UINT64:
            *std::any_cast<uint64_t*>(buffer[entryDescription.first]) = serial::read_u64_be(cur);
            break;
        case DataType::INT8:
            *std::any_cast<int8_t*>(buffer[entryDescription.first]) = serial::read_i8_be(cur);
            break;
        case DataType::INT16:
            *std::any_cast<int16_t*>(buffer[entryDescription.first]) = serial::read_i16_be(cur);
            break;
        case DataType::INT32:
            *std::any_cast<int32_t*>(buffer[entryDescription.first]) = serial::read_i32_be(cur);
            break;
        case DataType::INT64:
            *std::any_cast<int64_t*>(buffer[entryDescription.first]) = serial::read_i64_be(cur);
            break;
        case DataType::BOOL:
            *std::any_cast<bool*>(buffer[entryDescription.first]) = serial::read_bool_be(cur);
            break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Import Bytes unknown type");
        }
        });
    for (size_t stringId = 0; stringId < stringCount; stringId++) {
        string_alloc_buffer[stringId] = serial::read_string(cur);
    }
}

} // namespace: dlnk