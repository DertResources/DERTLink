#include "../include/ManifestBuilder.h"

#include <string>
#include <algorithm>
#include <variant>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <deque>
#include <vector>
#include <any>
#include <optional>
#include <functional>
#include <memory>
#include "../include/DeviceBuilder.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"
#include "../include/InitalizationGroup.h"
#include "../include/RuntimeControlObject.h"
#include "../include/SerializeHelper.h"
#include "../include/DebugTracer.h"

namespace dlnk
{

DeviceBuilder& ManifestBuilder::BuildNewDevice(std::string deviceName)
{
    SCOPE_TRACE("ManifestBuilder::BuildNewDevice");
    deviceManifests.emplace_back(deviceName, this);
    return deviceManifests.back();
}

bool ManifestBuilder::ValidateManifest()
{
    SCOPE_TRACE("ManifestBuilder::ValidateManifest");
    return std::all_of(deviceManifests.begin(), deviceManifests.end(),
    [&](DeviceBuilder& db) -> bool {
        // find device
        ShortDev DeviceMatch;
        if (!DeviceDictonary::FindShortDevice(shortDeviceDictonary, DeviceMatch, db.GetDeviceName()))
        {
            THROW_ERROR("Manifest Error: Device Not Found");
            return false;
        }
        // check every device entry
        return std::all_of(db.GetEntryManifests().begin(), db.GetEntryManifests().end(),
        [&](EntryManifest& em) -> bool{
            // find entry
            DataEntryPtr dataEntryMatch;
            if(!DeviceDictonary::FindShortDataEntry(DeviceMatch, dataEntryMatch, em.entryName))
            {
                THROW_ERROR("Manifest Error: Entry Not Found");
                return false;
            }
            if (!CheckDataDirection(dataEntryMatch)) {
                THROW_ERROR("Data Direction Wrong");
                return false;
            }
            return DataDirectionSwitch(em, dataEntryMatch, [&](){ //Init
                if (!CheckTypeMatching(em, dataEntryMatch)) {
                    DataTypeError(dataEntryMatch);
                    return false;
                }
                ConvertDataTypes(em, dataEntryMatch);
                std::visit([&](auto& de){
                    em.entryType = de.GetDataType();
                }, *dataEntryMatch);

                return true;
            },[&](){ //Desired
                return true;
            },[&](){ //Feedback
                return true;
            }); 
        });
    });
}

bool ManifestBuilder::CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::CheckTypeMatching");
    return std::visit([&](auto& de) -> bool {
        return std::visit(overloaded {
            [&](std::string value) -> bool {
                return de.GetDataType() == DataType::STRING;
            }, [&](bool value) -> bool {
                return de.GetDataType() == DataType::BOOL;
            }, [&](double value) -> bool {
                return (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](float value) -> bool {
                return (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](int64_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](int32_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](int16_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](int8_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](uint8_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](uint16_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](uint32_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }, [&](uint64_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }
        }, em.entryData.value());
    }, *dep);
}

void ManifestBuilder::ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::ConvertDataTypes");
    std::visit([&](auto& de)
    {
        std::visit([&](auto& data) {
            using T = std::decay_t<decltype(data)>;
            auto dt = de.GetDataType();
            if (dt == DataType::UINT8) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<uint8_t>(data);
            } else if (dt == DataType::UINT16) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<uint16_t>(data);
            } else if (dt == DataType::UINT32) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<uint32_t>(data);
            } else if (dt == DataType::UINT64) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<uint64_t>(data);
            } else if (dt == DataType::INT8) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<int8_t>(data);
            } else if (dt == DataType::INT16) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<int16_t>(data);
            } else if (dt == DataType::INT32) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<int32_t>(data);
            } else if (dt == DataType::INT64) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<int64_t>(data);
            } else if (dt == DataType::FLOAT) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<float>(data);
            } else if (dt == DataType::DOUBLE) {
                if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
                    em.entryData = static_cast<double>(data);
            } else if (dt == DataType::BOOL) {
                if constexpr (std::is_same_v<T, bool>)
                    em.entryData = data;
            } else if (dt == DataType::STRING) {
                if constexpr (std::is_same_v<T, std::string>)
                    em.entryData = data;
            }
        }, em.entryData.value());
    }, *dep);
}

bool ManifestBuilder::CheckDataDirection(DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::CheckDataDirection");
    return std::visit([&](auto& de) -> bool{
        return !(de.GetDataDirection() == DataDirection::INIT) ||
               !(de.GetDataDirection() == DataDirection::DESIREDSTATE ||
                 de.GetDataDirection() == DataDirection::FEEDBACK);
    }, *dep);
}

void ManifestBuilder::DataTypeError(DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::DataTypeError");
    std::visit([&](auto& de) {
        THROW_ERROR("Data type not matched");
        PrintDataType(de.GetDataType());
        std::cout << std::endl;
    }, *dep);
}

void ManifestBuilder::InitalizeControlObjects()
{
    SCOPE_TRACE("ManifestBuilder::InitalizeControlObjects");
    // for every device in the manifest
    std::for_each(deviceManifests.begin(), deviceManifests.end(),
    [&](DeviceBuilder& db){
        std::optional<std::any> createInfoCursor;
        // make a copy of the short device
        ShortDev sd;
        // find the matching device in the short dict
        DeviceDictonary::FindShortDevice(shortDeviceDictonary, sd, db.GetDeviceName());
        ShortDev sdr = sd;
        // grab all initalization groups from the matching device dictonary 
        std::deque<InitalizationGroup>& igv = sd.DevicePtr->GetInitaliztionGroupVector();
        // then, for each initalization group
        std::for_each(igv.begin(), igv.end(),
        [&](InitalizationGroup& ig) {
            // make a list of indexes that match indexes in the short entry vector
            std::vector<int32_t> idxs;
            // also check that every entry in each init group exists
            if(std::all_of(ig.getDataEntryVector().begin(), ig.getDataEntryVector().end(),
            [&](DataEntryVariant& dev) -> bool{
                // check that each entry exists and save its index
                return std::visit([&](auto& de) -> bool{
                    std::vector<EntryManifest>::iterator It =
                        std::find_if(db.GetEntryManifests().begin(),
                        db.GetEntryManifests().end(),
                        [&](EntryManifest& em) {
                            return em.entryName == de.GetName();
                        });

                    if (It == db.GetEntryManifests().end())
                        return false;
                    else
                    {
                        idxs.push_back(static_cast<int32_t>(It - db.GetEntryManifests().begin()));
                        //std::cout << de.GetName() << ": " << idxs.back() << std::endl;
                        return true;
                    }
                }, dev);
            }))
            {
                // if every entry exists, delete their pointers
                std::sort(idxs.rbegin(), idxs.rend());
                // erase any repetes just in case
                idxs.erase(std::unique(idxs.begin(), idxs.end()), idxs.end());
                std::for_each(idxs.begin(), idxs.end(),
                [&](size_t idx){
                    sd.GetShortDEVector().erase(sd.GetShortDEVector().begin() + idx);
                });
                // and run the int command
                //std::cout << ig.GetName() << std::endl;
                std::string s = sd.DeviceName;
                std::any& any_obj = OM.NewCreateInfo(s);
                ig.RunInitCmd(any_obj, sdr, tcpHarness, db);
            }
        });
    });
    OM.ConstructAllControlObjects();
}

bool ManifestBuilder::DataDirectionSwitch(EntryManifest& em,
                                          DataEntryPtr& dep,
                                          std::function<bool(void)> initFunc,
                                          std::function<bool(void)> desiredFunc,
                                          std::function<bool(void)> feedbackFunc)
{
    SCOPE_TRACE("ManifestBuilder::DataDirectionSwitch");
    return std::visit([&](auto& de) -> bool {
        if (em.entryData.has_value())
            return initFunc();
        else if (de.GetDataDirection() == DataDirection::FEEDBACK)
            return desiredFunc();
        else
            return feedbackFunc();
    }, *dep);
}

bool ManifestBuilder::IsInit(EntryManifest& em)
{
    SCOPE_TRACE("ManifestBuilder::IsInit");
    return em.entryData.has_value();
}

bool ManifestBuilder::IsFeedback(DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::IsFeedback");
    return std::visit([](auto& de) -> bool {
        return de.GetDataDirection() == DataDirection::FEEDBACK;
    },
        *dep);
}

bool ManifestBuilder::IsDesiredState(DataEntryPtr& dep)
{
    SCOPE_TRACE("ManifestBuilder::IsDesiredState");
    return std::visit([](auto& de) -> bool {
        return de.GetDataDirection() == DataDirection::DESIREDSTATE;
    },
        *dep);
}

void ManifestBuilder::ReadManifestFromBuffer(const Byte*& cur)
{
    SCOPE_TRACE("ManifestBuilder::ReadManifestFromBuffer");
    uint8_t DeviceManifestCount = serial::read_u8_be(cur);
    deviceManifests.clear();
    for (size_t i = 0; i < DeviceManifestCount; i++) {
        std::string deviceName = serial::read_string(cur);
        DeviceBuilder& db = BuildNewDevice(deviceName);
        db.ReadBuffer(cur);
    }
    ValidateManifest();
}

void ManifestBuilder::WriteManifestToBuffer(ByteVector& BV)
{
    SCOPE_TRACE("ManifestBuilder::WriteManifestToBuffer");
    bool result = ValidateManifest();
    if(!result)
    {
        THROW_ERROR("Manifest not valid");
        return;
    }
    // std::cout << "Is Manifest Valid: " << std::boolalpha
    //           << result
    //           << std::noboolalpha << std::endl;

    serial::write_u8_be(BV, static_cast<uint8_t>(deviceManifests.size()));
    for (DeviceBuilder& db : deviceManifests) {
        serial::write_string(BV, db.GetDeviceName());
        db.WriteBuffer(BV);
    }
}

std::string ManifestBuilder::Print(int tabs)
{
    SCOPE_TRACE("ManifestBuilder::Print");
    std::string out = "";
    out += "Manifest:\n";
    for(DeviceBuilder& db : deviceManifests)
    {
        out += db.Print(tabs + 1);
    }
    if(tabs == 0)
    {
        DISPLAY_DEBUG("");
        return "";
    }
    else
    {
        return out;
    }
}

}; // namespace: dlnk
