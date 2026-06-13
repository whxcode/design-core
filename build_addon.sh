#!/bin/bash
set -e

# build_addon.sh — 用原生编译 Node.js 插件 (addonkit.node)
#
# 用法: bash build_addon.sh [debug]
#   debug: 调试模式（默认 Release）

CORE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ADDON_SRC_DIR="$CORE_DIR/z-nodeapi"
BUILD_DIR="${ADDON_SRC_DIR}/build"
ADDON_OUTPUT_DIR="$CORE_DIR/../design-desktop-electron/dist/native"

BUILD_TYPE="Release"
if [ "$1" == "debug" ]; then
  BUILD_TYPE="Debug"
fi

echo ">>>> ADDON ${BUILD_TYPE} | 源码: $ADDON_SRC_DIR <<<<"

mkdir -p "$BUILD_DIR"
mkdir -p "$ADDON_OUTPUT_DIR"

if command -v nproc >/dev/null 2>&1; then
  JOBS=$(nproc)
elif command -v getconf >/dev/null 2>&1; then
  JOBS=$(getconf _NPROCESSORS_ONLN)
else
  JOBS=4
fi

cd "$BUILD_DIR"

cmake "$ADDON_SRC_DIR" \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DADDON_OUTPUT_DIR="$ADDON_OUTPUT_DIR"

echo "编译中..."
make -j"$JOBS"

# 更新 LSP compile_commands.json 链接
ln -sf "$BUILD_DIR/compile_commands.json" "$CORE_DIR/compile_commands.json"

echo "---------------------------------------"
echo "✅ ADDON 构建完成！"
echo "模式: $BUILD_TYPE"
ls -lh "$ADDON_OUTPUT_DIR"/*.node 2>/dev/null && echo "产物: $ADDON_OUTPUT_DIR/addonkit.node" || echo "⚠️ 未找到 .node 文件"
