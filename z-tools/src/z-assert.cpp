
// 这是一个洋气的 C++20 函数断言
#include "z-tools/include/z-assert.h"

void Z_ASSERT(bool condition, std::string_view message, const std::source_location location) {
    if (!condition) {
        std::cerr << "Z_ASSERT Failed!\n"
                  << "  File:     " << location.file_name() << "\n"
                  << "  Line:     " << location.line() << "\n"
                  << "  Function: " << location.function_name() << "\n"
                  << "  Message:  " << message << std::endl;

        throw std::runtime_error(std::string(message));
    }
}
