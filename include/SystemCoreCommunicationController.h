#pragma once
#include "../include/DeviceDictonary.h"
#include "Device.h"
#include "DynamicBuffer.h"
#include "ManifestBuilder.h"
#include "ObjectManager.h"

namespace dlnk
{
class SystemCoreCommunicationController;
using SCCC = SystemCoreCommunicationController;

class SystemCoreCommunicationController
{
public:
    SystemCoreCommunicationController()
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
    }
    
    void Stage_One_HandShake_FromBuffer()
    {
        
    }

    inline void ClearBuffer() {BV.clear();}

    static std::unique_ptr<SCCC> Instance;
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