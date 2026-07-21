#pragma once
#include <deque>
#include <string>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <functional>
#include <any>
#include "../include/InitalizationGroup.h"
#include "../include/SerializeHelper.h"

namespace dlnk
{
class DeviceDictonary; // forward declaration

typedef std::function<void(std::any& objectCursor, std::any parameter)> AccessFunc;

class Device
{
private:
    std::string deviceName;
    std::deque<InitalizationGroup> initalizationGroupVector;
    DeviceDictonary* deviceDictonaryPtr = nullptr;
    std::function<void(std::any& objectCursor, std::any parameter)> createInfoAccessFunc;
public:
    inline Device& SetDeviceName(std::string __deviceName) { deviceName = __deviceName; return *this; }
    void WriteToBuffer(ByteVector& byteBuffer);
    void ReadFromBuffer(const Byte*& cur);
    Device(std::string _deviceName);
    inline void SetDeviceDictonaryPtr(DeviceDictonary& ref)
    {
        deviceDictonaryPtr = &ref;
    }
    
    template <typename... Args>
    inline InitalizationGroup& AddIntializationGroup(Args... _args)
    {
        static_assert(std::is_constructible_v<InitalizationGroup, Args...>,
            "AddIntializationGroup: cannot construct Device from the provided arguments. "
            "Check that the argument types match a Device constructor.");

        initalizationGroupVector.emplace_back(std::forward<Args>(_args)...);
        initalizationGroupVector.back().SetDevicePtr(*this);
        return initalizationGroupVector.back();
    }

    void Print(uint8_t tabs = 0);

    inline DeviceDictonary& ExitDevice()
    {
        return *deviceDictonaryPtr;
    }

    inline std::string GetName()
    {
        return this->deviceName;
    }

    inline std::deque<InitalizationGroup>& GetInitaliztionGroupVector()
    {
        return this->initalizationGroupVector;
    }

    inline void SetAccessFunction(AccessFunc func) { createInfoAccessFunc  = func; }
};

}; // namespace: dlnk
