#include "../include/DataEntry.h"

namespace dlib::communication
{
template<typename T>
DataEntry<T>::DataEntry(std::string _name, DataDirection _direction)
: name{_name}
, direction{_direction}
, byteOffset{0}
{
    SetDataType<T>();
}

template<typename T>
void DataEntry<T>::WriteToBuffer(ByteVector& byteBuffer)
{
    write_string(byteBuffer, name);

    Byte DataDescription = static_cast<Byte>(direction) << 4 |
                           static_cast<Byte>(dataType)       ;
    write_u8_be(byteBuffer, DataDescription);
}

template<typename T>
void DataEntry<T>::ReadFromBuffer(const Byte*& cur)
{
    name = read_string(cur);
    Byte DataDescription = read_u8_be(cur);
    direction = DataDirection(static_cast<uint8_t>(DataDescription >> 4));
    dataType = DataType((DataDescription & 15));
}

template<typename T>
void DataEntry<T>::Print(uint8_t tabs) const
{
    std::string tabString = std::string(2*tabs, ' ');
    // Name
    dlib::print_t(tabString + "\033[92mData Entry: \"" + name + "\""  + "\033[32m\n");
    // Data Type
    dlib::print_t(tabString + "  data type: ");
    PrintDataType(dataType);
    dlib::print_t("\n");
    //Data Direction
    dlib::print_t(tabString + "  data direction: ");
    switch (direction)
    {
    case DataDirection::DESIREDSTATE:
        dlib::print_t("desired state"); break;
    case DataDirection::FEEDBACK:
        dlib::print_t("sensor feedback"); break;
    case DataDirection::INIT:
        dlib::print_t("hardware initalization"); break;
    default:
        dlib::print_t("unknown"); break;
    }
    dlib::print_t("\n");
}

}; // namespace: dlib::communication