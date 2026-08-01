#pragma once
#include "DertLink/include/Device.h"
#include "DeviceDictonary.h"
#include "ManifestBuilder.h"
#include "DynamicBuffer.h"
#include "ObjectManager.h"
#include "SerializeHelper.h"
#include "../include/CommunicationHandler.h"
#include <memory>

namespace dlnk
{
class CoprocessorCommunicationController;
using CPCC = CoprocessorCommunicationController;
using CM = CommunicationManager;
using MBS = MESSAGE_BODY_SIGNATURE;
using MT = MESSAGETYPE;

class CoprocessorCommunicationController
{
public:
    CoprocessorCommunicationController()
    : DD{}
    , SDV{}
    , OM{DD}
    , MB{OM,
        DD,
        SDV,
        DB_DesiredState,
        DB_Feedback}
    , DB_Feedback{}
    , DB_DesiredState{}
    {}
    ManifestBuilder& GetManifestBuilder() {return MB;}
    DeviceDictonary& GetDeviceDictonary() {return DD;}
    void StartCommunication()
    {
        // Fill Short Device Dictonary when the DD is full
        DeviceDictonary::FillShortDevVector(DD, SDV);
        DeviceDictonary::SortShortDevVector(SDV);

        MB.ValidateManifest();
        //Stage 1 of handshake
        StageOneHandshake_ToBuffer();
    }

    /**
     * Writes out the Device Dictonary for checking
     */
    void StageOneHandshake_ToBuffer()
    {
        ClearBuffer();
        //Message Header
        CM::WriteMessageType(BV, MT::HANDSHAKE_STEP_ONE);

        //device dictonary
        CM::WriteMessageBodySignature(BV, MBS::DEVICE_DICTONARY);
        DD.WriteToBuffer(BV);

        //end of message
        CM::WriteMessageBodySignature(BV, MBS::END_OF_MESSAGE);
    }

    void StageOneHandshake_FromBuffer()
    {
        const Byte* cur = &BV[0];
        MT messageType;
        MBS messageBodySignature;
        // Message Header
        messageType = CM::ReadMessageType(cur);
        if(messageType != MT::HANDSHAKE_STEP_ONE)
            std::cout << "ERROR: Wrong Message Type" << std::endl;
        
        // Message Body
        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::DEVICE_DICTONARY)
            std::cout << "ERROR: Wrong Message Body Signature" << std::endl;
        DeviceDictonary dd;
        dd.ReadFromBuffer(cur);

        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::END_OF_MESSAGE)
            std::cout << "ERROR: Wrong Message Body Signature" << std::endl;

        if(DeviceDictonary::CompareDictonaries(dd, DD))
            std::cout << "Dictonaries Match" << std::endl;
        else
            std::cout << "Error: Dictonaries don't match" << std::endl;
    }

    void ClearBuffer() {BV.clear();}

    static std::unique_ptr<CPCC> Instance;
private:
    DeviceDictonary DD;
    ShortDevVector SDV;
    ObjectManager   OM;
    ManifestBuilder MB;
    DynamicBuffer   DB_Feedback;
    DynamicBuffer   DB_DesiredState;
    ByteVector      BV;
};

};