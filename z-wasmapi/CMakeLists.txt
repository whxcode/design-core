# wasmapi/CMakeLists.txt

set(WASMAPI_SOURCES
    src/ZBinding.cpp
    src/z-wasm-document-adapter.cpp
)

add_library(z_wasmapi STATIC ${WASMAPI_SOURCES})

target_include_directories(z_wasmapi
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(z_wasmapi PUBLIC z_app z_wasm)
