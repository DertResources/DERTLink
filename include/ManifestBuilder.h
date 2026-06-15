#pragma once
#include <deque>
#include <string>
#include "../include/DeviceBuilder.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"

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
    ManifestBuilder(DeviceDictonary& _DD)
    : DD {&_DD} {}

    DeviceBuilder& BuildNewDevice(std::string deviceName);

    bool ValidateManifest();

    bool CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep);

    bool CheckDataDirection(DataEntryPtr& dep);

    void ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep);

    void DataTypeError(DataEntryPtr& dep);

    void DataDirectionError();
    
    inline bool IsInit(EntryManifest& em) { return em.entryData.has_value(); }

private:
    std::deque<DeviceBuilder> deviceManifests;
    DeviceDictonary* DD;

}; // class end

}; // namespace: dlnk  