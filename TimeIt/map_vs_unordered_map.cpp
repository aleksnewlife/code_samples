
#include "benchmark/benchmark.h"
#include <map>
#include <unordered_map>
#include <random>

template<typename T>
static void MapInsert(benchmark::State& state)
{
	std::string test_str("test string test string test string");

	std::vector<std::string> keys;
	keys.resize(state.max_iterations);
	for (int i = 0; i < state.max_iterations; ++i)
	{
		keys[i] = test_str + std::to_string(rand() % state.max_iterations);
	}
	T mp;

	uint64_t i = 0;
	for (auto _ : state)
	{
		mp.emplace(std::pair(keys[i], test_str));
		++i;
	}
}

BENCHMARK_TEMPLATE(MapInsert, std::map<std::string, std::string>)->Iterations(1000000);
BENCHMARK_TEMPLATE(MapInsert, std::unordered_map<std::string, std::string>)->Iterations(1000000);


template<typename T>
static void MapRemove(benchmark::State& state)
{
	std::string test_str("test string test string test string");

	std::vector<std::string> keys;
	keys.resize(state.max_iterations);
	for (int i = 0; i < state.max_iterations; ++i)
	{
		keys[i] = test_str + std::to_string(rand() % state.max_iterations);
	}
	T mp;

	uint64_t i = state.max_iterations;
	for (auto _ : state)
	{
		mp.erase(keys[i]);
		--i;
	}
}

BENCHMARK_TEMPLATE(MapRemove, std::map<std::string, std::string>)->Iterations(1000000);
BENCHMARK_TEMPLATE(MapRemove, std::unordered_map<std::string, std::string>)->Iterations(1000000);


template<typename T>
static void MapFind(benchmark::State& state)
{
	std::string test_str("test string test string test string");

	std::vector<std::string> keys;
	keys.resize(state.max_iterations);
	for (int i = 0; i < state.max_iterations; ++i)
	{
		keys[i] = test_str + std::to_string(rand() % state.max_iterations);
	}
	T mp;

	uint64_t i = state.max_iterations;
	for (auto _ : state)
	{
		(void) mp.find(keys[i]);
		--i;
	}
}

BENCHMARK_TEMPLATE(MapFind, std::map<std::string, std::string>)->Iterations(1000000);
BENCHMARK_TEMPLATE(MapFind, std::unordered_map<std::string, std::string>)->Iterations(1000000);
