#!/usr/bin/env python3
"""Export Bazel's real C++ action arguments for editor tooling."""
import json
from pathlib import Path
import subprocess


def main():
    root = Path(__file__).resolve().parent.parent
    bazel = str(root / "utils/bazel.sh")
    execution_root = Path(subprocess.check_output(
        [bazel, "info", "execution_root"], text=True, cwd=root,
    ).strip())
    query = 'mnemonic("CppCompile", deps(//tools/coolc:coolc))'
    graph = json.loads(subprocess.check_output(
        [bazel, "aquery", query, "--output=jsonproto", "--include_commandline"],
        text=True, cwd=root,
    ))
    commands = []
    for action in graph.get("actions", []):
        arguments = action.get("arguments", [])
        if "-c" not in arguments:
            continue
        source = arguments[arguments.index("-c") + 1]
        workspace_source = root / source
        file = workspace_source if workspace_source.is_file() else execution_root / source
        commands.append({
            "directory": str(execution_root),
            "file": str(file),
            "arguments": arguments,
        })
    output = root / "build/compile_commands.json"
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(commands, indent=2) + "\n", encoding="utf-8")
    print(f"Exported {len(commands)} C++ compile commands to {output}")


if __name__ == "__main__":
    main()
