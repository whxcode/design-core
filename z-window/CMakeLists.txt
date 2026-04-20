# /home/whx/project/design/design-core/z-window/CMakeLists.txt

# 1. 扫描源码
file(GLOB_RECURSE WINDOW_SOURCES "src/*.cpp")
add_library(window_lib STATIC ${WINDOW_SOURCES})

# 2. 直接链接（不需要再 add_subdirectory 了，因为顶级 CMake 已经加载过了）
target_link_libraries(window_lib PRIVATE z_matrix z_paint)

# 3. 指定头文件搜索路径
# 这里直接用 CMAKE_SOURCE_DIR（指向项目根目录），非常稳
target_include_directories(window_lib PRIVATE "${CMAKE_SOURCE_DIR}/sk-matrix")

# 应用公共参数
target_compile_options(window_lib PRIVATE ${COMMON_COMPILE_OPTIONS})
