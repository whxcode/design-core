# Skia 接入记录

## 目标

- 使用 Google 官方 Skia 仓库作为 `design-core` 的绘制基础。
- 第一阶段优先处理 wasm 环境下的 Skia 静态库构建，并后续链接进 `DesignCore.wasm`。
- 后续再处理 addon/native 环境，不在第一阶段混入 Electron 原生窗口或 GPU 共享纹理问题。

## Submodule

```bash
git submodule update --init --recursive third_party/skia
```

当前 Skia 路径：

```text
third_party/skia
```

## Wasm 构建

推荐通过 `build_wasm.sh` 统一入口触发，先构建 Skia wasm 静态库，再构建业务 wasm：

```bash
bash build_wasm.sh --with-skia
```

默认不会同步 Skia 依赖。首次拉取或更新 Skia 依赖时，显式执行：

```bash
bash build_wasm.sh --with-skia --sync-skia-deps
```

强制重编 Skia：

```bash
bash build_wasm.sh --force-skia
```

平时只改 `design-core` 业务代码时，可以继续使用：

```bash
bash build_wasm.sh
```

如果 `third_party/skia/out/design_wasm/libskia.a` 已存在，默认不会重新构建 Skia；使用 `--force-skia` 可强制重编。

也可以单独构建 Skia：

```bash
bash scripts/build_skia_wasm.sh
```

首次构建会同步 Skia 依赖并下载 `gn` / `ninja`，耗时较长。

跳过依赖同步：

```bash
SKIA_SYNC_DEPS=0 bash scripts/build_skia_wasm.sh
```

Debug 构建：

```bash
bash scripts/build_skia_wasm.sh debug
```

可配置环境变量：

```bash
BUILD_SKIA_WASM=1      # 在 build_wasm.sh 中启用 Skia 构建
SKIA_SYNC_DEPS=1       # 执行 tools/git-sync-deps
SKIA_FORCE_BUILD=1     # 即使已有 libskia.a 也重编
SKIA_BUILD_DIR=...     # 覆盖 Skia 构建输出目录
SKIA_TARGETS=...       # 覆盖 ninja 目标
```

## 迁移方向

- 当前 `ZVgEngine` / `IZEngine` 是 NanoVG 风格的自研抽象。
- Skia 迁移后，绘制代码应逐步直接面向 `SkCanvas`、`SkPaint`、`SkPath`。
- `ZWindow` / surface 层负责提供 `SkSurface` 或 `SkCanvas`，业务 painter 负责具体绘制。

## 暂不处理

- addon 原生渲染窗口。
- Electron DOM canvas 与 native GPU texture 的 0 拷贝共享。
- 全量 painter 一次性迁移。
