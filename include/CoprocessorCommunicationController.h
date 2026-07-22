#pragma once
#include "DeviceDictonary.h"
#include "ManifestBuilder.h"
#include "DynamicBuffer.h"
#include "ObjectManager.h"
#include "SerializeHelper.h"
#include <memory>

namespace dlnk
{
class CoprocessorCommunicationController;
using CPCC = CoprocessorCommunicationController;

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
    }
    
    void BufferStageOneHandshake()
    {

    }

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