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
    using ShortDev = std::pair<std::string, std::vector<DataEntryVariant*>>;
    std::vector<ShortDev> ShortDevDict;

    // Make a total reference of
    std::for_each(DD->GetDeviceVector().begin(), DD->GetDeviceVector().end(),
        [&](Device& device) {
            ShortDev sd;
            sd.first = device.GetName();
            std::for_each(device.GetInitaliztionGroupVector().begin(), device.GetInitaliztionGroupVector().end(),
                [&](InitalizationGroup& ig) {
                    std::for_each(ig.getDataEntryVector().begin(), ig.getDataEntryVector().end(),
                        [&](DataEntryVariant& dev) {
                            sd.second.push_back(&dev);
                        });
                });
            ShortDevDict.push_back(sd);
        });

    // Sort Device Names
    std::sort(ShortDevDict.begin(), ShortDevDict.end(),
        [](ShortDev& sda, ShortDev& sdb) {
            return sda.first < sdb.first;
        });

    // Sort Data Entries
    std::for_each(ShortDevDict.begin(), ShortDevDict.end(),
        [&](ShortDev& sd) {
            std::sort(sd.second.begin(), sd.second.end(),
                [&](DataEntryVariant* deva, DataEntryVariant* devb) {
                    return std::visit([&](auto& dea) -> bool {
                        return std::visit([&](auto& deb) -> bool {
                            return dea.GetName() < deb.GetName();
                        },
                            *devb);
                    },
                        *deva);
                });
        });

    return std::all_of(deviceManifests.begin(), deviceManifests.end(),
    [&](DeviceBuilder& db) -> bool {
        // find device iterator
        auto devIt = std::lower_bound(ShortDevDict.begin(), ShortDevDict.end(), db.GetDeviceName(),
            [](ShortDev& sd, std::string target) -> bool {
                return sd.first < target;
            });
        // exit if device not found
        if (devIt == ShortDevDict.end() || devIt->first != db.GetDeviceName())
            return false;
        // dereference
        ShortDev& devDict = *devIt;
        // check every device entry
        bool valid = true;
        std::for_each(db.GetEntryManifests().begin(), db.GetEntryManifests().end(),
        [&](EntryManifest& em) {
            // find the device entry that matches the entry manifest
            auto deMatchIt = std::lower_bound(devDict.second.begin(), devDict.second.end(), em.entryName,
                [](DataEntryVariant* dev, std::string target) -> bool {
                    return std::visit([&](auto& de) {
                        return de.GetName() < target;
                    },
                        *dev);
                });
            // exit if no entry found
            if (deMatchIt == devDict.second.end() || std::visit([&](auto& de) { return de.GetName(); }, **deMatchIt) != em.entryName) {
                valid = false;
                return;
            }
            // validate
            bool result = std::visit([&](auto& de) -> bool {
                if (em.entryData.has_value()) {
                    bool dataDirectionMatched = de.GetDataDirection() == DataDirection::INIT;
                    if (!dataDirectionMatched) {
                        std::cout << "Data Direction Wrong" << std::endl;
                        return false;
                    }

                    bool dataTypeMatched = std::visit(overloaded {
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
                        },
                    },
                        em.entryData.value());
                    if (!dataTypeMatched) {
                        std::cout << "data type not matched: ";
                        PrintDataType(de.GetDataType());
                        std::cout << std::endl;
                        return false;
                    }
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
                    },
                        em.entryData.value());

                } else {
                    bool dataDirectionMatched = (de.GetDataDirection() == DataDirection::DESIREDSTATE || de.GetDataDirection() == DataDirection::FEEDBACK);
                    if (!dataDirectionMatched) {
                        std::cout << "Data Direction Wrong" << std::endl;
                        return false;
                    }
                }
                return true;
            },
                **deMatchIt);

            if (!result)
                valid = false;
        });
        return valid;
    });
}



}; // namespace: dlnk
