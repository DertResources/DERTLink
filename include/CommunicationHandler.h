#pragma once
#include <cstdint>
#include "../include/SerializeHelper.h"
#include "../include/SerializeHelper.h"
namespace dlnk
{
enum class MESSAGETYPE: uint8_t
{
    FAILURE                 = 0  ,                
    SUCCESS                 = 1  ,                
    HANDSHAKE_STEP_ONE      = 8  ,     
    HANDSHAKE_STEP_TWO      = 9  ,     
    HANDSHAKE_STEP_THREE    = 10 ,   
    PERIOTIC_TRANSFER       = 11 ,      
    COMMUNICATION_CESSATION = 12
};

enum class MESSAGE_BODY_SIGNATURE: uint8_t
{
    DEVICE_DICTONARY = 0,
    MANIFEST         = 1,
    ERROR_MESSAGE    = 2,
    FEEDBACK_BUFFER  = 3,
    END_OF_MESSAGE   = 255
};

class CommunicationManager
{
public:
    static void WriteMessageType(ByteVector& BV, MESSAGETYPE mt);
    
    static void WriteMessageBodySignature(ByteVector& BV, MESSAGE_BODY_SIGNATURE mt);

    static MESSAGETYPE ReadMessageType(const Byte*& cur);

    static MESSAGE_BODY_SIGNATURE ReadMessageBodySignature(const Byte*& cur);

    static void SendMessage(ByteVector& BV) { buffer = BV; }

    static ByteVector& ReciveMessage() { return buffer; }
private:
    static std::vector<uint8_t> message_type_map;

    static std::vector<uint8_t> message_body_signature_map;

    static uint8_t GetId(MESSAGETYPE mt);

    static uint8_t GetId(MESSAGE_BODY_SIGNATURE mt);

    static MESSAGETYPE GetMessageTypeFromId(uint8_t id);

    static MESSAGE_BODY_SIGNATURE GetMessageBodySignatureFromId(uint8_t id);

    static ByteVector buffer;
};

}; // namespace: dlnk