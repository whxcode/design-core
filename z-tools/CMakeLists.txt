# z-tools
# file(GLOB_RECURSE Z_SOURCES "src/*.cpp")

# 2. 使用扫描到的变量创建静态库
# add_library(z_tools STATIC ${Z_SOURCES})

# 3. 包含路径（保持你要求的 z-matrix/include/xxx 引用风格）
# target_include_directories(z_tools PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")


# z-tools/CMakeLists.txt
file(GLOB_RECURSE Z_SOURCES "src/*.cpp")

# 统一使用下划线，和你的 z_matrix 保持风格一致
add_library(z_tools STATIC ${Z_SOURCES})

# 包含路径
target_include_directories(z_tools PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
