#pragma once
#include <deque>
#include <string>
#include <variant>
#include <functional>
#include <cstdint>
#include "../include/DeviceBuilder.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"
#include "../include/DynamicBuffer.h"

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
    ManifestBuilder(DeviceDictonary & _DD,
                    ShortDevVector & _shortDeviceDictonary, 
                    DynamicBuffer& _dbDesiredState, 
                    DynamicBuffer& _dbFeedback)
    : DD{_DD}
    , shortDeviceDictonary {_shortDeviceDictonary}
    , dbDesiredState {_dbDesiredState}
    , dbFeedback {_dbFeedback}
    {}

    DeviceBuilder& BuildNewDevice(std::string deviceName);

    bool ValidateManifest();

    bool CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep);

    bool CheckDataDirection(DataEntryPtr& dep);

    void ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep);

    void DataTypeError(DataEntryPtr& dep);

    void DataDirectionError();
    
    void AllocateDataFeedback(EntryManifest& em);

    void AllocateDataDesiredState(EntryManifest& em);
    
    //template<typename T, typename N>
    //T EmplaceData(std::function<T(N)> func,
    //              std::string key,
    //              ShortDev& DeviceEntries);

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

    void InitalizeControlObjects();

private:
    std::deque<DeviceBuilder> deviceManifests;
    DeviceDictonary& DD;
    ShortDevVector& shortDeviceDictonary;
    DynamicBuffer& dbDesiredState;
    DynamicBuffer& dbFeedback;

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