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
    TracerScopeInfo(std::string _scopeName, uint32_t _lineNumber, std::string _filename)
    : scopeName{_scopeName}
    , lineNumber{_lineNumber}
    {
        std::string::size_type pos1 = _filename.find('\\' + rootDirName + '\\');
        if(pos1 != std::string::npos)
        {
            uint32_t startlen = pos1 + rootDirName.size() + 2;
            filename = _filename.substr(startlen, filename.size()-startlen);
            return;
        };

        std::string::size_type pos2 = _filename.find('\\' + rootDirName + '\\');
        if(pos2 != std::string::npos)
        {
            uint32_t startlen = pos2 + rootDirName.size() + 2;
            filename = _filename.substr(startlen, filename.size()-startlen);
            return;
        }

        std::cout << "DEBUG TRACER FAILURE: Root directory not found" << std::endl;
    }

    std::string ToString()
    {
        return scopeName + ": line=" + std::to_string(lineNumber) + " File=" + filename;
    }
private:
    std::string scopeName;
    std::string filename;
    uint32_t lineNumber;
    static std::string rootDirName;
};

class DebugTracer
{
public:
    DebugTracer()
    {
        scopeStack.reserve(reservedStackDepth);
    }
    bool PrintError(std::string errorMessage)
    {
        std::string message;
        for(TracerScopeInfo* curScope : scopeStack)
        {
            if(curScope)
                message += "> " + curScope->ToString() + '\n';
            else
                return false;
        }

        std::cout << message << "\33[91m" << errorMessage << "\33[m" << std::endl;
        return true;
    }
    bool PrintWarning(std::string warningMessage)
    {
        std::string message;
        for(TracerScopeInfo* curScope : scopeStack)
        {
            if(curScope)
                message += "> " + curScope->ToString() + '\n';
            else
                return false;
        }

        std::cout << message << "\33[93m" << warningMessage << "\33[m" << std::endl;
        return true;
    }
    void AddScope(TracerScopeInfo* ptr)
    {
        scopeStack.push_back(ptr);
    }
    void RemoveLastScope()
    {
        scopeStack.pop_back();
    }
    static DebugTracer Instance;
private:
    constexpr static uint8_t reservedStackDepth = 10;
    std::vector<TracerScopeInfo*> scopeStack;
};

class TracerScope_Guard
{
public:
    TracerScope_Guard(std::unique_ptr<TracerScopeInfo>& ptr)
    {
        DebugTracer::Instance.AddScope(ptr.get());
    }

    ~TracerScope_Guard()
    {
        DebugTracer::Instance.RemoveLastScope();
    }

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