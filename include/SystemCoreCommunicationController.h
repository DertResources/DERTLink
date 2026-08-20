#pragma once
#include "../include/DeviceDictonary.h"
#include "Device.h"
#include "DynamicBuffer.h"
#include "ManifestBuilder.h"
#include "ObjectManager.h"

#include "../include/CommunicationHandler.h"
#include "frc/Errors.h"

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
    
    bool StageOneHandshake_FromBuffer();

    bool StageTwoHandshake_FromBuffer();

    bool CreateAllObjects();
    
    inline void ClearBuffer() {BV.clear();}

    static std::optional<SCCC> Instance;

    static void StartProcess();
    static void ShutDownProcess();
    static void RestartProcess();
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