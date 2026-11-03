#!/usr/bin/env bash
set -euo pipefail
# Resolve declared data through Bazel's runfiles library.
if [[ -f "${RUNFILES_DIR:-}/bazel_tools/tools/bash/runfiles/runfiles.bash" ]]; then
    source "$RUNFILES_DIR/bazel_tools/tools/bash/runfiles/runfiles.bash"
elif [[ -f "$0.runfiles/bazel_tools/tools/bash/runfiles/runfiles.bash" ]]; then
    source "$0.runfiles/bazel_tools/tools/bash/runfiles/runfiles.bash"
else
    echo 'Bazel runfiles library is unavailable.' >&2
    exit 1
fi
compiler="$(rlocation "$1")"
checker="$(rlocation "$2")"
shift 2
runfiles_root="$(dirname -- "$checker")/.."
output_root="${BUILD_WORKSPACE_DIRECTORY:-${TEST_UNDECLARED_OUTPUTS_DIR:-$PWD}}"
exec python3 "$checker" --compiler "$compiler" \
    --tests "$runfiles_root/checker/tests" \
    --output "$output_root/build/checker-results" "$@"
