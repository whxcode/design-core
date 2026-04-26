cmake_minimum_required(VERSION 3.20)
project(DesignCoreAddon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ADDON_OUTPUT_DIR)
  set(ADDON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../design-addon/dist/native")
endif()

execute_process(
  COMMAND node -p "process.execPath"
  OUTPUT_VARIABLE NODE_EXECUTABLE_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(NODE_BIN_DIR "${NODE_EXECUTABLE_PATH}" DIRECTORY)
get_filename_component(NODE_ROOT_DIR "${NODE_BIN_DIR}/.." ABSOLUTE)

set(NODE_INCLUDE_DIR "${NODE_ROOT_DIR}/include/node" CACHE PATH "Node or Electron headers directory")

add_library(addonkit MODULE
  src/addonkit.cpp
)

target_include_directories(addonkit
  PRIVATE
    "${NODE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

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
  target_link_options(addonkit PRIVATE "-Wl,--unresolved-symbols=ignore-all")
endif()
