#!/bin/bash
set -e

usage() {
  echo "用法: bash build.sh <wasm|addon> [debug]"
  echo ""
  echo "  wasm    — 用 Emscripten 编译 wasm 产物，输出到 design-web/public/wasm/"
  echo "  addon   — 用原生编译 Node.js 插件 (addonkit.node)，输出到 design-desktop-electron/dist/native/"
  echo "  debug   — (可选) 调试模式"
  exit 1
}

BUILD_MODE="$1"
[ -z "$BUILD_MODE" ] && usage
shift

CORE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

case "$BUILD_MODE" in
  wasm)
    bash "$CORE_DIR/build_wasm.sh" "$@"
    ;;
  addon)
    bash "$CORE_DIR/build_addon.sh" "$@"
    ;;
  *)
    echo "错误: 未知构建模式 '$BUILD_MODE'"
    usage
    ;;
esac
