# z-types

本目录维护由 `design-core/z-wasmapi/src/ZBinding.cpp` 导出的 TypeScript 类型定义。

当前文件：

- `core-api.ts`：`Window`、`Document`、`App`、`getApp`、`createCore` 对应类型。

wasm 构建脚本会把这些类型打进本地 npm 包：

- `design-core/modules/wasm/build/types`
