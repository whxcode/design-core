#!/bin/bash
set -e

MODULE_DIR=$(
  cd "$(dirname "${BASH_SOURCE[0]}")"
  pwd
)
CORE_DIR=$(
  cd "$MODULE_DIR/../.."
  pwd
)

CMAKE_BUILD_DIR=${WASM_CMAKE_BUILD_DIR:-"$MODULE_DIR/.cmake-build"}
PACKAGE_DIR=${WASM_PACKAGE_DIR:-"$MODULE_DIR/build"}
EM_CACHE_DIR="$CMAKE_BUILD_DIR/.emscripten_cache"
SKIA_BUILD_DIR=${SKIA_BUILD_DIR:-"$CORE_DIR/third_party/skia/out/design_wasm"}
SKIA_EMSDK_DIR="$CORE_DIR/third_party/skia/third_party/externals/emsdk"
EMCMAKE=${EMCMAKE:-"$SKIA_EMSDK_DIR/upstream/emscripten/emcmake"}
CORE_TYPES_DIR="$CORE_DIR/z-types"
PACKAGE_TEMPLATE_DIR="$MODULE_DIR/package"
LEGACY_CMAKE_BUILD_DIR="$CORE_DIR/build_wasm"

BUILD_TYPE="Release"
BUILD_SKIA_WASM=${BUILD_SKIA_WASM:-1}
SKIA_SYNC_DEPS=${SKIA_SYNC_DEPS:-0}
SKIA_FORCE_BUILD=${SKIA_FORCE_BUILD:-0}
PACKAGE_ONLY=0

package_wasm() {
  local artifact_dir="$1"

  if [ ! -f "$artifact_dir/DesignCore.js" ] || [ ! -f "$artifact_dir/DesignCore.wasm" ]; then
    echo "找不到 wasm 产物: $artifact_dir/DesignCore.js 或 $artifact_dir/DesignCore.wasm"
    exit 1
  fi

  echo "组装 z-design npm 包..."
  rm -rf "$PACKAGE_DIR"
  mkdir -p "$PACKAGE_DIR"

  cp -v "$PACKAGE_TEMPLATE_DIR"/package.json "$PACKAGE_DIR"/
  cp -v "$PACKAGE_TEMPLATE_DIR"/index.cjs "$PACKAGE_DIR"/
  cp -v "$PACKAGE_TEMPLATE_DIR"/index.d.ts "$PACKAGE_DIR"/
  cp -v "$PACKAGE_TEMPLATE_DIR"/index.mjs "$PACKAGE_DIR"/
  cp -v "$PACKAGE_TEMPLATE_DIR"/index.ts "$PACKAGE_DIR"/
  cp -v "$artifact_dir"/DesignCore.* "$PACKAGE_DIR"/

  if [ "$BUILD_SKIA_WASM" = "1" ] && [ ! -f "$SKIA_BUILD_DIR/libskia.wasm.a" ]; then
    echo "⚠️ 未找到 Skia wasm 静态库: $SKIA_BUILD_DIR/libskia.wasm.a"
  fi

  if [ -d "$CORE_TYPES_DIR" ]; then
    mkdir -p "$PACKAGE_DIR/types"
    cp -rv "$CORE_TYPES_DIR"/. "$PACKAGE_DIR/types"/
  fi

  if [ -f "$artifact_dir/compile_commands.json" ]; then
    ln -sf "$artifact_dir/compile_commands.json" "$CORE_DIR/compile_commands.json"
  fi

  echo "---------------------------------------"
  echo "✅ wasm npm 包构建完成"
  echo "模式: $BUILD_TYPE"
  echo "包目录: $PACKAGE_DIR"
  echo "包内容:"
  ls -F "$PACKAGE_DIR"
}

for arg in "$@"; do
  case "$arg" in
    debug)
      BUILD_TYPE="Debug"
      ;;
    --with-skia)
      BUILD_SKIA_WASM=1
      ;;
    --skip-skia)
      BUILD_SKIA_WASM=0
      ;;
    --force-skia)
      BUILD_SKIA_WASM=1
      SKIA_FORCE_BUILD=1
      ;;
    --skip-skia-deps)
      SKIA_SYNC_DEPS=0
      ;;
    --sync-skia-deps)
      SKIA_SYNC_DEPS=1
      ;;
    --package-only)
      PACKAGE_ONLY=1
      ;;
    --help)
      echo "Usage: bash modules/wasm/build.sh [debug] [--skip-skia] [--force-skia] [--sync-skia-deps] [--package-only]"
      echo ""
      echo "Default:"
      echo "  Build Skia incrementally when needed, build DesignCore.wasm, then assemble z-design package."
      echo ""
      echo "Environment:"
      echo "  BUILD_SKIA_WASM=0       Skip Skia build step."
      echo "  SKIA_SYNC_DEPS=1        Run Skia tools/git-sync-deps before building Skia."
      echo "  SKIA_FORCE_BUILD=1      Force Skia gn/ninja even when skip checks would allow reuse."
      echo "  SKIA_BUILD_DIR=...      Override Skia build output directory."
      echo "  SKIA_TARGETS=...        Override Skia ninja targets."
      echo "  EMCMAKE=...             Override emcmake. Default uses Skia's vendored emsdk."
      echo "  WASM_CMAKE_BUILD_DIR=... Override CMake build directory."
      echo "  WASM_PACKAGE_DIR=...    Override final npm package directory."
      echo "  --package-only          Assemble npm package from existing DesignCore artifacts."
      exit 0
      ;;
    *)
      echo "Unknown argument: $arg"
      echo "Run: bash modules/wasm/build.sh --help"
      exit 1
      ;;
  esac
done

echo ">>>> wasm 模式: ${BUILD_TYPE} | 源码: $CORE_DIR <<<<"

if [ "$PACKAGE_ONLY" = "1" ]; then
  node "$CORE_DIR/scripts/gen-ts-types.js"
  if [ -f "$CMAKE_BUILD_DIR/DesignCore.js" ]; then
    package_wasm "$CMAKE_BUILD_DIR"
  else
    package_wasm "$LEGACY_CMAKE_BUILD_DIR"
  fi
  exit 0
fi

if [ "$BUILD_SKIA_WASM" = "1" ]; then
  echo "构建 Skia wasm 静态库..."
  SKIA_BUILD_DIR="$SKIA_BUILD_DIR" \
    SKIA_SYNC_DEPS="$SKIA_SYNC_DEPS" \
    SKIA_FORCE_BUILD="$SKIA_FORCE_BUILD" \
    bash "$CORE_DIR/scripts/build_skia_wasm.sh" "$([ "$BUILD_TYPE" = "Debug" ] && echo debug)"
fi

node "$CORE_DIR/scripts/gen-ts-types.js"

if [ -d "$CMAKE_BUILD_DIR" ]; then
  echo "清理旧 CMake 缓存..."
  rm -f "$CMAKE_BUILD_DIR/CMakeCache.txt"
fi

mkdir -p "$CMAKE_BUILD_DIR"
mkdir -p "$EM_CACHE_DIR"
export EM_CACHE="$EM_CACHE_DIR"

if command -v nproc >/dev/null 2>&1; then
  JOBS=$(nproc)
elif command -v getconf >/dev/null 2>&1; then
  JOBS=$(getconf _NPROCESSORS_ONLN)
else
  JOBS=4
fi

cd "$CMAKE_BUILD_DIR"

if [ ! -x "$EMCMAKE" ]; then
  echo "Unable to find emcmake: $EMCMAKE"
  exit 1
fi

"$EMCMAKE" cmake "$CORE_DIR/z-wasmapi" \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "编译 wasm..."
make -j"$JOBS"

package_wasm "$CMAKE_BUILD_DIR"
