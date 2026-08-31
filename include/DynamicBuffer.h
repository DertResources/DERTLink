#pragma once

#include <deque>
#include <vector>
#include <string>
#include <cstdint>
#include <mutex>

#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"

namespace dlnk
{
template<uint8_t bufferCount>
class DynamicBuffer
{
private:
    struct GuardedBuffer
    {
        std::mutex mutex;
        // buffer is a deque so that pointers remain valid on resize
        std::deque<std::any> buffer;
        std::vector<std::string> stringBuffer;
    };
    struct Mapping
    {
        size_t originalIndex;
        size_t targetIndex;
        DataType typeOfData;
    };
    std::mutex bufferDescMutex;
    std::vector<Mapping> bufferDescription;
    size_t stackSize = 0;
    std::array<GuardedBuffer, bufferCount> buffers;
    
public:
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
    void AllocateString(uint8_t& ptr);
    
    // Special String Handling
    // inline std::string ReadString(uint8_t heapIdPtr) { return buffers[0].[heapIdPtr]; }
    // inline void WriteString(uint8_t heapIdPtr, std::string str) { string_alloc_buffers[0][heapIdPtr] = str; }

    void SwapBuffers(uint8_t a_BufferIdx, uint8_t b_BufferIdx);

    void SortMappingsForExport();
    void SortMappingsForImport();

    // populate Buffer
    void ExportBytes(ByteVector& expBuff, uint8_t sourceBufferIdx);
    void ImportBytes(const Byte*& cur, uint8_t targetBufferIdx);
};

}; // namespace: dlnk

#include "../cpp/DynamicBuffer.tpp"

/*
Workable example:

original data: 1,2,3,4,5

transformed data: 4,2,5,1,3

mapping (idxs):
0 > 3
1 > 1
2 > 4
3 > 0
4 > 2


for exporting, original is the store data...
so sort based on target

for inporting, original is the input...
so sort based on target

simple, just sort based on target

*/