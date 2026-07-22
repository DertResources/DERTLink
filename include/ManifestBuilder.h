#pragma once
#include <deque>
#include <string>
#include <variant>
#include <cstdint>
#include "../include/DeviceBuilder.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"
#include "../include/DynamicBuffer.h"
#include "../include/ObjectManager.h"
#include "SerializeHelper.h"
#include <functional>
#include <iostream>


namespace dlnk
{


// overload visit
template<typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

class ManifestBuilder
{
public:
    ManifestBuilder(ObjectManager &_OM,
                    DeviceDictonary &_DD,
                    ShortDevVector &_shortDeviceDictonary, 
                    DynamicBuffer &_dbDesiredState, 
                    DynamicBuffer &_dbFeedback)
    : OM{_OM}
    , DD{_DD}
    , shortDeviceDictonary {_shortDeviceDictonary}
    , dbDesiredState {_dbDesiredState}
    , dbFeedback {_dbFeedback}
    {}

    DeviceBuilder& BuildNewDevice(std::string deviceName);

    void InitalizeControlObjects();

    bool ValidateManifest();

    void WriteManifestToBuffer(ByteVector& BV)
    {
        serial::write_u8_be(BV, deviceManifests.size());
        for(DeviceBuilder& db : deviceManifests)
        {
            serial::write_string(BV, db.GetDeviceName());
            db.WriteBuffer(BV);
        }
    }

    void ReadManifestFromBuffer(const Byte*& cur)
    {
        uint8_t DeviceManifestCount = serial::read_u8_be(cur);
        deviceManifests.clear();
        for(size_t i = 0; i < DeviceManifestCount; i++)
        {
            std::string deviceName = serial::read_string(cur);
            DeviceBuilder& db = BuildNewDevice(deviceName);
            db.ReadBuffer(cur);
        }
        ValidateManifest();
    }

private:

    bool CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep);

    bool CheckDataDirection(DataEntryPtr& dep);

    void ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep);

    void DataTypeError(DataEntryPtr& dep);

    void DataDirectionError();
    
    void AllocateDataFeedback(EntryManifest& em);

    void AllocateDataDesiredState(EntryManifest& em);
    
    //template<typename T, typename N>
    //T EmplaceData(std::function<T(N)> func,
    //              std::string key);

    inline bool IsInit(EntryManifest& em) { return em.entryData.has_value(); }
    inline bool IsFeedback(DataEntryPtr& dep) 
    {
        return std::visit([](auto& de) -> bool {
            return de.GetDataDirection() == DataDirection::FEEDBACK;
        }, *dep);
    }
    inline bool IsDesiredState(DataEntryPtr& dep)
    {
        return std::visit([](auto& de) -> bool {
            return de.GetDataDirection() == DataDirection::DESIREDSTATE;
        },
        *dep);
    }
    inline bool DataDirectionSwitch(EntryManifest & em, DataEntryPtr & dep, std::function<bool(void)> initFunc,
                                                                            std::function<bool(void)> desiredFunc,
                                                                            std::function<bool(void)> feedbackFunc )
    {
        return std::visit([&](auto& de) -> bool {
            if(em.entryData.has_value())
                return initFunc();
            else if (de.GetDataDirection() == DataDirection::FEEDBACK)
                return desiredFunc();
            else
                return feedbackFunc();
        }, *dep);
    }

    std::deque<DeviceBuilder> deviceManifests;
    ObjectManager& OM;
    DeviceDictonary& DD;
    ShortDevVector& shortDeviceDictonary;
    DynamicBuffer& dbDesiredState;
    DynamicBuffer& dbFeedback;

    //std::any currentControlObjectContext;
    ShortDev* CurrentDeviceContext;
    typedef std::variant<uint8_t  , uint8_t *,
                         uint16_t , uint16_t*,
                         uint32_t , uint32_t*,
                         uint64_t , uint64_t*,
                         int8_t   , int8_t  *,
                         int16_t  , int16_t *,
                         int32_t  , int32_t *,
                         int64_t  , int64_t *,
                         bool     , bool    *,
                         float    , float   *,
                         double   , double  *,
                         std::string> AllTypes;

}; // class end

}; // namespace: dlnk  