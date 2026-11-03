# Project conventions

Follow the Google C++ Style Guide and docs/StyleGuide.md for project-owned C++.
Use C++20, lowercase snake_case .cc/.h filenames, header guards, PascalCase
functions/types, snake_case variables, and trailing underscores on class fields.
Keep paired headers and implementations in each cool/<component> package with
its BUILD file. The CLI entry point belongs under tools/coolc/.

Run make format before make check-style. Validate code changes with make build
and make check; inspect per-file comparison results rather than relying solely
on the regression runner's process exit status.

Preserve checker/ and third_party/antlr4-runtime/ contents. Exclude generated
files in build/ and bazel-bin/ from formatting. Development tools live outside
the source tree in the user cache.

Use BUILD files and MODULE.bazel for builds. The main targets are //:coolc and
//test:check. Keep MODULE.bazel.lock tracked. Style commands also cover owned
BUILD and Starlark files.

ANTLR-generated API names and overrides retain their required spellings.
Contain dependency exceptions at the CLI boundary; use return values for project
I/O failures. Document frontend behavior in docs/Frontend.md and component
boundaries in docs/Architecture.md.
