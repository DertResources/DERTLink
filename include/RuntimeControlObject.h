#pragma once
#include "../include/DynamicBuffer.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"
#include "../include/DeviceBuilder.h"
#include "../include/AutoGenFile.h"
#include "../include/ManifestBuilder.h"
#include "DERTLink/include/DebugTracer.h"
#include "fmt/base.h"

#include <variant>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <any>
#include <functional>
#include <exception>
#include <type_traits>

/* I'm aware that this entire file is unsafe and frankly a crime.
 * At the same time, I think it works. So good luck!
 *  -Janelyn
 */


namespace dlnk
{

template<typename Func>
void allocate(Func& func, DynamicBuffer& db, DataType dt)
{
    SCOPE_TRACE("dlnk::allocate");
    switch (dt) {
    case DataType::BOOL:
        bool* pBool;
        db.AllocateBool(pBool);
        func(pBool);
        break;
    case DataType::FLOAT:
        float* pFloat;
        db.AllocateFloat(pFloat);
        func(pFloat);
        break;
    case DataType::DOUBLE:
        double* pDouble;
        db.AllocateDouble(pDouble);
        func(pDouble);
        break;
    case DataType::STRING:
        uint8_t vString;
        db.AllocateString(vString);
        func(vString);
        break;
    case DataType::INT8:
        int8_t* pInt8;
        db.AllocateInt8(pInt8);
        func(pInt8);
        break;
    case DataType::INT16:
        int16_t* pInt16;
        db.AllocateInt16(pInt16);
        func(pInt16);
        break;
    case DataType::INT32:
        int32_t* pInt32;
        db.AllocateInt32(pInt32);
        func(pInt32);
        break;
    case DataType::INT64:
        int64_t* pInt64;
        db.AllocateInt64(pInt64);
        func(pInt64);
        break;
    case DataType::UINT8:
        uint8_t* pUInt8;
        db.AllocateUInt8(pUInt8);
        func(pUInt8);
        break;
    case DataType::UINT16:
        uint16_t* pUInt16;
        db.AllocateUInt16(pUInt16);
        func(pUInt16);
        break;
    case DataType::UINT32:
        uint32_t* pUInt32;
        db.AllocateUInt32(pUInt32);
        func(pUInt32);
        break;
    case DataType::UINT64:
        uint64_t* pUInt64;
        db.AllocateUInt64(pUInt64);
        func(pUInt64);
        break;
    default:
        DISPLAY_ERROR("Error: Data Type not found");
    }
}


template <typename Func, typename T>
void input(Func&& func,
        T _key,
        ShortDev& sd,
        DynamicBuffer& dbFeedback,
        DynamicBuffer& dbDesiredState,
        DeviceBuilder& db)
{
    SCOPE_TRACE("dlnk::input");
    if constexpr (!std::is_same<T, const char*>())
    {
        func(_key);
        return;
    } else
    {
        std::string key = _key;

        if (key == "___NO_RETURN_TYPE___")
        {
            func(0);
            return;
        }
        DataEntryPtr dep;
        DeviceDictonary::FindShortDataEntry(sd, dep, key);
        DataDirection dd;
        DataType dt;
        std::visit([&](auto& de) {
            dd = de.GetDataDirection();
            dt = de.GetDataType();
        }, *dep);

        if (dd == DataDirection::INIT)
        {
            // get reference to entry manifest list
            std::vector<EntryManifest>& a = db.GetEntryManifests();
            // search for entry matching key name
            std::vector<EntryManifest>::iterator It = std::find_if(a.begin(), a.end(), [&](EntryManifest& b) {
                return b.entryName == key;
            });
            // Check if search didnt find result
            if(It == a.end())
                DISPLAY_ERROR("Error: Entry manifest name not found in initalization pass");
            // Check if found entry has value, which it should at this point
            if(!(It->entryData.has_value()))
                DISPLAY_ERROR("Error: Entry manifest does not have value at initalization pass when type is INIT");
            std::visit([&func](auto& data){
                func(data);
            }, It->entryData.value());
        }
        else if (dd == DataDirection::FEEDBACK)
        {
            allocate(func, dbFeedback, dt);
        }
        else if (dd == DataDirection::DESIREDSTATE)
        {
            allocate(func, dbDesiredState, dt);
        }
    }
}


template <typename Ret, typename Class, typename Arg>
void RunFunc(std::any& obj, AllTypes arg, Ret (Class::*funcPtr)(Arg))
{
    SCOPE_TRACE("dlnk::RunFunc (one arg)");
    std::visit([&](auto& a){
        using U = std::decay_t<decltype(a)>;
        if constexpr (( std::is_pointer_v<Arg> && !std::is_pointer_v<U>) ||
                      (!std::is_pointer_v<Arg> &&  std::is_pointer_v<U>))
        {
            DISPLAY_ERROR("Buffer type can not be assigned to Init type");
            return;
        }
        
        if constexpr (std::is_same_v<U, Arg>)
        {
            Ret result = std::bind(funcPtr, *(std::any_cast<std::shared_ptr<Class>>(obj).get()), a)();
            obj = std::make_shared<Ret>(std::move(result));
            return;
        }

        if constexpr    (std::is_pointer_v<U>)
        {
            // pointer
            if constexpr (std::is_convertible_v<std::remove_pointer_t<U>, std::remove_pointer_t<Arg>>)
            {
                DISPLAY_ERROR("To be implemented later");
                // TO BE IMPLEMENTED LATER
            } else {
                DISPLAY_ERROR("Types do not match, Pointer types are not convertable");
            }
        } else {
            // not pointer
            if constexpr (std::is_convertible_v<U, Arg>)
            {
                DISPLAY_WARNING("WARNING: Narrowing conversion not caught, implement checks later");
                Ret result = std::bind(funcPtr, 
                                        *(std::any_cast<std::shared_ptr<Class>>(obj).get()), 
                                        static_cast<Arg>(a))();
                obj = std::make_shared<Ret>(std::move(result));
            } else {
                DISPLAY_ERROR("Types do not match, Datatypes are not convertable");
            }
        }
    }, arg);
}

template <typename Ret, typename Class>
void RunFunc(std::any& obj, AllTypes arg, Ret (Class::*funcPtr)())
{
    SCOPE_TRACE("dlnk::RunFunc (no arg)");
    try {
        Ret result = std::bind(funcPtr,
            *(std::any_cast<std::shared_ptr<Class>&>(obj).get()))();
        obj = std::make_shared<std::any>(std::move(result));
    }
    catch (std::exception& e)
    {
        std::cout << "Init Group bound function call failed (no arg)" << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }
}

}; // namespace: dlnk

