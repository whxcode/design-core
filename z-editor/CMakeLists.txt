add_library(editor_lib STATIC
    src/handlers/z-viewport-handler.cpp
    src/z-editor-context.cpp
    src/ui-event/z-ui-common-handle.cpp
    src/ui-event/z-ui-handle.cpp
    src/ui-event/z-ui-handle-event.cpp
)

target_include_directories(editor_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
