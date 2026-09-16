load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name="murmur3",
    srcs=["murmur3/murmur3_hash.h", "murmur3/murmur3_hash.c", "murmur3/bench.cpp"],
    deps = ["@google_benchmark//:benchmark_main"]
)
