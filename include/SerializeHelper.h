#pragma once
#include <vector>
#include <string>
#include <cstdint>


namespace dlib::communication
{
static constexpr size_t DLIB_MAX_STRING_LENGTH = 256;

typedef uint8_t Byte;
typedef std::vector<Byte> ByteVector;

// write 8,16,32,64
inline void write_u8_be(ByteVector& buf, uint8_t v)
{
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u16_be(ByteVector& buf, uint16_t v)
{
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u32_be(ByteVector& buf, uint32_t v)
{
    buf.push_back(static_cast<Byte>(v >> 24));
    buf.push_back(static_cast<Byte>(v >> 16));
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u64_be(ByteVector& buf, uint64_t v)
{
    buf.push_back(static_cast<Byte>(v >> 56));
    buf.push_back(static_cast<Byte>(v >> 48));
    buf.push_back(static_cast<Byte>(v >> 40));
    buf.push_back(static_cast<Byte>(v >> 32));
    buf.push_back(static_cast<Byte>(v >> 24));
    buf.push_back(static_cast<Byte>(v >> 16));
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

// // // // // // // // // // // // // // // // // // // // // 
// WRITE UINT 8, 16, 32, 64
// // // // // // // // // // // // // // // // // // // // // 

inline void write_i8_be(ByteVector& buf, int8_t v) { write_u8_be(buf, static_cast<uint8_t>(v)); }

inline void write_i16_be(ByteVector& buf, int16_t v) { write_u16_be(buf, static_cast<uint16_t>(v)); }

inline void write_i32_be(ByteVector& buf, int32_t v) { write_u32_be(buf, static_cast<uint32_t>(v)); }

inline void write_i64_be(ByteVector& buf, int64_t v) { write_u64_be(buf, static_cast<uint64_t>(v)); }

// // // // // // // // // // // // // // // // // // // // // 
// READ UINT 8, 16, 32, 64
// // // // // // // // // // // // // // // // // // // // // 

inline uint8_t read_u8_be(const Byte*& cur)
{
    const Byte* begin = cur;
    cur += 1;
    return Byte(begin[0]);
}

inline uint16_t read_u16_be(const Byte*& cur)
{
    const Byte* begin = cur;
    cur += 2;
    return (uint16_t(begin[0]) << 8 |
            uint16_t(begin[1])      );
}

inline uint32_t read_u32_be(const Byte*& cur)
{
    const Byte* begin = cur;
    cur += 4;
    return (uint32_t(begin[0]) << 24 |
            uint32_t(begin[1]) << 16 | 
            uint32_t(begin[2]) << 8  |
            uint32_t(begin[3])       );
}

inline uint64_t read_u64_be(const Byte*& cur)
{
    const Byte* begin = cur;
    cur += 8;
    return (uint64_t(begin[0]) << 56 |
            uint64_t(begin[1]) << 48 |
            uint64_t(begin[2]) << 40 |
            uint64_t(begin[3]) << 32 |
            uint64_t(begin[4]) << 24 |
            uint64_t(begin[5]) << 16 |
            uint64_t(begin[6]) << 8  |
            uint64_t(begin[7])       );
}

// // // // // // // // // // // // // // // // // // // // // 
// READ INT 8, 16, 32, 64
// // // // // // // // // // // // // // // // // // // // // 

inline int8_t read_i8_be(const Byte*& cur) { return static_cast<int8_t>(read_u8_be(cur)); }

inline int16_t read_i16_be(const Byte*& cur) { return static_cast<int16_t>(read_u16_be(cur)); }

inline int32_t read_i32_be(const Byte*& cur) { return static_cast<int32_t>(read_u32_be(cur)); }

inline int64_t read_i64_be(const Byte*& cur) { return static_cast<int64_t>(read_u64_be(cur)); }

// // // // // // // // // // // // // // // // // // // // // 
// WRITE 
// // // // // // // // // // // // // // // // // // // // // 
inline void write_bool_be(ByteVector& buf, bool v)
{
    write_u8_be(buf, static_cast<uint8_t>(v? 1 : 0));
}

inline void write_float_be(ByteVector& buf, float v)
{
    uint32_t val;
    std::memcpy(&val, &v, sizeof(uint32_t));
    write_u32_be(buf, val);
}

inline void write_double_be(ByteVector& buf, double v)
{
    uint64_t val;
    std::memcpy(&val, &v, sizeof(uint64_t));
    write_u64_be(buf, val);
}


inline void write_string(ByteVector& buf, const std::string& v)
{
    size_t len = v.length();
    for (size_t pos = 0; pos < len && pos < DLIB_MAX_STRING_LENGTH; pos++)
    {
        buf.push_back(static_cast<Byte>(v[pos]));
    }
    buf.push_back(static_cast<Byte>('\0'));
}
// // // // // // // // // // // // // // // // // // // // // 
// READ
// // // // // // // // // // // // // // // // // // // // // 

inline bool read_bool_be(const Byte*& cur)
{
    return static_cast<bool>(read_u8_be(cur));
}

inline float read_float_be(const Byte*& cur)
{
    uint32_t val = read_u32_be(cur);
    float v;
    std::memcpy(&v, &val, sizeof(uint32_t));
    return v;
}

inline double read_double_be(const Byte*& cur)
{
    uint64_t val = read_u64_be(cur);
    double v;
    std::memcpy(&v, &val, sizeof(uint64_t));
    return v;
}

inline std::string read_string(const Byte*& cur)
{
    std::string out;

    for(size_t pos = 0;
        static_cast<char>(cur[pos]) != '\0' && pos < DLIB_MAX_STRING_LENGTH;
        out.push_back(static_cast<char>(cur[pos])), pos++);
    cur = &cur[out.length()+1];
    return out;
}

}; // namespace: dlib::communication