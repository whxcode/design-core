#!/bin/bash
set -e

CORE_DIR=$(
  cd "$(dirname "${BASH_SOURCE[0]}")/.."
  pwd
)

SKIA_ROOT=${SKIA_ROOT:-"$CORE_DIR/third_party/skia"}
SKIA_BUILD_DIR=${SKIA_BUILD_DIR:-"$SKIA_ROOT/out/design_wasm"}
SKIA_BUILD_TYPE=${SKIA_BUILD_TYPE:-release}
SKIA_FORCE_BUILD=${SKIA_FORCE_BUILD:-0}
SKIA_TARGETS=${SKIA_TARGETS:-"skia"}
SKIA_GN=${SKIA_GN:-"$SKIA_ROOT/bin/gn"}
SKIA_NINJA=${SKIA_NINJA:-"$SKIA_ROOT/third_party/ninja/ninja"}

if [ ! -e "$SKIA_ROOT/.git" ]; then
  echo "Skia submodule is missing. Run:"
  echo "  git submodule update --init --recursive third_party/skia"
  exit 1
fi

if [ "${SKIA_SYNC_DEPS:-0}" = "1" ]; then
  echo "Syncing Skia dependencies..."
  python3 "$SKIA_ROOT/tools/git-sync-deps"
fi

if [ ! -x "$SKIA_GN" ]; then
  "$SKIA_ROOT/bin/fetch-gn"
fi

if [ ! -x "$SKIA_NINJA" ]; then
  "$SKIA_ROOT/bin/fetch-ninja"
fi

if [ ! -x "$SKIA_GN" ]; then
  echo "Unable to find executable gn: $SKIA_GN"
  exit 1
fi

if [ ! -x "$SKIA_NINJA" ]; then
  echo "Unable to find executable ninja: $SKIA_NINJA"
  exit 1
fi

IS_DEBUG=false
IS_OFFICIAL_BUILD=true
if [ "$SKIA_BUILD_TYPE" = "debug" ] || [ "$1" = "debug" ]; then
  IS_DEBUG=true
  IS_OFFICIAL_BUILD=false
fi

pushd "$SKIA_ROOT" >/dev/null

"$SKIA_GN" gen "$SKIA_BUILD_DIR" \
  --args="is_debug=${IS_DEBUG} \
  is_official_build=${IS_OFFICIAL_BUILD} \
  is_component_build=false \
  is_trivial_abi=true \
  target_cpu=\"wasm\" \
  skia_enable_skottie=false \
  skia_enable_svg=false \
  skia_use_expat=false \
  skia_use_jpeg_gainmaps=false \
  skia_use_webgl=true \
  skia_use_webgpu=false \
  skia_use_system_zlib=false \
  skia_use_system_libpng=false \
  skia_use_system_libjpeg_turbo=false \
  skia_use_system_libwebp=false \
  skia_use_system_freetype2=false \
  skia_use_system_harfbuzz=false \
  skia_use_system_icu=false \
  werror=false"

"$SKIA_NINJA" -C "$SKIA_BUILD_DIR" $SKIA_TARGETS

popd >/dev/null

echo "Skia wasm build completed:"
echo "  $SKIA_BUILD_DIR"
