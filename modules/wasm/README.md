# z-design wasm package

本目录负责 design-core 的 wasm 版本构建。

- `build.sh`：构建 `DesignCore.js` / `DesignCore.wasm`，并组装 npm 包。
- `package/`：npm 包模板。
- `build/`：最终生成的本地 npm 包，包名为 `z-design`。

使用方式：

```bash
cd design-core
bash modules/wasm/build.sh
cd ../design-web
pnpm link ../design-core/modules/wasm/build
```

根目录 `build_wasm.sh` 只作为兼容入口保留，会转发到本脚本。
