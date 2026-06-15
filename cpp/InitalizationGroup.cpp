#include "../include/InitalizationGroup.h"

namespace dlnk
{

void InitalizationGroup::Print(uint8_t tabs)
{
    std::string tabString = std::string(tabs * 2, ' ');
    std::cout << tabString << "\033[96m" << "Initalization Group: \"" << GroupName << "\"" << std::endl;
    std::cout << tabString << "{" << "\033[m" << std::endl;
    for (DataEntryVariant& de : GroupDataEntries)
    {
        std::visit(
        [=](auto& a) {
            a.Print(tabs + 1);
        }, de);
    }
    std::cout << tabString << "\033[96m" << "}" << "\033[m" << std::endl;
}

}; // namespace: dlnk