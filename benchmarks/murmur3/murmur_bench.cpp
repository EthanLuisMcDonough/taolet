#include <benchmark/benchmark.h>
#include <iostream>
#include <cstdlib>

extern "C" {
    #include "murmur3_hash.h"
}

#ifndef STR_HEAP_LEN
#define STR_HEAP_LEN (1 << 8) * 100000
#endif

template <typename T>
static std::vector<T> random_fill(size_t len, T low, T high) {
    std::srand(200);
    std::vector<T> v(len, 0);
    for (auto &val : v)
        val = static_cast<T>(std::rand() % (high - low) + low);
    return v;
}

void BM_Murmur3_Rand(benchmark::State &state) {
    size_t MIN_LEN = state.range(0);
    size_t MAX_LEN = state.range(1);
    size_t KEYS = STR_HEAP_LEN / MAX_LEN;

    auto chars = random_fill<uint8_t>(STR_HEAP_LEN, 0, 255);
    auto sizes = random_fill<uint8_t>(KEYS, MIN_LEN, MAX_LEN);

    for (auto _ : state) {
        size_t index = state.iterations() % STR_HEAP_LEN;
        size_t data_index = index * MAX_LEN;
        auto result = MurmurHash3_x86_32(chars.data() + data_index, sizes[index]);
        benchmark::DoNotOptimize(result);
    }
}

void BM_Murmur3_Fixed(benchmark::State &state) {
    size_t FIXED_LEN = state.range(0);
    auto chars = random_fill<uint8_t>(STR_HEAP_LEN, 0, 255);

    for (auto _ : state) {
        size_t index = state.iterations() % STR_HEAP_LEN;
        size_t data_index = index * FIXED_LEN;
        auto result = MurmurHash3_x86_32(chars.data() + data_index, FIXED_LEN);
        benchmark::DoNotOptimize(result);
    }
}
