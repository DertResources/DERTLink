#include "../include/SystemCoreCommunicationController.h"

namespace dlnk 
{
std::unique_ptr<SCCC> SCCC::Instance = std::make_unique<SCCC>();
};