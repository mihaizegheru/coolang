load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "coolc",
    srcs = ["main.cc"],
    copts = [
        "-Wall",
        "-Wextra",
        "-Wpedantic",
    ],
    visibility = ["//visibility:public"],
    deps = ["//cool/frontend"],
)
