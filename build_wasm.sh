#!/bin/bash
set -e

CORE_DIR=$(
  cd "$(dirname "${BASH_SOURCE[0]}")"
  pwd
)

exec bash "$CORE_DIR/modules/wasm/build.sh" "$@"
