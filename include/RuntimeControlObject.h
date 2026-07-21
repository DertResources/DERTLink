#pragma once
#include "../include/DynamicBuffer.h"
#include "../include/DeviceDictonary.h"
#include "../include/DataEntry.h"
#include "../include/DeviceBuilder.h"
#include "../../../../../Second_Process/Second_Process/AutoGenFile.h"

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
        uint8_t* pString;
        db.AllocateString(pString);
        func(pString);
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
        std::cout << "Error: Data Type not found" << std::endl;
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
                std::cout << "Error: Entry manifest name not found in initalization pass"
                          << std::endl;
            // Check if found entry has value, which it should at this point
            if(!(It->entryData.has_value()))
                std::cout << "Error: Entry manifest does not have value at initalization pass when type is INIT"
                          << std::endl;
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
void RunFunc(std::any& obj, std::any arg, Ret (Class::*funcPtr)(Arg))
{
    if (obj.type() != typeid(Class))
        std::cout << "Init Group bound function class cast failed" << std::endl;
    if (arg.type() != typeid(Arg))
        std::cout << "Init Group bound function argment cast failed" << std::endl;

    try {
        obj = std::bind(funcPtr, std::any_cast<Class&>(obj), std::any_cast<Arg>(arg))();
    } catch (std::exception e) {
        std::cout << "Init Group bound function call failed" << std::endl;
    }
}
template <typename Ret, typename Class>
void RunFunc(std::any& obj, std::any arg, Ret (Class::*funcPtr)())
{
    if (obj.type() != typeid(Class))
        std::cout << "Init Group bound function class cast failed" << std::endl;

    try {
        obj = std::bind(funcPtr, std::any_cast<Class&>(obj))();
    } catch (std::exception e) {
        std::cout << "Init Group bound function call failed" << std::endl;
    }
}

}; // namespace: dlnk












// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

//template <typename Ret, typename Arg>
//Arg first_arg_type(Ret (*)(Arg)); // helper: deduces the single parameter type of a free function
//
//#define CONSTRUCT_FUNC_WRAPPPER(funcName)                                                                          \
//    [](auto&& arg) {                                                                                               \
//        using ArgT = std::decay_t<decltype(arg)>;                                                                  \
//        using ParamT = decltype(first_arg_type(funcName));                                                         \
//        if constexpr (std::is_same_v<ArgT, ParamT>) {                                                              \
//            funcName(std::forward<decltype(arg)>(arg));                                                            \
//        } else {                                                                                                   \
//            std::cout << "Error: " #funcName " not callable with this type (got " << typeid(ArgT).name() << ")\n"; \
//        }                                                                                                          \
//    };
//
//
//#define EMPLACE_DATA(funcName, stringKey)                 \
//    {                                                     \
//        auto wrapper = CONSTRUCT_FUNC_WRAPPPER(funcName); \
//        input(wrapper, stringKey, sd, dbf, dbd, db);      \
//    }
//
//#define INIT_CMD [](ShortDev& sd, DynamicBuffer& dbf, DynamicBuffer& dbd, DeviceBuilder& db)










/*
 * 
 * DataEntryPtr dep;
                DeviceDictonary::FindShortDataEntry(sd, dep, "DisplayName");
                DataDirection dd;
                DataType dt;
                std::visit([&](auto& de) {
                    dd = de.GetDataDirection();
                    dt = de.GetDataType();
                },
                    *dep);

                if (dd == DataDirection::INIT) {
                    // get reference to entry manifest list
                    std::vector<EntryManifest>& a = db.GetEntryManifests();
                    // search for entry matching key name
                    std::vector<EntryManifest>::iterator It = std::find_if(a.begin(), a.end(), [&](EntryManifest& b) {
                        return b.entryName == "DisplayName";
                    });
                    // Check if search didnt find result
                    if (It == a.end())
                        std::cout << "Error: Entry manifest name not found in initalization pass"
                                  << std::endl;
                    // Check if found entry has value, which it should at this point
                    if (!(It->entryData.has_value()))
                        std::cout << "Error: Entry manifest does not have value at initalization pass when type is INIT"
                                  << std::endl;
                    std::visit([&wrapper](auto& data) {
                        wrapper(data);
                    },
                        It->entryData.value());

                } else if (dd == DataDirection::FEEDBACK) {
                    allocate(wrapper, dbf, dt);
                } else if (dd == DataDirection::DESIREDSTATE) {
                    allocate(wrapper, dbd, dt);
                }.
 */





