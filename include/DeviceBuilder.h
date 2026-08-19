#pragma once
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <type_traits>
#include <cstdint>
#include "../include/DataEntry.h"
#include "DERTLink/include/DebugTracer.h"
#include "SerializeHelper.h"
#include <iostream>

namespace dlnk
{

class ManifestBuilder;

using EntryDataVariant = std::variant<
    std::string, bool, double, float,
    int64_t, int32_t, int16_t, int8_t,
    uint64_t, uint32_t, uint16_t, uint8_t> ;

template<typename T, typename Variant>
struct is_variant_member;

template<typename T, typename... Ts>
struct is_variant_member<T, std::variant<Ts...>>
    : std::disjunction<std::is_same<T, Ts>...> {
};

template<typename T, typename Variant>
inline constexpr bool is_variant_member_v = is_variant_member<T, Variant>::value;

struct EntryManifest
{
    EntryManifest(std::string _entryName,
        std::optional<EntryDataVariant> _entryData)
        : entryName{ _entryName }
        , entryData{ _entryData }
        , entryType{ std::nullopt }
    {}

    std::string entryName;
    std::optional<EntryDataVariant> entryData;
    std::optional<DataType> entryType;
};

class DeviceBuilder
{
public:
    DeviceBuilder(std::string _DeviceName, ManifestBuilder* _parentptr)
        : deviceName{_DeviceName}
        , parentptr{_parentptr}
    {}

    DeviceBuilder& AssignData(std::string DataEntryName);

    // Exact variant type match
    template<typename T, std::enable_if_t<
        is_variant_member_v<T, EntryDataVariant> &&
        !std::is_convertible_v<T, std::string>, int> = 0 >
    DeviceBuilder & AssignData(std::string DataEntryName, T value);

    // String and const char* 
    template<typename T, std::enable_if_t<
        std::is_convertible_v<T, std::string>, int> = 0 >
    DeviceBuilder & AssignData(std::string DataEntryName, T value);

    // int literals -> int32_t
    DeviceBuilder& AssignData(std::string DataEntryName, int value);

    // bool explicit overload (prevents bool -> int ambiguity)
    DeviceBuilder& AssignData(std::string DataEntryName, bool value);

    ManifestBuilder& ExitDeviceBuilder();
    std::vector<EntryManifest>& GetEntryManifests();
    std::string GetDeviceName() const;
    void WriteBuffer(ByteVector& BV);
    void ReadBuffer(const Byte*& cur);
    std::string Print(int tabs = 0);

private:
    std::string deviceName;
    std::vector<EntryManifest> entryManifests;
    ManifestBuilder* parentptr;
};

}; // namespace dlnk

#include "../cpp/DeviceBuilder.tpp"