#pragma once
#include "../include/DeviceDictonary.h"
#include "Device.h"
#include "DynamicBuffer.h"
#include "ManifestBuilder.h"
#include "ObjectManager.h"

#include "../include/CommunicationHandler.h"

#include <wpi/print.h>

namespace dlnk
{
class SystemCoreCommunicationController;
using SCCC = SystemCoreCommunicationController;

using CM = CommunicationManager;
using MBS = MESSAGE_BODY_SIGNATURE;
using MT = MESSAGETYPE;

class SystemCoreCommunicationController
{
public:
    SystemCoreCommunicationController()
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
        
        OM.SetupMaps(DD);
    }
    
    void StageOneHandshake_FromBuffer()
    {
        BV = CM::ReciveMessage();
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
            wpi::println("Dictonaries Match");
        else
            wpi::println("Error: Dictonaries don't match");
    }

    void StageTwoHandshake_FromBuffer()
    {
        BV = CM::ReciveMessage();
        const Byte* cur = &BV[0];
        MT messageType;
        MBS messageBodySignature;
        
        // Message Header
        messageType = CM::ReadMessageType(cur);
        if(messageType != MT::HANDSHAKE_STEP_TWO)
            std::cout << "ERROR: Wrong Message Type" << std::endl;

        // Message Body
        messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::MANIFEST)
            std::cout << "ERROR: Wrong Message Body Signature" << std::endl;

        MB.ReadManifestFromBuffer(cur);
        
         messageBodySignature = CM::ReadMessageBodySignature(cur);
        if(messageBodySignature != MBS::END_OF_MESSAGE)
            std::cout << "ERROR: Wrong Message Body Signature" << std::endl;
    }

    void CreateAllObjects()
    {
        MB.InitalizeControlObjects();
    }

    inline void ClearBuffer() {BV.clear();}

    static std::shared_ptr<SCCC> Instance;
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