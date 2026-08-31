#include "../include/TripleBuffer.h"
#include "../include/DebugTracer.h"

namespace dlnk
{

void TripleBuffer::RequestData()
{
    SCOPE_TRACE("TrippleBuffer::RequestData");

    if(lastWrittenIdx == -1)
        THROW_ERROR("No Data to pull from tripple buffer. State Machine Fault");

    buffers.SwapBuffers(lastWrittenIdx, accessorBufferIdx);
}

void TripleBuffer::WriteData(const Byte*& cur)
{
    SCOPE_TRACE("TripleBuffer::WriteData");
    
    if(lastWrittenIdx == 1)
    {
        buffers.ImportBytes(cur, 2);
        lastWrittenIdx = 2;
    }
    else
    {
        buffers.ImportBytes(cur, 1);
        lastWrittenIdx = 1;
    }

}

}; // namespace: dlnk