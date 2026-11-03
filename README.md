# Coolang

Coolang is a C++20 compiler frontend for the Cool language. It uses ANTLR for
lexing and parsing, a separate abstract syntax tree, and Bazel for builds.
Code is organized into packages with headers, implementations, and BUILD files
kept together, following Google C++ conventions.

The command-line pipeline, token inspection, input ownership, and diagnostic
plumbing are in place. Full language recognition, AST construction, and node
printing are under development. The intended frontend behavior is documented in
[Frontend.md](docs/Frontend.md).

## Requirements

- Bazelisk, using the Bazel version pinned in `.bazelversion`.
- A C++20 compiler and a JDK 21 installation.
- Python 3 for development helpers and regression checks.
- clang-format, cpplint, and buildifier for style checks.

Set `JAVA_HOME` for the local JDK, or use the cached tool installation under
`${XDG_CACHE_HOME:-$HOME/.cache}/coolang-tools/`. The scripts in `utils/` locate
these cached tools automatically. Bazel downloads pinned build dependencies and
the ANTLR 4.13.0 generator on the first build; the C++ runtime is vendored.
The compiled frontend runs without a JVM.

## Build and run

```bash
make build
make run ARGS="checker/tests/01-class.cl"
make tokens ARGS="checker/tests/01-class.cl"
```

The Makefile delegates to Bazel. Direct commands are also available:

```bash
bazel build //:coolc
bazel run //:coolc -- "$PWD/checker/tests/01-class.cl"
bazel run //:coolc -- --tokens "$PWD/checker/tests/01-class.cl"
```

Use `./utils/bazel.sh` if Bazelisk is not on your PATH. Use absolute source paths
with `bazel run`, which executes from a runfiles directory. The `make run` and
`utils/run.sh` helpers preserve the calling working directory.

The executable is `bazel-bin/tools/coolc/coolc`. `make build` also creates
`build/bin/coolc` and exports `build/compile_commands.json` for editor tooling.
Multiple source files can be passed in one invocation:

```bash
./utils/run.sh first.cl second.cl
./utils/run.sh --tokens "path with spaces/input.cl"
```

## Lexer development

Edit `cool/parser/CoolLexer.g4` and run `make tokens` against a small input file.
The lexer-only mode prints `text : TOKEN_NAME` for each token and bypasses parsing
and AST construction. Whitespace matched by a `skip` rule does not appear.

Bazel regenerates the C++ lexer, parser, listeners, and visitors when a grammar
changes. Generated files live under `bazel-bin/cool/parser/generated/`; edit the
source `.g4` files rather than generated output.

## Regression checks

```bash
make check
# or:
bazel run //test:check
```

The runner compares compiler output with the `.ref` fixtures in `checker/tests`.
Per-file results identify matching output or the first differing line. Actual
outputs are written to `build/checker-results/` for inspection:

```bash
diff -u checker/tests/01-class.ref build/checker-results/01-class.out
```

Use the individual comparison results to assess a run; the runner may return
zero even when output differs. See [test/README.md](test/README.md) for details.

## Source layout

| Path | Responsibility |
| --- | --- |
| `cool/ast/` | AST types and tree printing. |
| `cool/parser/` | ANTLR grammars, parsing sessions, and AST construction. |
| `cool/frontend/` | Multi-file compilation pipeline. |
| `tools/coolc/` | Command-line entry point. |
| `bazel/` | Shared C++ rules and the ANTLR generator target. |
| `third_party/` | Vendored ANTLR runtime and dependency BUILD target. |
| `checker/` | Regression inputs, reference output, and compatibility files. |
| `test/` | Bazel regression-check launcher. |
| `utils/` | Build, run, style, and editor helpers. |
| `docs/` | Frontend, architecture, and style documentation. |

Each component declares explicit dependencies in its BUILD file. Includes match
source-tree paths, for example `#include "cool/parser/parsing.h"`.

## Development workflow

```bash
make format
make check-style
make build
make check
```

`make format` applies Google clang-format and buildifier. `make lint` runs
cpplint; `make check-style` verifies C++ and BUILD formatting and C++ lint.
Generated files and vendored dependencies are excluded from style operations.

`make compile-commands` refreshes the editor database from Bazel's action graph.
`make clean` clears generated build outputs and the local `build/` convenience
folder. Bazel recreates its `bazel-*` output symlinks on subsequent builds.

Keep `MODULE.bazel.lock` tracked to preserve dependency resolution.

## Documentation

- [Frontend behavior](docs/Frontend.md)
- [Compiler architecture](docs/Architecture.md)
- [C++ and BUILD style](docs/StyleGuide.md)
- [Regression checks](test/README.md)
