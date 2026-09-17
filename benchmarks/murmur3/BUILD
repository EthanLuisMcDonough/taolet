load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name="murmur_bench",
    hdrs=["murmur3_hash.h", "murmur_bench.h"],
    srcs=["murmur3_hash.c", "murmur_bench.cpp"],
    deps = ["@google_benchmark//:benchmark_main"],
    visibility = ["//benchmarks:__pkg__"],
)
