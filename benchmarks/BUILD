load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name="taolet",
    srcs=["bench.cpp"],
    deps = ["@google_benchmark//:benchmark_main", "//benchmarks/murmur3:murmur_bench"]
)
