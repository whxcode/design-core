add_library(z_wasm STATIC
    src/z-js-value.cpp
)

target_include_directories(z_wasm
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(z_wasm PUBLIC z_editor)
