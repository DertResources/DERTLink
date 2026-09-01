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

    void AllocateDouble(double*& ptr   ) { buffers.AllocateDouble(ptr);         };
    void AllocateFloat(float*& ptr     ) { buffers.AllocateFloat(ptr);          };
    void AllocateUInt8(uint8_t*& ptr   ) { buffers.AllocateUInt8(ptr);          };
    void AllocateUInt16(uint16_t*& ptr ) { buffers.AllocateUInt16(ptr);         };
    void AllocateUInt32(uint32_t *& ptr) { buffers.AllocateUInt32(ptr);         }; 
    void AllocateUInt64(uint64_t *& ptr) { buffers.AllocateUInt64(ptr);         };
    void AllocateInt8(int8_t*& ptr     ) { buffers.AllocateInt8(ptr);           };
    void AllocateInt16(int16_t*& ptr   ) { buffers.AllocateInt16(ptr);          }; 
    void AllocateInt32(int32_t*& ptr   ) { buffers.AllocateInt32(ptr);          }; 
    void AllocateInt64(int64_t*& ptr   ) { buffers.AllocateInt64(ptr);          }; 
    void AllocateBool(bool*& ptr       ) { buffers.AllocateBool(ptr);           }; 
    void AllocateString(uint8_t& ptr   ) { buffers.AllocateString(ptr); };

private:
    std::atomic_int8_t lastWrittenIdx = -1;
    DynamicBuffer<3> buffers;
};

}; // namespace: dlnk