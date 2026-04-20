#pragma once
#include <iostream>
#include <source_location>
#include <string>

// 这是一个洋气的 C++20 函数断言
void Z_ASSERT(bool condition, std::string_view message = "",
              const std::source_location location = std::source_location::current());
