# z-wasmapi/CMakeLists.txt — Wasm 构建入口
# include 核心模块 + wasm 专用配置

cmake_minimum_required(VERSION 3.10)
project(DesignCoreWasm LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 1. Wasm 环境配置
# ============================================================

# 路径定义
if(CMAKE_SYSROOT)
  set(EM_SYSROOT "${CMAKE_SYSROOT}")
elseif(DEFINED ENV{EM_CACHE})
  set(EM_SYSROOT "$ENV{EM_CACHE}/sysroot")
else()
  message(FATAL_ERROR "Unable to resolve Emscripten sysroot. Run this project through emcmake.")
endif()

# 基础编译选项
set(COMMON_COMPILE_OPTIONS
    "-pthread"
)

set(COMMON_LINK_OPTIONS
    "-pthread"
    "-sPTHREAD_POOL_SIZE=10"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sUSE_PTHREADS=1"
    "-sMODULARIZE=1"
    "-sEXPORT_NAME='createCore'"
    "-sFULL_ES3=1"
    "-sUSE_WEBGL2=1"
    "-sMAX_WEBGL_VERSION=2"
    "-sMIN_WEBGL_VERSION=2"
    "--bind"
)

set(SKIA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/skia")
set(SKIA_BUILD_DIR "$ENV{SKIA_BUILD_DIR}")
if(SKIA_BUILD_DIR STREQUAL "")
  set(SKIA_BUILD_DIR "${SKIA_ROOT}/out/design_wasm")
endif()

set(SKIA_WASM_LIBS
    "${SKIA_BUILD_DIR}/libskia.wasm.a"
    "${SKIA_BUILD_DIR}/libskcms.wasm.a"
    "${SKIA_BUILD_DIR}/libpng.wasm.a"
    "${SKIA_BUILD_DIR}/libjpeg.wasm.a"
    "${SKIA_BUILD_DIR}/libwebp.wasm.a"
    "${SKIA_BUILD_DIR}/libwuffs.wasm.a"
    "${SKIA_BUILD_DIR}/libzlib.wasm.a"
    "${SKIA_BUILD_DIR}/libraw_ptr.wasm.a"
    "${SKIA_BUILD_DIR}/liballocator_base.wasm.a"
    "${SKIA_BUILD_DIR}/liballocator_core.wasm.a"
    "${SKIA_BUILD_DIR}/liballocator_shim.wasm.a"
)

set(SKIA_COMPILE_DEFINITIONS
    "SK_TRIVIAL_ABI=[[clang::trivial_abi]]"
)

set(WASM_EXPORT_FLAGS
    "-sEXPORTED_FUNCTIONS=_malloc,_free,_main"
    "-sEXPORTED_RUNTIME_METHODS=HEAPU8"
)

# Debug/Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Configuring for DEBUG mode...")
  list(APPEND COMMON_COMPILE_OPTIONS "-g" "-sASSERTIONS=2")
  list(APPEND COMMON_LINK_OPTIONS "-g" "-gsource-map" "-sASSERTIONS=2")
else()
  message(STATUS "Configuring for RELEASE mode...")
  list(APPEND COMMON_COMPILE_OPTIONS "-O3")
  list(APPEND COMMON_LINK_OPTIONS "-O3" "--closure=1")
endif()

add_compile_options(${COMMON_COMPILE_OPTIONS})
add_link_options(${COMMON_LINK_OPTIONS})

# ============================================================
# 2. 核心模块（include 根 CMakeLists）
# ============================================================
include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

target_compile_definitions(z_paint PUBLIC Z_ENABLE_SKIA=1)
target_compile_definitions(z_window PUBLIC Z_ENABLE_SKIA=1)
target_compile_definitions(z_paint PUBLIC ${SKIA_COMPILE_DEFINITIONS})
target_compile_definitions(z_window PUBLIC ${SKIA_COMPILE_DEFINITIONS})

target_include_directories(z_paint SYSTEM PUBLIC "${SKIA_ROOT}")
target_include_directories(z_window SYSTEM PUBLIC "${SKIA_ROOT}")

# ============================================================
# 3. Wasm 专用模块
# ============================================================
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../z-wasm z-wasm)

# z_wasmapi
set(WASMAPI_SOURCES
    src/ZBinding.cpp
    src/z-wasm-document-adapter.cpp
)

add_library(z_wasmapi STATIC ${WASMAPI_SOURCES})

target_compile_definitions(z_wasmapi PUBLIC ${SKIA_COMPILE_DEFINITIONS})

target_include_directories(z_wasmapi
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(z_wasmapi PUBLIC z_app z_wasm)

# ============================================================
# 4. 主程序
# ============================================================
add_executable(DesignCore ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp)

target_compile_definitions(DesignCore PRIVATE ${SKIA_COMPILE_DEFINITIONS})

target_include_directories(DesignCore SYSTEM PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${SKIA_ROOT}"
    "${EM_SYSROOT}/include"
)

target_link_libraries(DesignCore PRIVATE z_wasmapi z_app ${SKIA_WASM_LIBS})

target_compile_options(DesignCore PRIVATE ${COMMON_COMPILE_OPTIONS})

target_link_options(DesignCore PRIVATE
    ${COMMON_LINK_OPTIONS}
    ${WASM_EXPORT_FLAGS}
    "-o" "DesignCore.js"
)
