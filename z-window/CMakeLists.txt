set(WINDOW_SOURCES "src/ZWindow.cpp")
add_library(z_window STATIC ${WINDOW_SOURCES})

target_link_libraries(z_window PRIVATE z_matrix z_paint)

target_include_directories(z_window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
