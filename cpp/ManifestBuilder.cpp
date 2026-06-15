#include "../include/ManifestBuilder.h"

namespace dlnk
{

DeviceBuilder& ManifestBuilder::BuildNewDevice(std::string deviceName)
{
    deviceManifests.emplace_back(deviceName, this);
    return deviceManifests.back();
}

bool ManifestBuilder::ValidateManifest()
{
    ShortDevVector ShortDevDict;
    DeviceDictonary::FillShortDevVector(*DD, ShortDevDict);
    DeviceDictonary::SortShortDevVector(ShortDevDict);

    return std::all_of(deviceManifests.begin(), deviceManifests.end(),
    [&](DeviceBuilder& db) -> bool {
        // find device
        ShortDev devDict;
        if (!DeviceDictonary::FindShortDevice(ShortDevDict, devDict, db.GetDeviceName())) {
            return false;
        }
        // check every device entry
        bool valid = true;
        std::for_each(db.GetEntryManifests().begin(), db.GetEntryManifests().end(),
        [&](EntryManifest& em) {
            // find entry
            DataEntryPtr deviceMatch;
            if(!DeviceDictonary::FindShortDataEntry(devDict, deviceMatch, em.entryName)) { 
                valid = false;
                return;
            }
            // validate
            // entryData has 
            if (IsInit(em)) {
                if (!CheckTypeMatching(em, deviceMatch)) {
                    DataTypeError(deviceMatch);
                    valid = false;
                }
                ConvertDataTypes(em, deviceMatch);
            }
            if (!CheckDataDirection(deviceMatch)) {
                DataDirectionError();
                valid = false;
            }
        });
        return valid;
    });
}

bool ManifestBuilder::CheckTypeMatching(EntryManifest& em, DataEntryPtr& dep)
{
    return std::visit([&](auto& de) -> bool {
        return std::visit(overloaded {
            [&](std::string value) -> bool {
                return de.GetDataType() == DataType::STRING;
            },
            [&](bool value) -> bool {
                return de.GetDataType() == DataType::BOOL;
            },
            [&](double value) -> bool {
                return (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](float value) -> bool {
                return (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](int64_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](int32_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](int16_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](int8_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](uint8_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](uint16_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](uint32_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            },
            [&](uint64_t value) -> bool {
                return (de.GetDataType() == DataType::UINT8) || (de.GetDataType() == DataType::UINT16) || (de.GetDataType() == DataType::UINT32) || (de.GetDataType() == DataType::UINT64) || (de.GetDataType() == DataType::INT8) || (de.GetDataType() == DataType::INT16) || (de.GetDataType() == DataType::INT32) || (de.GetDataType() == DataType::INT64) || (de.GetDataType() == DataType::FLOAT) || (de.GetDataType() == DataType::DOUBLE);
            }
        }, em.entryData.value());
    }, *dep);
}

void ManifestBuilder::ConvertDataTypes(EntryManifest& em, DataEntryPtr& dep)
{
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
    return std::visit([&](auto& de) -> bool{
        return !(de.GetDataDirection() == DataDirection::INIT) ||
               !(de.GetDataDirection() == DataDirection::DESIREDSTATE ||
                 de.GetDataDirection() == DataDirection::FEEDBACK);
    }, *dep);
}

void ManifestBuilder::DataTypeError(DataEntryPtr& dep)
{
    std::visit([&](auto& de) {
        std::cout << "data type not matched: ";
        PrintDataType(de.GetDataType());
        std::cout << std::endl;
    }, *dep);
}

void ManifestBuilder::DataDirectionError()
{
    std::cout << "Data Direction Wrong" << std::endl;
}

}; // namespace: dlnk
