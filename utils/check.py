#!/usr/bin/env python3
"""Compare the native frontend output with regression fixtures."""
import argparse
from pathlib import Path
import re
import subprocess
import sys


def checker_lines(data):
    """Match LineNumberReader: CR, LF and CRLF end lines; other controls do not."""
    lines = re.split(r"\r\n|[\r\n]", data.decode("utf-8", errors="replace"))
    if lines[-1] == "":
        lines.pop()
    return lines


def mismatch(actual, reference):
    actual_lines, reference_lines = checker_lines(actual), checker_lines(reference)
    for number, (left, right) in enumerate(zip(actual_lines, reference_lines), 1):
        if left != right:
            return number
    # Preserve established mismatch locations for extra output.
    if len(actual_lines) > len(reference_lines):
        return len(reference_lines) + 2
    if len(reference_lines) > len(actual_lines):
        return len(actual_lines) + 1
    return 0


def main():
    root = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path, default=root / "build/bin/coolc")
    parser.add_argument("--tests", type=Path, default=root / "checker/tests")
    parser.add_argument("--output", type=Path, default=root / "build/checker-results")
    args = parser.parse_args()
    compiler = args.compiler.resolve()
    test_dir = args.tests.resolve()
    test_files = sorted(test_dir.glob("*.cl"))
    if not test_files:
        parser.error(f"No .cl tests found in {test_dir}")
    args.output.mkdir(parents=True, exist_ok=True)
    total = 0
    for test in test_files:
        reference = test.with_suffix(".ref").read_bytes()
        # Use paths relative to the fixture directory's parent.
        result = subprocess.run(
            [str(compiler), "--", str(test.relative_to(test_dir.parent))],
            cwd=test_dir.parent, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
        (args.output / (test.stem + ".out")).write_bytes(result.stdout)
        line = mismatch(result.stdout, reference)
        if result.returncode != 0:
            status = f"Failed (compiler exited {result.returncode})"
        elif line:
            status = f"Failed at line {line}"
        else:
            status = "OK"
            total += 5
        print(f"{test.name:<30} -> {status}")
    print(f"Total: {total}")
    # Keep comparison diagnostics separate from runner setup failures.
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except OSError as error:
        print(f"Checker error: {error}", file=sys.stderr)
        sys.exit(1)
