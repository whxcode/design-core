file(GLOB_RECURSE APP_SERVICE_SOURCES "src/*.cpp")

add_library(z_app_service STATIC ${APP_SERVICE_SOURCES})

target_include_directories(z_app_service
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(z_app_service PUBLIC z_document z_kiwi)
