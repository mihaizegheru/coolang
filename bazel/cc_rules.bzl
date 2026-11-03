"""Shared settings for project-owned C++ components."""

load("@rules_cc//cc:cc_library.bzl", "cc_library")

def cool_cc_library(name, **kwargs):
    """Declares a compiler component with project warning settings."""
    cc_library(
        name = name,
        copts = ["-Wall", "-Wextra", "-Wpedantic"],
        **kwargs
    )
