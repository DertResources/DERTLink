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
    
    void SetDeviceDictonaryPtr(DeviceDictonary& ref);
    
    template <typename... Args>
    InitalizationGroup& AddIntializationGroup(Args... _args);

    void Print(uint8_t tabs = 0);
    
    DeviceDictonary& ExitDevice();
    
    std::string GetName();
    
    std::deque<InitalizationGroup>& GetInitaliztionGroupVector();
    
    void SetAccessFunction(AccessFunc func);
};

}; // namespace: dlnk

#include "../cpp/Device.tpp"