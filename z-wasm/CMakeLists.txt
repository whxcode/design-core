add_library(wasm_lib STATIC
    src/z-js-value.cpp
)

target_include_directories(wasm_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(wasm_lib PUBLIC editor_lib)
