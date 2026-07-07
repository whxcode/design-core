# z-nodeapi/CMakeLists.txt — Addon 构建入口
# include 核心模块 + Node-API 配置

cmake_minimum_required(VERSION 3.20)
project(DesignCoreAddon LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 1. Node-API 环境配置
# ============================================================
if(NOT DEFINED ADDON_OUTPUT_DIR)
  set(ADDON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../design-desktop-electron/dist/native")
endif()

execute_process(
  COMMAND node -p "process.execPath"
  OUTPUT_VARIABLE NODE_EXECUTABLE_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(NODE_BIN_DIR "${NODE_EXECUTABLE_PATH}" DIRECTORY)
get_filename_component(NODE_ROOT_DIR "${NODE_BIN_DIR}/.." ABSOLUTE)

set(NODE_INCLUDE_DIR "${NODE_ROOT_DIR}/include/node" CACHE PATH "Node or Electron headers directory")

# ============================================================
# 2. 核心模块（include 根 CMakeLists）
#    所有静态库需 -fPIC 才能链接到 .node 共享模块
#    Z_ADDON_MODE 告诉核心代码：渲染走 Electron WebGL，不创建原生 SDL 窗口
# ============================================================
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-DZ_ADDON_MODE)
include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt)

# ============================================================
# 3. Addon 目标
# ============================================================
add_library(addonkit MODULE
  src/ZNodeBinding.cpp
  src/z-node-document-adapter.cpp
)

target_include_directories(addonkit
  PRIVATE
    "${NODE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/node-addon-api"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(addonkit PRIVATE z_app)

set_target_properties(addonkit PROPERTIES
  PREFIX ""
  SUFFIX ".node"
  LIBRARY_OUTPUT_DIRECTORY "${ADDON_OUTPUT_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${ADDON_OUTPUT_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${ADDON_OUTPUT_DIR}"
)

if(APPLE)
  set_target_properties(addonkit PROPERTIES
    LINK_FLAGS "-undefined dynamic_lookup"
  )
elseif(UNIX)
  # 用 pkg-config 获取 GLEW + SDL2 及其全部依赖的静态库
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)
  pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

  target_link_libraries(addonkit PRIVATE PkgConfig::GLEW PkgConfig::SDL2)

  target_link_options(addonkit PRIVATE
    "-Wl,--unresolved-symbols=ignore-all")
endif()
