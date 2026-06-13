#!/bin/bash
set -e

# --- 1. 路径自动定位 ---
CORE_DIR=$(
  cd "$(dirname "${BASH_SOURCE[0]}")"
  pwd
)
BUILD_DIR="$CORE_DIR/build_wasm"
EM_CACHE_DIR="$BUILD_DIR/.emscripten_cache"

# Web 产物输出路径
WEB_DIST_DIR="$CORE_DIR/../design-web/public/wasm"
WEB_TYPES_DIR="$CORE_DIR/../design-web/src/types/design-core"
CORE_TYPES_DIR="$CORE_DIR/z-types"

# --- 2. 模式判断 ---
# 默认 Release，传入 "debug" 参数则开启调试模式
BUILD_TYPE="Release"
if [ "$1" == "debug" ]; then
  BUILD_TYPE="Debug"
fi

echo ">>>> 模式: ${BUILD_TYPE} | 源码: $CORE_DIR <<<<"

# echo "生成 kiwi schema..."
#bash "$CORE_DIR/z-kiwi/gen.sh"

# --- 4. 生成 TypeScript 类型 ---
node "$CORE_DIR/scripts/gen-ts-types.js"
# 不再暴力删除整个目录，而是清理上次的缓存，这样增量编译会快一点
if [ -d "$BUILD_DIR" ]; then
  echo "清理旧缓存..."
  rm -rf "$BUILD_DIR/CMakeCache.txt"
fi
mkdir -p "$BUILD_DIR"
mkdir -p "$EM_CACHE_DIR"
export EM_CACHE="$EM_CACHE_DIR"

if command -v nproc >/dev/null 2>&1; then
  JOBS=$(nproc)
elif command -v getconf >/dev/null 2>&1; then
  JOBS=$(getconf _NPROCESSORS_ONLN)
else
  JOBS=4
fi

cd "$BUILD_DIR"

# 注入 CMAKE_BUILD_TYPE，并生成 LSP 用的 JSON
emcmake cmake "$CORE_DIR/z-wasmapi" \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 多线程并行编译
echo "编译中..."
make -j"$JOBS"

if [ -d "$WEB_DIST_DIR" ]; then
  echo "检测到前端仓库，正在同步产物..."
  mkdir -p "$WEB_DIST_DIR"

  # 清空前端旧产物，防止残留旧版本的 .map 或 .wasm
  rm -f "$WEB_DIST_DIR"/DesignCore.*

  # 拷贝所有产物 (js, wasm, worker.js, 以及 debug 模式下的 map)
  cp -v "$BUILD_DIR"/DesignCore.* "$WEB_DIST_DIR/"

  # 同步 z-types 到前端类型目录
  if [ -d "$CORE_TYPES_DIR" ]; then
    rm -rf "$WEB_TYPES_DIR"
    mkdir -p "$WEB_TYPES_DIR"
    cp -rv "$CORE_TYPES_DIR"/. "$WEB_TYPES_DIR"/
  fi

  # 同步 z-kiwi TS 运行时和类型到前端 kiwi 目录
  KIWI_JS_SRC="$CORE_DIR/z-kiwi/js"
  KIWI_JS_DST="$CORE_DIR/../design-web/src/kiwi"
  if [ -d "$KIWI_JS_SRC" ]; then
    mkdir -p "$KIWI_JS_DST"
    rm -f "$KIWI_JS_DST"/schema.js "$KIWI_JS_DST"/schema.ts "$KIWI_JS_DST"/schema.d.ts
    cp -v "$KIWI_JS_SRC"/schema.ts "$KIWI_JS_DST"/
    cp -v "$KIWI_JS_SRC"/schema.d.ts "$KIWI_JS_DST"/
    echo "kiwi ts 已同步到 $KIWI_JS_DST"
  fi

  # 更新根目录的 LSP 配置文件
  ln -sf "$BUILD_DIR/compile_commands.json" "$CORE_DIR/compile_commands.json"

  echo "---------------------------------------"
  echo "✅ 构建并同步完成！"
  echo "模式: $BUILD_TYPE"
  echo "产物目录内容:"
  ls -F "$WEB_DIST_DIR"
  if [ -d "$WEB_TYPES_DIR" ]; then
    echo "类型目录内容:"
    ls -F "$WEB_TYPES_DIR"
  fi
else
  echo "⚠️ 未检测到前端目录 $WEB_DIST_DIR，产物保留在 $BUILD_DIR"
fi
