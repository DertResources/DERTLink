#pragma once
#include <string>
#include <iostream>

namespace dlnk
{

inline void print_t(std::string message)
{
    std::cout << message;
}

inline void cry()
{
    print_t("sobs\nT.T\n");
}
}; // namespace: dlnk