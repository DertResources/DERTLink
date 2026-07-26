#pragma once

#include <deque>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <cstdint>

#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"

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
    void ImportBytes(const Byte*& cur);

};

}; // namespace: dlnk