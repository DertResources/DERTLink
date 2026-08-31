#pragma once
#include "../include/DynamicBuffer.h"
#include "../include/SerializeHelper.h"
#include <atomic>

namespace dlnk
{

class TripleBuffer
{
private:
    static constexpr uint8_t accessorBufferIdx = 0; 
public:
    void RequestData();

    void WriteData(const Byte*& cur);

private:
    std::atomic_int8_t lastWrittenIdx = -1;
    DynamicBuffer<3> buffers;
};

}; // namespace: dlnk