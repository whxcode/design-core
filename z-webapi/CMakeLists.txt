# webapi/CMakeLists.txt

# 直接显式指定源文件，确保路径正确
# 如果你的文件在 src 目录下，就写 src/Bindings.cpp
set(WEBAPI_SOURCES
    src/ZBinding.cpp
)

# 如果还是报错，检查这个文件是否存在！
add_library(z_webapi STATIC ${WEBAPI_SOURCES})

# 路径往上提一级，方便 #include "app/include/App.h"
target_include_directories(z_webapi
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(z_webapi PUBLIC z_wasm)
