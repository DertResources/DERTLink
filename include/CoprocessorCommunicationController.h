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
    , OM{}
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

        CM::SendMessage(BV);
    }

    void StageTwoHandshake_ToBuffer()
    {
        ClearBuffer();
        //Message Header
        CM::WriteMessageType(BV, MT::HANDSHAKE_STEP_TWO);

        //Manifest
        CM::WriteMessageBodySignature(BV, MBS::MANIFEST);
        MB.WriteManifestToBuffer(BV);

        //end of message
        CM::WriteMessageBodySignature(BV, MBS::END_OF_MESSAGE);

        CM::SendMessage(BV);
    }

    void ClearBuffer() {BV.clear();}

    static std::shared_ptr<CPCC> Instance;
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