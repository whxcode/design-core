# z-engine/CMakeLists.txt
# libs/nanovg 依赖
# src/*.h 我自己的实现
# src/*.cpp 我自己的实现

# 直接显式指定源文件，确保路径正确
# 如果你的文件在 src 目录下，就写 src/Bindings.cpp
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "libs/nanovg/*.cpp"
    "libs/nanovg/*.c"
)

# 如果还是报错，检查这个文件是否存在！
add_library(z_engine STATIC ${ENGINE_SOURCES})

target_include_directories(z_engine
    PUBLIC
        # 这个路径是为了让外部（如 z-draw）能用 #include "z-engine/xxx.h"
        # 假设 z-engine 文件夹在项目根目录
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
    PRIVATE
        # 这个路径是为了让 nanovg.c 内部能直接 #include "nanovg.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/nanovg"
        # 这个路径是为了让你在 src 下的文件能互相引用
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# 4. WASM 特殊链接配置
if(EMSCRIPTEN)
  target_link_options(z_engine PUBLIC "-s FULL_ES3=1")
endif()
