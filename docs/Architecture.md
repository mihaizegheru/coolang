# Compiler architecture

Compiler code is organized into Bazel packages under `cool/`. Each package keeps
its `.h`, `.cc`, and BUILD files together. Includes use source-tree paths such as
`#include "cool/parser/parsing.h"`.

## Packages

- `cool/ast`: AST node definitions in `ast.h` and the paired
  `ast_printer.h` / `ast_printer.cc` implementation. Nodes retain ANTLR token and
  context information for source locations and later analysis.
- `cool/parser`: paired `parsing.h` / `parsing.cc` and
  `ast_builder.h` / `ast_builder.cc`, together with the lexer and parser grammars.
  The AST builder and generated grammar targets are package-private. `Parsing`
  hides generated parser details behind its implementation.
- `cool/frontend`: paired `compiler.h` / `compiler.cc`, providing the reusable
  multi-file compilation pipeline.
- `tools/coolc`: the command-line entry point, exposed through `//:coolc`.
- `bazel`: common C++ settings and the prebuilt ANTLR generation tool.
- `third_party`: dependency BUILD target and vendored runtime sources.
- `test`: regression-check launcher using fixtures in `checker/tests`.
- `utils`: development commands and editor integration.

## Build graph

```text
//tools/coolc:coolc
    -> //cool/frontend:frontend
        -> //cool/parser:parser
            -> //cool/parser:ast_builder
            -> //cool/parser:grammar
        -> //cool/ast:ast_printer
            -> //cool/ast:ast
    -> //third_party:antlr4_runtime (transitive)

//cool/parser:generate_grammar
    -> //bazel:antlr4
```

The grammar action declares its `.g4` inputs, generated outputs, and tool.
Bazel regenerates affected targets when those inputs change. `MODULE.bazel`
pins rule versions and the generator hash; `MODULE.bazel.lock` records resolution.
ANTLR runs only during generation. The executable links the C++ runtime.

## Compilation pipeline

The driver collects options and input paths, then calls the frontend. The
frontend creates a parsing session per source file. Token-only mode dumps lexer
output directly; normal compilation parses each input and checks diagnostics
before building and printing a combined AST.

The parser owns recognition and parse-tree conversion. The AST represents
language constructs independently of the grammar's generated context hierarchy.
The printer owns the textual representation described in [Frontend.md](Frontend.md).

## Ownership

Each `Parsing` session owns its input, lexer, token stream, parser, and parse tree.
The frontend retains all sessions while the global AST is constructed and
printed. AST token/context pointers are non-owning and remain valid throughout
that pipeline.

If a later stage retains the AST beyond compilation, extend the owning context
rather than keeping pointers to destroyed parser sessions. Keep diagnostic
locations and source filenames available to semantic analysis and lowering.

## Extending the frontend

Add a package such as `cool/semantics/` or `cool/codegen/` when implementing a new
stage. Keep headers, implementations, and BUILD targets together. Connect the
new target to the frontend pipeline and keep CLI handling in `tools/coolc/`.
Generated artifacts belong in Bazel's output tree.
