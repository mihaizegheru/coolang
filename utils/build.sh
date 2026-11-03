#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/env.sh"
"$PROJECT_ROOT/utils/bazel.sh" build //:coolc
mkdir -p "$PROJECT_ROOT/build/bin"
ln -sfn "$PROJECT_ROOT/bazel-bin/tools/coolc/coolc" "$PROJECT_ROOT/build/bin/coolc"
python3 "$PROJECT_ROOT/utils/compile_commands.py"
