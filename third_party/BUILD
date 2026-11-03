load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "antlr4_runtime",
    srcs = glob(["antlr4-runtime/src/**/*.cpp"]),
    hdrs = glob(["antlr4-runtime/src/**/*.h"]),
    defines = ["ANTLR4CPP_STATIC"],
    includes = ["antlr4-runtime/src"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
