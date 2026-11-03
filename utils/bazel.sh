#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/env.sh"
cd "$PROJECT_ROOT"
if [[ -x "$COOL_TOOLS_DIR/bin/bazel" ]]; then
    exec "$COOL_TOOLS_DIR/bin/bazel" "$@"
fi
exec bazel "$@"
