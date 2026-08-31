#pragma once
#include "../include/DynamicBuffer.h"
#include <any>
#include <cassert>
#include <cstdint>
#include <string>
#include <algorithm>
#include <numeric>
#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"
#include "../include/DebugTracer.h"

namespace dlnk
{
template<uint8_t bufferCount>
std::deque<std::any>& DynamicBuffer<bufferCount>::GetBuffer(uint8_t bufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::GetBuffer");
    static_assert(0 <= bufferIdx && bufferIdx <= bufferCount);
    return buffers[bufferIdx];
}

// Allocators
template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateDouble(double*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateDouble");
    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<double>(0.0));
    ptr = std::any_cast<double>(&(buffers[0].back()));
    bufferDescription.push_back({buffers[0].size(), DataType::DOUBLE});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateFloat(float*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateFloat");
    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<float>(0.0f));
    ptr = std::any_cast<float>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::FLOAT });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt8(uint8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt8");
    stackSize += 1; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<uint8_t>(0));
    ptr = std::any_cast<uint8_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::UINT8 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt16(uint16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt16");
    stackSize += 2; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<uint16_t>(0));
    ptr = std::any_cast<uint16_t>(&(buffers[0].back()));
    bufferDescription.push_back({buffers[0].size(), DataType::UINT16});
}
template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt32(uint32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt32");
    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<uint32_t>(0));
    ptr = std::any_cast<uint32_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::UINT32 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt64(uint64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt64");
    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<uint64_t>(0));
    ptr = std::any_cast<uint64_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::UINT64 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt8(int8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt8");
    stackSize += 1; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<int8_t>(0));
    ptr = std::any_cast<int8_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::INT8 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt16(int16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt16");
    stackSize += 2; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<int16_t>(0));
    ptr = std::any_cast<int16_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::INT16 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt32(int32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt32");
    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<int32_t>(0));
    ptr = std::any_cast<int32_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::INT32 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt64(int64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt64");
    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<int64_t>(0));
    ptr = std::any_cast<int64_t>(&(buffers[0].back()));
    bufferDescription.push_back({ buffers[0].size(), DataType::INT64 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateBool(bool*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateBool");
    stackSize += 1; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].emplace_back(std::make_any<bool>(false));
    ptr = std::any_cast<bool>(&(buffers[0].back()));
    bufferDescription.push_back({buffers[0].size(), DataType::BOOL});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateString(uint8_t& stringId)
{
    SCOPE_TRACE("DynamicBuffer::AllocateString");
    for(size_t i = 0; i < bufferCount; i++)
        string_alloc_buffers[i].push_back("");
    stringId = static_cast<uint8_t>(string_alloc_buffers[0].size());
    bufferDescription.push_back({buffers[0].size(), DataType::STRING});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SwapBuffers(uint8_t a_BufferIdx, uint8_t b_BufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::SwapBuffers");
    static_assert(buffers[a_BufferIdx].size() == buffers[b_BufferIdx], "Buffers to swap are of different sizes");
    buffers[a_BufferIdx].swap(buffers[b_BufferIdx]);
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SortMappingsForExport()
{
    SCOPE_TRACE("DynamicBuffer::SortMappingsForExport");
    std::sort(bufferDescription.begin(), bufferDescription.end(),
        [](Mapping& a, Mapping& b) -> bool{
            return a.targetIndex < b.targetIndex;
        });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SortMappingsForImport()
{
    SCOPE_TRACE("DynamicBuffer::SortMappingsForImport");
    std::sort(bufferDescription.begin(), bufferDescription.end(),
        [](Mapping& a, Mapping& b) -> bool{
            return a.originalIndex < b.originalIndex;
        });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::ExportBytes(ByteVector& expBuff, uint8_t sourceBufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::ExportBytes");
    static_assert(0 <= sourceBufferIdx && sourceBufferIdx <= bufferCount);

    // reserve space for data
    size_t heapSize = std::accumulate(string_alloc_buffers.begin(), string_alloc_buffers.end(), static_cast<size_t>(0),
        [](size_t sum, const std::string& sa) {
            return sum + sa.size() + 1; // add 1 for size # of string
        });
    uint32_t entireBufferSize = static_cast<uint32_t>(stackSize + heapSize + /* number of strings*/sizeof(uint8_t));
    expBuff.reserve(expBuff.size() + entireBufferSize);

    // Write number of strings
    serial::write_u8_be(expBuff, static_cast<uint8_t>(string_alloc_buffers.size()));

    // write out every stack value
    std::deque<std::any>& sourceBuffer = buffers[sourceBufferIdx];
    for(Mapping& map : bufferDescription)
    {
        switch (map.typeOfData) {
        case DataType::DOUBLE:
            serial::write_double_be(expBuff, std::any_cast<double>(sourceBuffer[map.orignalIndex])); break;
        case DataType::FLOAT:
            serial::write_float_be(expBuff, std::any_cast<float>(sourceBuffer[map.orignalIndex])); break;
        case DataType::UINT8:
            serial::write_u8_be(expBuff, std::any_cast<uint8_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::UINT16:
            serial::write_u16_be(expBuff, std::any_cast<uint16_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::UINT32:
            serial::write_u32_be(expBuff, std::any_cast<uint32_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::UINT64:
            serial::write_u64_be(expBuff, std::any_cast<uint64_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::INT8:
            serial::write_i8_be(expBuff, std::any_cast<int8_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::INT16:
            serial::write_i16_be(expBuff, std::any_cast<int16_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::INT32:
            serial::write_i32_be(expBuff, std::any_cast<int32_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::INT64:
            serial::write_i64_be(expBuff, std::any_cast<int64_t>(sourceBuffer[map.orignalIndex])); break;
        case DataType::BOOL:
            serial::write_bool_be(expBuff, std::any_cast<bool>(sourceBuffer[map.orignalIndex])); break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Export Bytes unknown type");
        }
    }
    // write all strings
    for(std::string& se : string_alloc_buffers[sourceBufferIdx])
    {
        serial::write_string(expBuff, se);
    }
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::ImportBytes(const Byte*& cur, uint8_t targetBufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::ImportBytes");
    static_assert(0 <= targetBufferIdx && targetBufferIdx <= bufferCount);
    size_t stringCount = static_cast<size_t>(serial::read_u8_be(cur));

    std::deque<std::any>& tartgetBuffer = buffers[targetBufferIdx];
    for(Mapping& map : bufferDescription)
    {
        switch (map.typeOfData) {
        case DataType::DOUBLE:
            *std::any_cast<double*>(tartgetBuffer[map.targetIndex]) = serial::read_double_be(cur);
            break;
        case DataType::FLOAT:
            *std::any_cast<float*>(tartgetBuffer[map.targetIndex]) = serial::read_float_be(cur);
            break;
        case DataType::UINT8:
            *std::any_cast<uint8_t*>(tartgetBuffer[map.targetIndex]) = serial::read_u8_be(cur);
            break;
        case DataType::UINT16:
            *std::any_cast<uint16_t*>(tartgetBuffer[map.targetIndex]) = serial::read_u16_be(cur);
            break;
        case DataType::UINT32:
            *std::any_cast<uint32_t*>(tartgetBuffer[map.targetIndex]) = serial::read_u32_be(cur);
            break;
        case DataType::UINT64:
            *std::any_cast<uint64_t*>(tartgetBuffer[map.targetIndex]) = serial::read_u64_be(cur);
            break;
        case DataType::INT8:
            *std::any_cast<int8_t*>(tartgetBuffer[map.targetIndex]) = serial::read_i8_be(cur);
            break;
        case DataType::INT16:
            *std::any_cast<int16_t*>(tartgetBuffer[map.targetIndex]) = serial::read_i16_be(cur);
            break;
        case DataType::INT32:
            *std::any_cast<int32_t*>(tartgetBuffer[map.targetIndex]) = serial::read_i32_be(cur);
            break;
        case DataType::INT64:
            *std::any_cast<int64_t*>(tartgetBuffer[map.targetIndex]) = serial::read_i64_be(cur);
            break;
        case DataType::BOOL:
            *std::any_cast<bool*>(tartgetBuffer[map.targetIndex]) = serial::read_bool_be(cur);
            break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Import Bytes unknown type");
        }
    }
    for (size_t stringId = 0; stringId < stringCount; stringId++)
    {
        string_alloc_buffers[targetBufferIdx][stringId] = serial::read_string(cur);
    }
}

} // namespace: dlnk