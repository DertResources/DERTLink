#pragma once
#include <deque>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <utility>
#include <type_traits>
#include "../include/DataEntry.h"

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
            : DeviceName{_DeviceName}
            , parentptr{_parentptr}
        {}

        inline DeviceBuilder& AssignData(std::string DataEntryName) { entryManifests.emplace_back(DataEntryName, std::nullopt); return *this; }

        // Exact variant type match
        template<typename T, std::enable_if_t<
            is_variant_member_v<T, EntryDataVariant> &&
            !std::is_convertible_v<T, std::string>, int> = 0 >
        inline DeviceBuilder & AssignData(std::string DataEntryName, T value) { entryManifests.emplace_back(DataEntryName, value); return *this; }

        // String and const char* 
        template<typename T, std::enable_if_t<
            std::is_convertible_v<T, std::string>, int> = 0 >
        inline DeviceBuilder & AssignData(std::string DataEntryName, T value) { entryManifests.emplace_back(DataEntryName, std::string(value)); return *this; }

        // int literals -> int32_t
        inline DeviceBuilder& AssignData(std::string DataEntryName, int value) { entryManifests.emplace_back(DataEntryName, static_cast<int32_t>(value)); return *this; }

        // bool explicit overload (prevents bool -> int ambiguity)
        inline DeviceBuilder& AssignData(std::string DataEntryName, bool value) { entryManifests.emplace_back(DataEntryName, value); return *this; }

        inline ManifestBuilder& ExitDeviceBuilder() { return *parentptr; }

        inline std::vector<EntryManifest>& GetEntryManifests() { return entryManifests; }

        inline std::string GetDeviceName() const { return DeviceName; }

    private:
        std::string DeviceName;
        std::vector<EntryManifest> entryManifests;
        ManifestBuilder* parentptr;
    };
} // namespace dlnk