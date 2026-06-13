#pragma once

#include "../include/SerializeHelper.h"
#include "../include/DataEntry.h"
#include <deque>
#include <algorithm>

namespace dlib::communication
{

class DynamicBuffer
{
public:
    // Allocators
    inline void AllocateDouble (size_t& byteOffset, double   *& ptr) { AllocateBytes(sizeof(double   ), byteOffset); ptr = reinterpret_cast< double*   >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::DOUBLE); }
    inline void AllocateFloat  (size_t& byteOffset, float    *& ptr) { AllocateBytes(sizeof(float    ), byteOffset); ptr = reinterpret_cast< float*    >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::FLOAT);}
    inline void AllocateUInt8  (size_t& byteOffset, uint8_t  *& ptr) { AllocateBytes(sizeof(uint8_t  ), byteOffset); ptr = reinterpret_cast< uint8_t*  >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::UINT8);}
    inline void AllocateUInt16 (size_t& byteOffset, uint16_t *& ptr) { AllocateBytes(sizeof(uint16_t ), byteOffset); ptr = reinterpret_cast< uint16_t* >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::UINT16);}
    inline void AllocateUInt32 (size_t& byteOffset, uint32_t *& ptr) { AllocateBytes(sizeof(uint32_t ), byteOffset); ptr = reinterpret_cast< uint32_t* >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::UINT32);}
    inline void AllocateUInt64 (size_t& byteOffset, uint64_t *& ptr) { AllocateBytes(sizeof(uint64_t ), byteOffset); ptr = reinterpret_cast< uint64_t* >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::UINT64);}
    inline void AllocateInt8   (size_t& byteOffset, int8_t   *& ptr) { AllocateBytes(sizeof(int8_t   ), byteOffset); ptr = reinterpret_cast< int8_t*   >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::INT8);}
    inline void AllocateInt16  (size_t& byteOffset, int16_t  *& ptr) { AllocateBytes(sizeof(int16_t  ), byteOffset); ptr = reinterpret_cast< int16_t*  >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::INT16);}
    inline void AllocateInt32  (size_t& byteOffset, int32_t  *& ptr) { AllocateBytes(sizeof(int32_t  ), byteOffset); ptr = reinterpret_cast< int32_t*  >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::INT32);}
    inline void AllocateInt64  (size_t& byteOffset, int64_t  *& ptr) { AllocateBytes(sizeof(int64_t  ), byteOffset); ptr = reinterpret_cast< int64_t*  >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::INT64);}
    inline void AllocateBool   (size_t& byteOffset, bool     *& ptr) { AllocateBytes(sizeof(bool     ), byteOffset); ptr = reinterpret_cast< bool*     >(&buffer[byteOffset]); bufferDescription.emplace_back(byteOffset, DataType::BOOL);}
    inline void AllocateString (size_t& byteOffset                 ) { AllocateBytes(2                , byteOffset);                                                           bufferDescription.emplace_back(byteOffset, DataType::STRING);}
    // Special String Handling
    inline std::string ReadString(size_t byteOffset)
    {
        // TODO: FIX
        return "";
    }
    inline void WriteString(size_t byteOffset, std::string str)
    {
        string_alloc_buffer.emplace_back(byteOffset, str);
    }
    // populate Buffer
    inline void PopulateBuffer(Byte* startPtr, size_t length)
    {
        buffer.assign(startPtr, startPtr + length);
    }
    inline void ExportBytes(ByteVector& expBuff)
    {
        size_t heapSize =0;
            // set every string offset number
            std::for_each(string_alloc_buffer.begin(), string_alloc_buffer.end(),
            [&](std::pair<size_t, std::string>& se) {
                *reinterpret_cast<uint16_t*>(&expBuff[se.first]) = static_cast<uint16_t>(stackSize + heapSize);
                heapSize += se.second.size() + 1;
            });  
        
        // clear existing buffer and reserve space for 
        expBuff.reserve(stackSize + heapSize);
        expBuff.clear();

        // write out every stack value
        std::for_each(bufferDescription.begin(), bufferDescription.end(),
        [&](std::pair<size_t, DataType>& entryDescription){
            uint8_t outsize = 0;
            switch(entryDescription.second)
            {
                case DataType::DOUBLE:
                    write_double_be(expBuff, *reinterpret_cast<double*>(&buffer[entryDescription.first]));
                    break;
                case DataType::FLOAT:
                    write_float_be(expBuff, *reinterpret_cast<float*>(&buffer[entryDescription.first]));
                    break;
                case DataType::UINT8:
                    write_u8_be(expBuff, *reinterpret_cast<uint8_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::UINT16:
                    write_u16_be(expBuff, *reinterpret_cast<uint16_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::UINT32:
                    write_u32_be(expBuff, *reinterpret_cast<uint32_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::UINT64:
                    write_u64_be(expBuff, *reinterpret_cast<uint64_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::INT8:
                    write_i8_be(expBuff, *reinterpret_cast<int8_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::INT16:
                    write_i16_be(expBuff, *reinterpret_cast<int16_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::INT32:
                    write_i32_be(expBuff, *reinterpret_cast<int32_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::INT64:
                    write_i64_be(expBuff, *reinterpret_cast<int64_t*>(&buffer[entryDescription.first]));
                    break;
                case DataType::BOOL:
                    write_bool_be(expBuff, *reinterpret_cast<bool*>(&buffer[entryDescription.first]));
                    break;
                case DataType::STRING:
                    write_u16_be(expBuff, *reinterpret_cast<uint16_t*>(&buffer[entryDescription.first]));
                    break;
            }
        });
        // write all strings
        std::for_each(string_alloc_buffer.begin(), string_alloc_buffer.end(),
        [&](std::pair<size_t, std::string>& se) {
            write_string(expBuff, se.second);
        });
    }
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
    std::deque<Byte> buffer;
    std::vector<std::pair<size_t, DataType>> bufferDescription;
    std::vector<std::pair<size_t, std::string>> string_alloc_buffer;
    size_t stackSize = 0;
};

}; // namespace: dlib::communication