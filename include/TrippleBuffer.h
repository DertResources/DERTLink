#pragma once
#include "../include/DynamicBuffer.h"
#include <deque>
#include <mutex>
#include <utility>


namespace dlnk
{

class TrippleBuffer
{
private:
    static constexpr uint8_t accessorBufferIdx = 0; 
public:
    void RequestData()
    {
        std::lock_guard<class Mutex>
        buffers.SwapBuffers(lastWrittenIdx, accessorBufferIdx);
    }

    private:
    
    int8_t lastWrittenIdx = -1;
    DynamicBuffer<3> buffers;

};

}; // namespace: dlnk