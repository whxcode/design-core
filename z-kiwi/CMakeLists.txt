# z-kiwi/CMakeLists.txt
file(GLOB_RECURSE KIWI_SOURCES "src/*.cpp")

add_library(z_kiwi STATIC ${KIWI_SOURCES})

target_link_libraries(z_kiwi PUBLIC z_document z_tools z_matrix)

target_include_directories(z_kiwi
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
