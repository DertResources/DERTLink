#include "../include/CommunicationHandler.h"
#include <algorithm>
namespace dlnk
{

ByteVector CommunicationManager::buffer = {};

std::vector<uint8_t> CommunicationManager::message_type_map =
{
    0x00,
    0x01,
    0x08,
    0x09,
    0x0a,
    0x0b,
    0x0c
};

std::vector<uint8_t> CommunicationManager::message_body_signature_map = 
{
    0x00,
    0x01,
    0x02,
    0x03,
    0xff
};

MESSAGE_BODY_SIGNATURE CommunicationManager::GetMessageBodySignatureFromId(uint8_t id)
{
    const auto it = std::find(message_body_signature_map.begin(),
                               message_body_signature_map.end(),
                               id);
    if (message_body_signature_map.end() == it)
    {
        std::cout << "ERORR: enum id not in ennumeration, defaulting to value of zero" << std::endl;
        return static_cast<MESSAGE_BODY_SIGNATURE>(0);
    }
    return static_cast<MESSAGE_BODY_SIGNATURE>(id);
}

MESSAGETYPE CommunicationManager::GetMessageTypeFromId(uint8_t id)
{
    const auto it = std::find(message_type_map.begin(),
                               message_type_map.end(),
                               id);
    if (message_type_map.end() == it)
    {
        std::cout << "ERORR: enum id not in ennumeration, defaulting to value of zero" << std::endl;
        return static_cast<MESSAGETYPE>(0);   
    }
    return static_cast<MESSAGETYPE>(id);
}

uint8_t CommunicationManager::GetId(MESSAGETYPE mt)
{
    return static_cast<uint8_t>(mt);
}
uint8_t CommunicationManager::GetId(MESSAGE_BODY_SIGNATURE mt)
{
    return static_cast<uint8_t>(mt);
}

void CommunicationManager::WriteMessageType(ByteVector& BV, MESSAGETYPE mt)
{
    serial::write_u8_be(BV, GetId(mt));
}

void CommunicationManager::WriteMessageBodySignature(ByteVector& BV, MESSAGE_BODY_SIGNATURE mt)
{
    serial::write_u8_be(BV, GetId(mt));
}

MESSAGETYPE CommunicationManager::ReadMessageType(const Byte*& cur)
{
    return GetMessageTypeFromId(serial::read_u8_be(cur));
}

MESSAGE_BODY_SIGNATURE CommunicationManager::ReadMessageBodySignature(const Byte*& cur)
{
    return GetMessageBodySignatureFromId(serial::read_u8_be(cur));
}

}; // namespace: dnlk