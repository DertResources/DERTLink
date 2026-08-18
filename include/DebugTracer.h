#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace dlnk
{

class TracerScopeInfo
{
public:
    TracerScopeInfo(std::string _scopeName, uint32_t _lineNumber, std::string _filename);

    std::string ToString();
private:
    std::string scopeName;
    std::string filename;
    uint32_t lineNumber;
    static std::string rootDirName;
};

class DebugTracer
{
public:
    DebugTracer();

    bool PrintError(std::string errorMessage);

    bool PrintWarning(std::string warningMessage);

    void AddScope(TracerScopeInfo* ptr);
    
    void RemoveLastScope();
    
    static DebugTracer Instance;
private:
    constexpr static uint8_t reservedStackDepth = 10;
    std::vector<TracerScopeInfo*> scopeStack;
};

class TracerScope_Guard
{
public:
    TracerScope_Guard(std::unique_ptr<TracerScopeInfo>& ptr);

    ~TracerScope_Guard();

    TracerScope_Guard(const TracerScope_Guard& other) = delete;
    TracerScope_Guard(TracerScope_Guard&& other) = delete;
    TracerScope_Guard& operator=(const TracerScope_Guard& other)  = delete;
    TracerScope_Guard& operator=(TracerScope_Guard&& other) = delete;
};

#define SCOPE_TRACE(SCOPE_NAME)                                                                                        \
std::unique_ptr<TracerScopeInfo> _localTraceGuard = std::make_unique<TracerScopeInfo>(SCOPE_NAME, __LINE__, __FILE__); \
[[maybe_unused]] TracerScope_Guard _traceGuard(_localTraceGuard)

#define DISPLAY_ERROR(ERROR_MSG) \
dlnk::DebugTracer::Instance.PrintError(ERROR_MSG)


#define DISPLAY_WARNING(WARNING_MSG) \
dlnk::DebugTracer::Instance.PrintWarning(WARNING_MSG)

}; // namespace: dlnk