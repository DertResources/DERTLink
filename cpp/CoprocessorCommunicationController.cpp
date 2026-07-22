#include "../include/CoprocessorCommunicationController.h"

namespace dlnk 
{
std::unique_ptr<CPCC>CPCC::Instance = std::make_unique<CPCC>();
};