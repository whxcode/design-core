# 1. 递归扫描 src 目录下所有的 .cpp 文件，存入 Z_SOURCES 变量
#
file(GLOB_RECURSE Z_SOURCES "src/*.cpp")

# 2. 使用扫描到的变量创建静态库
add_library(z_editor STATIC ${Z_SOURCES})


target_include_directories(z_editor
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
