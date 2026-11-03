load("@rules_java//java:java_binary.bzl", "java_binary")
load("@rules_java//java:java_import.bzl", "java_import")

exports_files(["cc_rules.bzl"])

# Build-time dependency only: no project Java sources or runtime adapter.
java_import(
    name = "antlr4_jar",
    jars = ["@antlr4_tool//file"],
)

java_binary(
    name = "antlr4",
    main_class = "org.antlr.v4.Tool",
    visibility = ["//cool/parser:__pkg__"],
    runtime_deps = [":antlr4_jar"],
)
