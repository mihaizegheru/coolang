# Regression checks

Run the frontend against the reference fixture corpus:

```bash
make check
# or:
bazel run //test:check
```

The target builds the compiler and invokes `utils/check.py`. Its data files are
provided through Bazel runfiles; generated output is written to the workspace's
`build/checker-results/` directory.

## Fixtures and results

- `checker/tests/*.cl`: input programs.
- `checker/tests/*.ref`: expected output.
- `build/checker-results/*.out`: captured compiler output.

The runner merges stdout and stderr before comparison. It recognizes CR, LF,
and CRLF line endings while preserving other control characters in token text.
Per-file results show `OK`, a differing line, or an unexpected compiler exit.
The runner can return zero with mismatches, so inspect those individual results.

To investigate a difference:

```bash
diff -u checker/tests/01-class.ref build/checker-results/01-class.out
```

For lexer changes, isolate recognition from parser and AST behavior:

```bash
make tokens ARGS="checker/tests/01-class.cl"
```

The compatibility files and fixtures in `checker/` are preserved in place.
Their integrity manifest is `docs/checker-sha256.json`. Add new regression tests
under `test/` with explicit Bazel dependencies rather than editing those fixtures.
