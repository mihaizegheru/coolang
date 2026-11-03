#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/env.sh"
"$PROJECT_ROOT/utils/build.sh" >&2
exec "$PROJECT_ROOT/build/bin/coolc" "$@"
