load("//bazel:cc_rules.bzl", "cool_cc_library")

cool_cc_library(
    name = "frontend",
    srcs = ["compiler.cc"],
    hdrs = ["compiler.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//cool/ast",
        "//cool/ast:ast_printer",
        "//cool/parser",
    ],
)
