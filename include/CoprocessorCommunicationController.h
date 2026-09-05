#pragma once
#include "DertLink/include/Device.h"
#include "DeviceDictonary.h"
#include "ManifestBuilder.h"
#include "DynamicBuffer.h"
#include "ObjectManager.h"
#include "SerializeHelper.h"
#include "../include/CommunicationHandler.h"
#include "TCPHarness.h"
#include <memory>
#include <optional>

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
        TCP}
    , TCP{TCPHarness::ConnectionType::Client}
    {}
    ManifestBuilder& GetManifestBuilder() {return MB;}
    DeviceDictonary& GetDeviceDictonary() {return DD;}
    void StartCommunication();

    bool StageOneHandshake_ToBuffer();

    bool StageTwoHandshake_ToBuffer();

    void ClearBuffer() { BV.clear(); }

    static std::optional<CPCC> Instance;

    static void StartProcess();
    static void ShutDownProcess();
    static void RestartProcess();
private:
    DeviceDictonary DD;
    ShortDevVector SDV;
    ObjectManager   OM;
    ManifestBuilder MB;
    TCPHarness      TCP;
    ByteVector      BV;
};

};