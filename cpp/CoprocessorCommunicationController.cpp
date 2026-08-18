#include "../include/CoprocessorCommunicationController.h"
#include "../include/DebugTracer.h"
namespace dlnk 
{
    std::shared_ptr<CPCC>CPCC::Instance = std::make_shared<CPCC>();
};