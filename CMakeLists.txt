# 核心模块定义 — 无任何平台相关配置
# 供 z-wasmapi/ 和 z-nodeapi/ 通过 include() 引用
#
# 注意：本文件会被 include() 到平台 CMakeLists 中，
# cmake_minimum_required 和 project() 由上层入口定义

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(Z_CORE_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# --- 共享核心模块（不包含平台相关的 wasm/addon 专用模块） ---
add_subdirectory(${Z_CORE_ROOT}/z-tools z-tools)
add_subdirectory(${Z_CORE_ROOT}/z-matrix z-matrix)
add_subdirectory(${Z_CORE_ROOT}/z-document z-document)
add_subdirectory(${Z_CORE_ROOT}/z-kiwi z-kiwi)
add_subdirectory(${Z_CORE_ROOT}/z-app-service z-app-service)
add_subdirectory(${Z_CORE_ROOT}/z-paint z-paint)
add_subdirectory(${Z_CORE_ROOT}/z-window z-window)
add_subdirectory(${Z_CORE_ROOT}/z-editor z-editor)
add_subdirectory(${Z_CORE_ROOT}/z-app z-app)
