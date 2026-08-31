#pragma once
#include "../include/DynamicBuffer.h"
#include <any>
#include <cassert>
#include <cstdint>
#include <mutex>
#include <string>
#include <algorithm>
#include <numeric>
#include <tuple>
#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"
#include "../include/DebugTracer.h"

namespace dlnk
{
// Allocators
template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateDouble(double*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateDouble");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<double>(0.0));
    ptr = std::any_cast<double>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({idx, idx, DataType::DOUBLE});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateFloat(float*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateFloat");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<float>(0.0f));
    ptr = std::any_cast<float>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({idx, idx, DataType::FLOAT });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt8(uint8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt8");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 1; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<uint8_t>(0));
    ptr = std::any_cast<uint8_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::UINT8 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt16(uint16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt16");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 2; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<uint16_t>(0));
    ptr = std::any_cast<uint16_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::UINT16});
}
template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt32(uint32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt32");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<uint32_t>(0));
    ptr = std::any_cast<uint32_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::UINT32 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateUInt64(uint64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateUInt64");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<uint64_t>(0));
    ptr = std::any_cast<uint64_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::UINT64 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt8(int8_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt8");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 1; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<int8_t>(0));
    ptr = std::any_cast<int8_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::INT8 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt16(int16_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt16");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 2; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<int16_t>(0));
    ptr = std::any_cast<int16_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::INT16 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt32(int32_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt32");
    
    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 4; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<int32_t>(0));
    ptr = std::any_cast<int32_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::INT32 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateInt64(int64_t*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateInt64");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );

    stackSize += 8; // add bytes to stack
    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<int64_t>(0));
    ptr = std::any_cast<int64_t>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({ idx, idx, DataType::INT64 });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateBool(bool*& ptr)
{
    SCOPE_TRACE("DynamicBuffer::AllocateBool");

    std::lock_guard bufferDescLock(bufferDescMutex);
    auto lock = std::apply(
        [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
        buffers
    );
    stackSize += 1; // add bytes to stack

    for(size_t i = 0; i < bufferCount; i++)
        buffers[i].buffer.emplace_back(std::make_any<bool>(false));

    ptr = std::any_cast<bool>(&(buffers[0].buffer.back()));
    size_t idx = buffers[0].buffer.size() - 1;
    bufferDescription.push_back({idx, idx, DataType::BOOL});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::AllocateString(uint8_t& stringId)
{
    SCOPE_TRACE("DynamicBuffer::AllocateString");
    
    THROW_ERROR("Allocate String Not Implemented");

    // std::lock_guard bufferDescLock(bufferDescMutex);
    // auto lock = std::apply(
    //     [](auto&... gb){ return std::scoped_lock((gb.mutex)...); },
    //     buffers
    // );
    // stringId = static_cast<uint8_t>(buffers[0].stringBuffer.size());

    // for(size_t i = 0; i < bufferCount; i++)
    //     buffers[i].stringBuffer.push_back("");

    // bufferDescription.push_back({buffers[0].buffer.size(), DataType::STRING});
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SwapBuffers(uint8_t a_BufferIdx, uint8_t b_BufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::SwapBuffers");
    if(!(a_BufferIdx < bufferCount))
        THROW_ERROR("Buffer Index out of bounds");
    if(!(b_BufferIdx < bufferCount))
        THROW_ERROR("Buffer Index out of bounds");
    
    if(a_BufferIdx == b_BufferIdx)
        return;

    GuardedBuffer& a_guardbuff = buffers[a_BufferIdx];
    GuardedBuffer& b_guardbuff = buffers[b_BufferIdx];
    std::scoped_lock lock(a_guardbuff.mutex, b_guardbuff.mutex);

    if(a_guardbuff.buffer.size() != b_guardbuff.buffer.size())
        THROW_ERROR("Buffers to swap are of different sizes");
    
    a_guardbuff.buffer.swap(b_guardbuff.buffer);
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SortMappingsForExport()
{
    SCOPE_TRACE("DynamicBuffer::SortMappingsForExport");
    // lock for indirect affects on import and export operations
    std::lock_guard lock(bufferDescMutex);
    
    std::sort(bufferDescription.begin(), bufferDescription.end(),
        [](Mapping& a, Mapping& b) -> bool{
            return a.targetIndex < b.targetIndex;
        });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::SortMappingsForImport()
{
    SCOPE_TRACE("DynamicBuffer::SortMappingsForImport");
    // lock for indirect affects on import and export operations
    std::lock_guard lock(bufferDescMutex);

    std::sort(bufferDescription.begin(), bufferDescription.end(),
        [](Mapping& a, Mapping& b) -> bool{
            return a.originalIndex < b.originalIndex;
        });
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::ExportBytes(ByteVector& expBuff, uint8_t sourceBufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::ExportBytes");
    
    if(!(sourceBufferIdx < bufferCount))
        THROW_ERROR("Buffer Index out of bounds");

    GuardedBuffer& guardBuff = buffers[sourceBufferIdx];
    
    std::lock_guard bufferDescLock(bufferDescMutex);
    std::lock_guard lock(guardBuff.mutex);

    // reserve space for data
    size_t heapSize = std::accumulate(guardBuff.stringBuffer.begin(), guardBuff.stringBuffer.end(), static_cast<size_t>(0),
        [](size_t sum, const std::string& sa) {
            return sum + sa.size() + 1; // add 1 for size # of string
        });
    uint32_t entireBufferSize = static_cast<uint32_t>(stackSize + heapSize + /* number of strings*/sizeof(uint8_t));
    expBuff.reserve(expBuff.size() + entireBufferSize);

    // Write number of strings
    serial::write_u8_be(expBuff, static_cast<uint8_t>(guardBuff.stringBuffer.size()));

    // write out every stack value
    std::deque<std::any>& sourceBuffer = guardBuff.buffer;
    for(Mapping& map : bufferDescription)
    {
        if(map.originalIndex >= sourceBuffer.size())
            THROW_ERROR("Target Index is out of bounds");
        
        switch (map.typeOfData) {
        case DataType::DOUBLE:
            serial::write_double_be(expBuff, std::any_cast<double>(sourceBuffer[map.originalIndex])); break;
        case DataType::FLOAT:
            serial::write_float_be(expBuff, std::any_cast<float>(sourceBuffer[map.originalIndex])); break;
        case DataType::UINT8:
            serial::write_u8_be(expBuff, std::any_cast<uint8_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::UINT16:
            serial::write_u16_be(expBuff, std::any_cast<uint16_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::UINT32:
            serial::write_u32_be(expBuff, std::any_cast<uint32_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::UINT64:
            serial::write_u64_be(expBuff, std::any_cast<uint64_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::INT8:
            serial::write_i8_be(expBuff, std::any_cast<int8_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::INT16:
            serial::write_i16_be(expBuff, std::any_cast<int16_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::INT32:
            serial::write_i32_be(expBuff, std::any_cast<int32_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::INT64:
            serial::write_i64_be(expBuff, std::any_cast<int64_t>(sourceBuffer[map.originalIndex])); break;
        case DataType::BOOL:
            serial::write_bool_be(expBuff, std::any_cast<bool>(sourceBuffer[map.originalIndex])); break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Export Bytes unknown type");
        }
    }
    // write all strings
    for(std::string& se : guardBuff.stringBuffer)
    {
        serial::write_string(expBuff, se);
    }
}

template<uint8_t bufferCount>
void DynamicBuffer<bufferCount>::ImportBytes(const Byte*& cur, uint8_t targetBufferIdx)
{
    SCOPE_TRACE("DynamicBuffer::ImportBytes");

    if(!(targetBufferIdx < bufferCount))
        THROW_ERROR("Buffer Index out of bounds");

    GuardedBuffer& guardBuff = buffers[targetBufferIdx];
    
    std::lock_guard bufferDescLock(bufferDescMutex);
    std::lock_guard lock(guardBuff.mutex);

    size_t stringCount = static_cast<size_t>(serial::read_u8_be(cur));
    
    if(stringCount != guardBuff.stringBuffer.size())
        THROW_ERROR("String count doesn't match");

    std::deque<std::any>& targetBuffer = guardBuff.buffer;
    for(Mapping& map : bufferDescription)
    {
        if(map.targetIndex >= targetBuffer.size())
            THROW_ERROR("Target Index is out of bounds");

        switch (map.typeOfData) {
        case DataType::DOUBLE:
            targetBuffer[map.targetIndex] = std::make_any<double>(serial::read_double_be(cur));
            break;
        case DataType::FLOAT:
            targetBuffer[map.targetIndex] = std::make_any<float>(serial::read_float_be(cur));
            break;
        case DataType::UINT8:
            targetBuffer[map.targetIndex] = std::make_any<uint8_t>(serial::read_u8_be(cur));
            break;
        case DataType::UINT16:
            targetBuffer[map.targetIndex] = std::make_any<uint16_t>(serial::read_u16_be(cur));
            break;
        case DataType::UINT32:
            targetBuffer[map.targetIndex] = std::make_any<uint32_t>(serial::read_u32_be(cur));
            break;
        case DataType::UINT64:
            targetBuffer[map.targetIndex] = std::make_any<uint64_t>(serial::read_u64_be(cur));
            break;
        case DataType::INT8:
            targetBuffer[map.targetIndex] = std::make_any<int8_t>(serial::read_i8_be(cur));
            break;
        case DataType::INT16:
            targetBuffer[map.targetIndex] = std::make_any<int16_t>(serial::read_i16_be(cur));
            break;
        case DataType::INT32:
            targetBuffer[map.targetIndex] = std::make_any<int32_t>(serial::read_i32_be(cur));
            break;
        case DataType::INT64:
            targetBuffer[map.targetIndex] = std::make_any<int64_t>(serial::read_i64_be(cur));
            break;
        case DataType::BOOL:
            targetBuffer[map.targetIndex] = std::make_any<bool>(serial::read_bool_be(cur));
            break;
        default:
            DISPLAY_ERROR("Error: Dynamic Buffer Import Bytes unknown type");
        }
    }
    for (size_t stringId = 0; stringId < stringCount; stringId++)
    {
        guardBuff.stringBuffer[stringId] = serial::read_string(cur);
    }
}

} // namespace: dlnk