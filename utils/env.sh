#!/usr/bin/env bash
PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
COOL_TOOLS_DIR="${XDG_CACHE_HOME:-$HOME/.cache}/coolang-tools"
if [[ -x "$COOL_TOOLS_DIR/jdk/bin/java" ]]; then
    export JAVA_HOME="$COOL_TOOLS_DIR/jdk"
    if [[ ":$PATH:" != *":$JAVA_HOME/bin:"* ]]; then
        export PATH="$JAVA_HOME/bin:$PATH"
    fi
fi
