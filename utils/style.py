#!/usr/bin/env python3
"""Format or lint project-owned C++ sources without touching dependencies."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys


def main():
    root = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("format", "check-format", "lint", "check"))
    args = parser.parse_args()
    sources = sorted(
        str(path.relative_to(root))
        for directory in ("cool", "tools")
        for path in (root / directory).rglob("*")
        if path.suffix in (".h", ".cc")
    )
    if args.action in ("format", "check-format", "check"):
        formatter = shutil.which("clang-format")
        if formatter is None:
            parser.error("clang-format is required (configured with version 21).")
        flags = ["-i"] if args.action == "format" else ["--dry-run", "--Werror"]
        subprocess.run([formatter, *flags, *sources], cwd=root, check=True)
    if args.action in ("format", "check-format", "check"):
        cache = Path(os.environ.get("XDG_CACHE_HOME", Path.home() / ".cache"))
        cached_formatter = cache / "coolang-tools/bin/buildifier"
        buildifier = str(cached_formatter) if cached_formatter.is_file() else shutil.which("buildifier")
        if buildifier is None:
            parser.error("buildifier is required for BUILD files; see docs/StyleGuide.md.")
        build_files = ["BUILD", "MODULE.bazel", "third_party/BUILD"]
        for directory in ("bazel", "cool", "tools", "test", "utils"):
            build_files.extend(
                str(path.relative_to(root))
                for path in (root / directory).rglob("*")
                if path.name == "BUILD" or path.suffix == ".bzl"
            )
        mode = "fix" if args.action == "format" else "check"
        subprocess.run([buildifier, "-mode=" + mode, *sorted(build_files)], cwd=root, check=True)
    if args.action in ("lint", "check"):
        cache = Path(os.environ.get("XDG_CACHE_HOME", Path.home() / ".cache"))
        cached_linter = cache / "coolang-tools/style/python/cpplint.py"
        if cached_linter.is_file():
            command = [sys.executable, "-Wignore::DeprecationWarning", str(cached_linter)]
        elif shutil.which("cpplint"):
            command = [shutil.which("cpplint")]
        else:
            parser.error("cpplint is required; see docs/StyleGuide.md for setup.")
        subprocess.run(
            [*command, "--repository=" + str(root.parent), *sources],
            cwd=root, check=True,
        )
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except subprocess.CalledProcessError as error:
        sys.exit(error.returncode)
