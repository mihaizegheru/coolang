#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/env.sh"
exec "$PROJECT_ROOT/utils/bazel.sh" run //test:check -- "$@"
