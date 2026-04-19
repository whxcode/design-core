cmake_minimum_required(VERSION 3.10)
project(DesignCoreProject)

# 自动导出编译命令，方便 LSP (clangd) 识别
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# --- 1. 路径定义 ---
set(EM_PATH "/home/whx/emsdk/upstream/emscripten")
set(EM_SYSROOT "${EM_PATH}/cache/sysroot")

# --- 2. 基础编译选项 (公共部分) ---
set(COMMON_COMPILE_OPTIONS "-pthread" "-sUSE_SDL=2")
set(COMMON_LINK_OPTIONS
    "-pthread"
    "-sPTHREAD_POOL_SIZE=10"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sUSE_PTHREADS=1"
    "-sUSE_SDL=2"
    "-sMODULARIZE=1"
    "-sEXPORT_NAME='createCore'"
    "-sFULL_ES3=1"              # 修正：去掉了空格
    "-sUSE_WEBGL2=1"            # 修正：去掉了空格
    "-sMAX_WEBGL_VERSION=2"     # 修正：去掉了空格
    "-sMIN_WEBGL_VERSION=2"     # 修正：去掉了空格
    "--bind"
)

# --- 3. 区分 Debug 和 Release 的标志 ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Configuring for DEBUG mode...")
  # -g: 保留调试符号; -gsource-map: 生成源码映射; -sASSERTIONS=2: 开启更严格的运行时检查
  list(APPEND COMMON_COMPILE_OPTIONS "-g" "-sASSERTIONS=2")
  list(APPEND COMMON_LINK_OPTIONS "-g" "-gsource-map" "-sASSERTIONS=2")
else()
  message(STATUS "Configuring for RELEASE mode...")
  # -O3: 极致优化; --closure 1: 压缩混淆 JS 胶水代码
  list(APPEND COMMON_COMPILE_OPTIONS "-O3")
  list(APPEND COMMON_LINK_OPTIONS "-O3" "--closure=1")
endif()

# --- 4. 添加子模块 (确保它们也遵循 BUILD_TYPE) ---
add_subdirectory(z-matrix)
add_subdirectory(z-engine)
add_subdirectory(z-window)
add_subdirectory(z-document)
add_subdirectory(z-app)
add_subdirectory(z-webapi)

# --- 5. 创建主程序目标 ---
add_executable(DesignCore main.cpp)

# --- 6. 配置包含路径 (解决 LSP 和编译器的路径矛盾) ---
target_include_directories(DesignCore SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    # "${CMAKE_CURRENT_SOURCE_DIR}/z-document/include"
    # "${CMAKE_CURRENT_SOURCE_DIR}/z-window/include"
    "${EM_SYSROOT}/include"
    "${EM_SYSROOT}/include/SDL2"
)

# --- 7. 链接与输出配置 ---
target_link_libraries(DesignCore PRIVATE webapi_lib app_lib engine_lib)

# target_link_libraries(DesignCore PRIVATE document_lib window_lib)
target_compile_options(DesignCore PRIVATE ${COMMON_COMPILE_OPTIONS})

# 强制输出后缀为 .js，Emscripten 会自动生成同名的 .wasm 和 .worker.js
target_link_options(DesignCore PRIVATE ${COMMON_LINK_OPTIONS} "-o" "DesignCore.js")

