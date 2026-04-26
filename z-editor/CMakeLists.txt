add_library(editor_lib STATIC
    src/ui-event/z-ui-handle.cpp
)

target_include_directories(editor_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
