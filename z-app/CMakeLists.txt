# app/CMakeLists.txt

add_library(z_app STATIC src/ZApp.cpp)

target_include_directories(z_app
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.." # 指向设计核心根目录，方便 #include "window/..."
)

# 建立层级链接
target_link_libraries(z_app PUBLIC z_window z_document z_app_service z_editor z_paint)
