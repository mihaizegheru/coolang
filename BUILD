alias(
    name = "coolc",
    actual = "//tools/coolc:coolc",
    visibility = ["//visibility:public"],
)

# Export regression fixtures without changing the fixture tree.
filegroup(
    name = "checker_files",
    srcs = glob([
        "checker/tests/*.cl",
        "checker/tests/*.ref",
    ]),
    visibility = ["//test:__pkg__"],
)
