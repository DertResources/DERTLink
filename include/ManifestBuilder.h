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
private:
        std::deque<DeviceBuilder> deviceManifests;
        DeviceDictonary* DD;

    }; // class end

}; // namespace: dlnk