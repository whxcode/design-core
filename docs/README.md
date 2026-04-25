# Design Core Docs

开发文档目录：

- `CMakeList.md`：`design-core/CMakeLists.txt` 配置项说明（CMake 指令、Emscripten 参数、常见坑位）。
- `JsCppUint8Transfer.md`：JS 与 C++ 传递 `Uint8Array` 大数据的方法、优缺点、示例代码。

建议后续按主题新增文档，例如：

- `Build.md`：构建命令与产物流转
- `WebApiBinding.md`：Embind 绑定规范
- `ThreadModel.md`：pthread/worker/主线程协作模型
