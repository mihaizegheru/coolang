load("@rules_shell//shell:sh_binary.bzl", "sh_binary")

sh_binary(
    name = "check",
    srcs = ["run_checker.sh"],
    args = [
        "$(rlocationpath //tools/coolc:coolc)",
        "$(rlocationpath //utils:checker)",
    ],
    data = [
        "//:checker_files",
        "//tools/coolc",
        "//utils:checker",
    ],
    visibility = ["//visibility:public"],
    deps = ["@bazel_tools//tools/bash/runfiles"],
)
