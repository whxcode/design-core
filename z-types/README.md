# z-types

本目录维护由 `design-core/z-webapi/src/ZBinding.cpp` 导出的 TypeScript 类型定义。

当前文件：

- `core-api.ts`：`Window`、`Document`、`App`、`getApp`、`createCore` 对应类型。

构建脚本 `build.sh` 会在每次编译后自动同步这些类型到：

- `design-web/src/types/design-core`
