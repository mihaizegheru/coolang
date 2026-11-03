#!/usr/bin/env python3
"""Remove only the project's generated build convenience directory."""
from pathlib import Path
import shutil

root = Path(__file__).resolve().parent.parent
output = root / "build"
if output.exists():
    if output.is_symlink() or output.resolve().parent != root:
        raise RuntimeError(f"Refusing to remove unexpected build path: {output}")
    shutil.rmtree(output)
