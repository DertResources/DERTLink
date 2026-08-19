#include "../include/CoprocessorCommunicationController.h"
#include "../include/DebugTracer.h"
#include "DERTLink/include/DebugTracer.h"
namespace dlnk 
{
    std::shared_ptr<CPCC>CPCC::Instance = std::make_shared<CPCC>();

    void CPCC::StartCommunication()
    {
        SCOPE_TRACE("CoprocessorCommunicationController::StartCommunication");
        // Fill Short Device Dictonary when the DD is full
        DeviceDictonary::FillShortDevVector(DD, SDV);
        DeviceDictonary::SortShortDevVector(SDV);
    }

    bool CPCC::StageOneHandshake_ToBuffer()
    {
        SCOPE_TRACE("CoprocessorCommunicationController::StageOneHandshake_ToBuffer");
        ClearBuffer();
        //Message Header
        CM::WriteMessageType(BV, MT::HANDSHAKE_STEP_ONE);

        //device dictonary
        CM::WriteMessageBodySignature(BV, MBS::DEVICE_DICTONARY);
        DD.WriteToBuffer(BV);

        //end of message
        CM::WriteMessageBodySignature(BV, MBS::END_OF_MESSAGE);

        CM::SendMessage(BV);
        return true;
    }

    bool CPCC::StageTwoHandshake_ToBuffer()
    {
        SCOPE_TRACE("CoprocessorCommunicationController::StageTwoHandshake_ToBuffer");
        ClearBuffer();
        //Message Header
        CM::WriteMessageType(BV, MT::HANDSHAKE_STEP_TWO);

        //Manifest
        CM::WriteMessageBodySignature(BV, MBS::MANIFEST);
        MB.WriteManifestToBuffer(BV);

        //end of message
        CM::WriteMessageBodySignature(BV, MBS::END_OF_MESSAGE);

        CM::SendMessage(BV);
        return true;
    }
};