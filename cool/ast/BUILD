load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("//bazel:cc_rules.bzl", "cool_cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ast",
    hdrs = ["ast.h"],
    deps = ["//third_party:antlr4_runtime"],
)

cool_cc_library(
    name = "ast_printer",
    srcs = ["ast_printer.cc"],
    hdrs = ["ast_printer.h"],
    deps = [":ast"],
)
