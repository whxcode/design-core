# 扫描源码
file(GLOB_RECURSE DOCUMENT_SOURCES "src/*.cpp")

# 创建库
add_library(document_lib STATIC ${DOCUMENT_SOURCES})

# 2. 直接链接（不需要再 add_subdirectory 了，因为顶级 CMake 已经加载过了）
target_link_libraries(document_lib PRIVATE z_tools)

# 【核心修复点】
# 将路径改为当前目录（即 document 的上一层），这样编译器才能看到 document/include/...
target_include_directories(document_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# 应用公共编译参数
target_compile_options(document_lib PRIVATE ${COMMON_COMPILE_OPTIONS})
