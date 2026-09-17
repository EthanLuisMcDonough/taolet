#include "murmur3/murmur_bench.h"

BENCHMARK(BM_Murmur3_Rand)->Args({1, 1 << 4})->Args({1 << 5, 1 << 8});
BENCHMARK(BM_Murmur3_Fixed)->DenseRange(1 << 4, (1 << 4) + 3, 1);
