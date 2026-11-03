# Frontend behavior

This document defines the frontend's intended recognition, diagnostic, and AST
output contracts. Implementation coverage is described in the root README.
The source grammars are `cool/parser/CoolLexer.g4` and `CoolParser.g4`; the parser
entry rule is `program`.

## Inputs and output channels

The compiler accepts one or more Cool source paths. Each file has its own parsing
session, and its classes contribute to one program AST in input order.

Normal compilation writes the hierarchical AST to stdout. Diagnostics go to
stderr. Source errors suppress AST printing and end with `Compilation halted`.
Token-only mode bypasses parsing and prints each token's text and symbolic name:

```bash
./utils/run.sh first.cl second.cl
./utils/run.sh --tokens first.cl
```

Generated lexer/parser files are build outputs under
`bazel-bin/cool/parser/generated/`. Grammar actions use C++; Bazel handles token
vocabulary sharing between the lexer and parser.

## String values

The token text for a string literal contains its decoded value:

- Remove the opening and closing double quotes.
- Decode `\n`, `\t`, `\b`, and `\f` into their control characters.
- For another escaped character `\c`, retain `c` and remove the backslash.
- Escaped line breaks remain part of the string value.

For example, the source spelling `"12\n\a34"` decodes to `12`, a newline,
and `a34`. Its decoded length is six characters. A string may contain at most
1024 decoded characters.

Distinguish an actual NUL character from the two-character spelling `\0`.
The former is invalid inside a string; the latter decodes to the digit `0`.

Lexer actions can call `setText(...)` to replace the current token text. The
`RaiseError(...)` helper changes its category to `ERROR` and stores a diagnostic
message in that text.

## Lexical diagnostics

Represent lexical failures as `ERROR` tokens so recognition can continue without
ANTLR lexer recognition errors. Report the location at the start of the affected
literal, comment, or character.

| Message | Condition |
| --- | --- |
| `String constant too long` | Decoded string length exceeds 1024 characters. |
| `String contains null character` | A string contains an actual NUL. |
| `Unterminated string constant` | A string contains an unescaped newline; resume after it. |
| `EOF in string constant` | Input ends before the closing quote. |
| `Unmatched *)` | A block-comment terminator occurs outside a block comment. |
| `EOF in comment` | Input ends inside a block comment. |
| `Invalid character: c` | An unsupported character is encountered; include it in the message. |

Block comments support nesting. Whitespace and comments do not produce parser
input tokens. Handle both LF and CRLF line endings consistently.

Diagnostics use the source basename, a one-based line and column, and the error
category. For example:

```text
"input.cl", line 2:13, Lexical error: Invalid character: #
Compilation halted
```

ANTLR's parser supplies syntax diagnostic messages; the frontend adds the source
location and `Syntax error:` prefix. Keep these diagnostics separate from
lexer-produced `ERROR` messages.

## AST model

Build the AST by visiting generated parse-tree contexts, then visit the AST to
produce its textual representation. AST structure can differ from the generated
parse tree, allowing later analyses to work independently of grammar details.

Retain relevant tokens for lexemes and source locations. Preserve parser-context
and filename associations for subsequent diagnostics. Parsing sessions must
outlive AST nodes that reference their tokens or contexts.

## Hierarchical representation

Print one node per line. Each child's indentation is two spaces deeper than its
parent's. Children follow source-language order, omitting syntactic delimiters
such as parentheses, braces, and separators.

The table lists each node's printed label and children. `?` denotes an optional
child; `*` denotes a sequence. Placeholders describe children, not literal output.

| Construct | Label | Children |
| --- | --- | --- |
| Program | `program` | classes* |
| Class | `class` | name, parent?, features* |
| Attribute | `attribute` | name, type, initializer? |
| Method | `method` | name, formals*, return type, body |
| Variable | variable name | none |
| Literal | decoded literal value | none |
| Binary expression | operator text | left operand, right operand |
| Unary expression | operator text | operand |
| Variable update | `<-` | variable, expression |
| Explicit dispatch | `.` | receiver, static type?, method, arguments* |
| Implicit dispatch | `implicit dispatch` | method, arguments* |
| Conditional | `if` | condition, then branch, else branch |
| Loop | `while` | condition, body |
| Let expression | `let` | local definitions*, body |
| Local definition | `local` | name, type, initializer? |
| Case expression | `case` | expression, branches* |
| Case branch | `case branch` | name, type, body |
| Block | `block` | expressions* |

For input containing `class A {};`, the intended representation is:

```text
program
  class
    A
```

A decoded string may itself contain line breaks or tabs. Its value is printed
as stored rather than re-escaped. The reference fixtures in `checker/tests`
provide concrete output examples for each construct.

## Inspecting changes

Use a small source file to inspect tokens independently:

```bash
make tokens ARGS="path/to/input.cl"
```

Run `make check` to compare end-to-end behavior with the reference fixtures.
Inspect `.out` files under `build/checker-results/` alongside the matching `.ref`
files when tracking an output difference.
