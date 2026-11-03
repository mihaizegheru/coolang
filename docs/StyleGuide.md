# C++ and BUILD style

Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
Keep paired `.h` and `.cc` files in the same `cool/<component>` Bazel package,
with a local BUILD file and source-tree include paths.

## C++ conventions

- Target standard C++20 and use lowercase underscore-separated filenames.
- Make headers self-contained and use project/path-based macro guards.
- Include definitions directly and keep implementation details package-private.
- Use PascalCase types and functions, snake_case variables, and trailing
  underscores on class data members. Accessors may use snake_case.
- Prefer explicit ownership, RAII, explicit constructors, and small interfaces.
- Mark pending implementation work with `TODO(coolang)` comments.

ANTLR grammar filenames, generated APIs, and override names retain the forms
required by the tool. Dependency exceptions are contained at the executable
boundary; project I/O failures use return values.

A project license has not been selected, so cpplint's copyright check is disabled.
Vendored license and provenance files retain their existing contents.

## Commands

```bash
make format
make lint
make check-style
```

`make format` applies clang-format's Google preset to owned C++ sources and
buildifier to BUILD, MODULE.bazel, and `.bzl` files. `make lint` runs cpplint;
`make check-style` verifies formatting and lint without rewriting files.
Generated output, the compatibility checker tree, and vendored runtime sources
are excluded.

## Tool setup

The development setup uses clang-format 21, cpplint 2.0.2, and buildifier 10.2.0.
Install the tools on PATH, or use the external tool cache:

```bash
python3 -m pip install \
  --target "${XDG_CACHE_HOME:-$HOME/.cache}/coolang-tools/style/python" \
  cpplint==2.0.2
```

The cached buildifier path is
`${XDG_CACHE_HOME:-$HOME/.cache}/coolang-tools/bin/buildifier`.
Source formatting follows `.clang-format`; editor indentation follows
`.editorconfig`. Bazel supplies the compilation database through
`make compile-commands`.
