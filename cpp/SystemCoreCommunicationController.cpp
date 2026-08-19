#include "../include/SystemCoreCommunicationController.h"
#include "DERTLink/include/DebugTracer.h"

namespace dlnk 
{
    std::shared_ptr<SCCC> SCCC::Instance = std::make_shared<SCCC>();

    void SCCC::StartCommunication()
    {
        SCOPE_TRACE("SystemCoreCommunicationController::StartCommunication");
        // Fill Short Device Dictonary when the DD is full
        DeviceDictonary::FillShortDevVector(DD, SDV);
        DeviceDictonary::SortShortDevVector(SDV);
        
        OM.SetupMaps(DD);
    }


    bool SCCC::StageOneHandshake_FromBuffer()
    {
        SCOPE_TRACE("SystemCoreCommunicationController::StageOneHandshake_FromBuffer");
        BV = CM::ReciveMessage();
        const Byte* cur = &BV[0];
        MT messageType;
        MBS messageBodySignature;

        // Message Header
        messageType = CM::ReadMessageType(cur);
        if(messageType != MT::HANDSHAKE_STEP_ONE)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Type");
            return false;
        }
        
        // Message Body
        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::DEVICE_DICTONARY)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Body Signature");
            return false;
        }
        DeviceDictonary dd;
        dd.ReadFromBuffer(cur);

        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::END_OF_MESSAGE)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Body Signature");
            return false;
        }

        if(!DeviceDictonary::CompareDictonaries(dd, DD))
        {
            DISPLAY_ERROR("ERROR: Device Dictonaries dont match");
            return false;
        }
        
        return true;
    }

    bool SCCC::StageTwoHandshake_FromBuffer()
    {
        SCOPE_TRACE("SystemCoreCommunicationController::StageTwoHandshake_FromBuffer");
        BV = CM::ReciveMessage();
        const Byte* cur = &BV[0];
        MT messageType;
        MBS messageBodySignature;
        
        // Message Header
        messageType = CM::ReadMessageType(cur);
        if(messageType != MT::HANDSHAKE_STEP_TWO)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Type");
            return false;
        }

        // Message Body
        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::MANIFEST)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Body Signature");
            return false;
        }

        MB.ReadManifestFromBuffer(cur);
        
         messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::END_OF_MESSAGE)
        {
            DISPLAY_ERROR("ERROR: Wrong Message Body Signature");
            return false;
        }
        
        return true;
    }

    bool SCCC::CreateAllObjects()
    {
        SCOPE_TRACE("SystemCoreCommunicationController::CreateAllObjects");
        MB.InitalizeControlObjects();
        return true;
    }
};