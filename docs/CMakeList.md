# Design Core CMakeLists 说明

本文档解释 `design-core/CMakeLists.txt` 中各个关键配置项的作用，方便后续维护 wasm 构建链路。

## 1. 顶层项目声明

- `cmake_minimum_required(VERSION 3.10)`
  - 指定本项目最低需要 CMake 3.10，低于该版本会直接报错。
- `project(DesignCoreProject)`
  - 定义项目名，CMake 会基于这个名字组织构建目标和变量前缀。

## 2. C++ 标准配置

- `set(CMAKE_CXX_STANDARD 23)`
  - 指定使用 C++23 编译标准。
- `set(CMAKE_CXX_STANDARD_REQUIRED ON)`
  - 强制必须使用 C++23；如果编译器不支持则配置失败，不会静默降级到更低标准。

## 3. Emscripten 路径变量

- `set(EM_PATH ".../emscripten")`
  - Emscripten 工具链根目录。
- `set(EM_SYSROOT "${EM_PATH}/cache/sysroot")`
  - Emscripten sysroot 路径，包含 libc/SDL 等头文件与库。

## 4. 公共编译参数 `COMMON_COMPILE_OPTIONS`

- `-pthread`
  - 启用线程相关编译支持（对应 wasm + SharedArrayBuffer 场景）。
- `-sUSE_SDL=2`
  - 启用 SDL2 支持。

## 5. 公共链接参数 `COMMON_LINK_OPTIONS`

- `-pthread`
  - 链接阶段也启用线程支持。
- `-sPTHREAD_POOL_SIZE=10`
  - 预创建 10 个 pthread worker，减少运行时创建线程开销。
- `-sALLOW_MEMORY_GROWTH=1`
  - 允许 wasm 内存动态增长，避免固定内存不足直接崩溃。
- `-sUSE_PTHREADS=1`
  - 打开 Emscripten pthread 支持。
- `-sUSE_SDL=2`
  - 链接 SDL2 运行时。
- `-sMODULARIZE=1`
  - 生成工厂函数模式（不是全局 `Module` 单例），前端通过 `createCore({...})` 初始化实例。
- `-sEXPORT_NAME='createCore'`
  - 指定导出的工厂函数名为 `createCore`。
- `-sFULL_ES3=1`
  - 生成完整 ES3 兼容路径（主要是兼容策略）。
- `-sUSE_WEBGL2=1`
  - 启用 WebGL2。
- `-sMAX_WEBGL_VERSION=2`
  - 允许的最高 WebGL 版本为 2。
- `-sMIN_WEBGL_VERSION=2`
  - 要求最低 WebGL 版本为 2（即强制走 WebGL2）。
- `--bind`
  - 启用 Embind，支持 C++ 类/函数绑定到 JS。

## 6. wasm 导出参数 `WASM_EXPORT_FLAGS`

- `-sEXPORTED_FUNCTIONS=_malloc,_free,_main`
  - 声明 C 符号导出白名单。
  - `_malloc`/`_free`：前端可直接分配/释放 wasm 堆内存。
  - `_main`：允许运行时自动执行 `main()`（当 `noInitialRun: false` 时非常关键）。
  - 注意：设置了该项后，未列入的符号可能被裁剪。
- `-sEXPORTED_RUNTIME_METHODS=HEAPU8`
  - 导出 JS 运行时方法/对象，这里导出 `HEAPU8` 供前端直接访问 wasm 内存视图。

## 7. Debug / Release 分支

- `if(CMAKE_BUILD_TYPE STREQUAL "Debug")`
  - Debug 构建。
- `message(STATUS "Configuring for DEBUG mode...")`
  - 配置阶段打印当前模式。
- `list(APPEND COMMON_COMPILE_OPTIONS "-g" "-sASSERTIONS=2")`
  - 编译时增加调试符号与更严格断言。
- `list(APPEND COMMON_LINK_OPTIONS "-g" "-gsource-map" "-sASSERTIONS=2")`
  - 链接时生成 source map 并启用断言。
- `else()` / Release 分支
  - `-O3`：高优化。
  - `--closure=1`：对胶水 JS 做 Closure 压缩（更小但调试更难）。

## 8. 全局参数注入

- `add_compile_options(${COMMON_COMPILE_OPTIONS})`
  - 将公共编译参数应用到当前目录及子目录 target。
- `add_link_options(${COMMON_LINK_OPTIONS})`
  - 将公共链接参数应用到当前目录及子目录 target。

## 9. 子模块接入

- `add_subdirectory(...)`
  - 把 `z-tools`、`z-matrix`、`z-engine`、`z-paint`、`z-window`、`z-document`、`z-app`、`z-webapi` 纳入同一构建图。

## 10. 主目标与包含路径

- `add_executable(DesignCore main.cpp)`
  - 创建主可执行目标（Emscripten 下产物是 `DesignCore.js/.wasm`）。
- `target_include_directories(DesignCore SYSTEM PRIVATE ...)`
  - 给 `DesignCore` 增加头文件搜索路径。
  - `SYSTEM` 会把这些目录当作系统头，减少第三方头告警噪音。
  - `PRIVATE` 表示仅当前目标使用，不向依赖传播。

## 11. 链接和输出

- `target_link_libraries(DesignCore PRIVATE webapi_lib app_lib engine_lib)`
  - 将主程序与各静态库链接。
- `target_compile_options(DesignCore PRIVATE ${COMMON_COMPILE_OPTIONS})`
  - 给 `DesignCore` 再追加一次编译参数（和全局参数叠加）。
- `target_link_options(DesignCore PRIVATE ${COMMON_LINK_OPTIONS} ${WASM_EXPORT_FLAGS} "-o" "DesignCore.js")`
  - 给 `DesignCore` 追加最终链接参数。
  - `-o DesignCore.js` 指定输出入口 JS 文件名，Emscripten 会同时生成同名 wasm。

## 12. 常见坑位

- 若设置了 `EXPORTED_FUNCTIONS`，请确保 `_main` 是否符合当前启动策略：
  - 需要自动跑 `main`：保留 `_main`。
  - 不需要自动跑：可移除 `_main`，并在前端改为 `noInitialRun: true` + 手动入口。
- 线程模式要求前端服务返回：
  - `Cross-Origin-Opener-Policy: same-origin`
  - `Cross-Origin-Embedder-Policy: require-corp`
  否则 `SharedArrayBuffer` 不可用，pthread 初始化会失败。
