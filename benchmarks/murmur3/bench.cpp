extern "C" {
    #include "murmur3_hash.h"
}
#include <benchmark/benchmark.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define STR_HEAP_LEN (1 << 8) * 100000

template <typename T>
static void random_fill(std::vector<T> &v, T low, T high) {
    for (auto &val : v)
        val = static_cast<T>(std::rand() % (high - low) + low);
}

static void BM_Murmur3_Rand(benchmark::State &state) {
    size_t MIN_LEN = state.range(0);
    size_t MAX_LEN = state.range(1);
    size_t KEYS = STR_HEAP_LEN / MAX_LEN;

    std::srand(std::time({}));
    std::vector<uint8_t> chars(STR_HEAP_LEN, '\0');
    std::vector<uint8_t> sizes(KEYS, 0);
    random_fill<uint8_t>(chars, 0, 255);
    random_fill<uint8_t>(sizes, MIN_LEN, MAX_LEN);

    for (auto _ : state) {
        size_t index = state.iterations() % STR_HEAP_LEN;
        size_t data_index = index * MAX_LEN;
        auto result = MurmurHash3_x86_32(chars.data() + data_index, sizes[index]);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Murmur3_Rand)->Args({1, 1 << 4})->Args({1 << 5, 1 << 8});
