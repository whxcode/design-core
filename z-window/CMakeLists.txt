# /home/whx/project/design/design-core/z-window/CMakeLists.txt

# 1. 扫描源码
file(GLOB_RECURSE WINDOW_SOURCES "src/*.cpp")
add_library(z_window STATIC ${WINDOW_SOURCES})

# 2. 直接链接（不需要再 add_subdirectory 了，因为顶级 CMake 已经加载过了）
target_link_libraries(z_window PRIVATE z_engine z_matrix z_paint)


