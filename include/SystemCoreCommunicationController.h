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
    void StartCommunication();
    
    void StageOneHandshake_FromBuffer();

    void StageTwoHandshake_FromBuffer();

    void CreateAllObjects();

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