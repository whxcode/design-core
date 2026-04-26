# app/CMakeLists.txt

add_library(app_lib STATIC src/ZApp.cpp)

target_include_directories(app_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.." # 指向设计核心根目录，方便 #include "window/..."
)

# 建立层级链接
target_link_libraries(app_lib PUBLIC window_lib document_lib editor_lib)
