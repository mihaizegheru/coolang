load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//bazel:cc_rules.bzl", "cool_cc_library")

_GENERATED_SOURCES = [
    "generated/CoolLexer.cpp",
    "generated/CoolParser.cpp",
    "generated/CoolParserBaseListener.cpp",
    "generated/CoolParserListener.cpp",
    "generated/CoolParserBaseVisitor.cpp",
    "generated/CoolParserVisitor.cpp",
]

_GENERATED_HEADERS = [
    "generated/CoolLexer.h",
    "generated/CoolParser.h",
    "generated/CoolParserBaseListener.h",
    "generated/CoolParserListener.h",
    "generated/CoolParserBaseVisitor.h",
    "generated/CoolParserVisitor.h",
]

genrule(
    name = "generate_grammar",
    srcs = [
        "CoolLexer.g4",
        "CoolParser.g4",
    ],
    outs = _GENERATED_SOURCES + _GENERATED_HEADERS + [
        "generated/CoolLexer.interp",
        "generated/CoolLexer.tokens",
        "generated/CoolParser.interp",
        "generated/CoolParser.tokens",
    ],
    cmd = """
        mkdir -p "$(RULEDIR)/generated"
        "$(execpath //bazel:antlr4)" -Dlanguage=Cpp -Xexact-output-dir \
          -o "$(RULEDIR)/generated" "$(execpath CoolLexer.g4)"
        "$(execpath //bazel:antlr4)" -Dlanguage=Cpp -listener -visitor \
          -Xexact-output-dir -lib "$(RULEDIR)/generated" \
          -o "$(RULEDIR)/generated" "$(execpath CoolParser.g4)"
    """,
    tools = ["//bazel:antlr4"],
)

cc_library(
    name = "grammar",
    srcs = _GENERATED_SOURCES,
    hdrs = _GENERATED_HEADERS,
    includes = ["generated"],
    deps = ["//third_party:antlr4_runtime"],
)

cool_cc_library(
    name = "ast_builder",
    srcs = ["ast_builder.cc"],
    hdrs = ["ast_builder.h"],
    deps = [
        ":grammar",
        "//cool/ast",
    ],
)

cool_cc_library(
    name = "parser",
    srcs = ["parsing.cc"],
    hdrs = ["parsing.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":ast_builder",
        ":grammar",
        "//cool/ast",
        "//third_party:antlr4_runtime",
    ],
)
