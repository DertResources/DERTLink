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
    ManifestBuilder(ObjectManager& _OM,
                    DeviceDictonary& _DD,
                    ShortDevVector& _shortDeviceDictonary, 
                    DynamicBuffer& _dbDesiredState, 
                    DynamicBuffer& _dbFeedback)
    : OM{_OM}
    , DD{_DD}
    , shortDeviceDictonary {_shortDeviceDictonary}
    , dbDesiredState {_dbDesiredState}
    , dbFeedback {_dbFeedback}
    {}

    DeviceBuilder& BuildNewDevice(std::string deviceName);

    void InitalizeControlObjects();

    bool ValidateManifest();

    void WriteManifestToBuffer(ByteVector& BV);

    void ReadManifestFromBuffer(const Byte*& cur);

    void Print(int tabs = 0);
private:

    bool CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep);

    bool CheckDataDirection(DataEntryPtr& dep);

    void ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep);

    void DataTypeError(DataEntryPtr& dep);

    void DataDirectionError();
    
    bool IsInit(EntryManifest& em);

    bool IsFeedback(DataEntryPtr& dep);

    bool IsDesiredState(DataEntryPtr& dep);

    bool DataDirectionSwitch(EntryManifest & em,
                             DataEntryPtr & dep,
                             std::function<bool(void)> initFunc,
                             std::function<bool(void)> desiredFunc,
                             std::function<bool(void)> feedbackFunc);

    std::deque<DeviceBuilder> deviceManifests{};
    ObjectManager& OM;
    DeviceDictonary& DD;
    ShortDevVector& shortDeviceDictonary;
    DynamicBuffer& dbDesiredState;
    DynamicBuffer& dbFeedback;

    

}; // class end
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
}; // namespace: dlnk  