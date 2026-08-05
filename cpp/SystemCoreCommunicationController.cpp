#include "../include/SystemCoreCommunicationController.h"

namespace dlnk 
{
std::shared_ptr<SCCC> SCCC::Instance = std::make_shared<SCCC>();
};