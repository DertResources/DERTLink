#include "../include/DebugTracer.h"
#include "frc/Errors.h"
#include <wpi/Logger.h>

namespace dlnk
{
    
DebugTracer DebugTracer::Instance;

wpi::Logger DebugTracer::logger;

std::string TracerScopeInfo::rootDirName = "main";

TracerScopeInfo::TracerScopeInfo(std::string _scopeName, uint32_t _lineNumber, std::string _filename)
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

std::string TracerScopeInfo::ToString()
{
    return scopeName + ": line=" + std::to_string(lineNumber) + " File=" + filename;
}

std::string TracerScopeInfo::GetFilename()
{
    return filename;
}

uint32_t TracerScopeInfo::GetLine()
{
    return lineNumber;
}

DebugTracer::DebugTracer()
{
    scopeStack.reserve(reservedStackDepth);
}

bool DebugTracer::PrintError(std::string errorMessage)
{
    std::string message;
    for(TracerScopeInfo* curScope : scopeStack)
    {
        if(curScope)
            message += "> " + curScope->ToString() + '\n';
        else
            return false;
    }
#ifdef _IS_SIMULATION
    std::cout << message + errorMessage << std::endl;
#else
    FRC_ReportError(frc::warn::Warning, "{}", message+errorMessage);
#endif
    // frc::ReportError(frc::err::Error, scopeStack.back()->GetFilename().c_str(), scopeStack.back()->GetLine(), "", "{}", message+errorMessage);
    return true;
}

bool DebugTracer::PrintWarning(std::string warningMessage)
{
    std::string message;
    for(TracerScopeInfo* curScope : scopeStack)
    {
        if(curScope)
            message += "> " + curScope->ToString() + '\n';
        else
            return false;
    }
#ifdef _IS_SIMULATION
    std::cout << message + warningMessage << std::endl;
#else
    FRC_ReportError(frc::warn::Warning, "{}", message);
#endif
    return true;
}

bool DebugTracer::PrintDebug(std::string debugMessage)
{
#ifdef _IS_SIMULATION
    std::cout << debugMessage << std::endl;
#else
    fmt::print("{}", debugMessage);
#endif
    return true;
}

void DebugTracer::AddScope(TracerScopeInfo* ptr)
{
    scopeStack.push_back(ptr);
}

void DebugTracer::RemoveLastScope()
{
    scopeStack.pop_back();
}


TracerScope_Guard::TracerScope_Guard(std::unique_ptr<TracerScopeInfo>& ptr)
{
    DebugTracer::Instance.AddScope(ptr.get());
}

TracerScope_Guard::~TracerScope_Guard()
{
    DebugTracer::Instance.RemoveLastScope();
}

} // namespace: dlnk