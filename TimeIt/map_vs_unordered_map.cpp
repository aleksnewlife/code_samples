#include "Derectives.h"
#include "benchmark/benchmark.h"
#include <map>
#include <unordered_map>
#include <random>


#ifdef MAP_VR_UNMAP

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
	state.SetLabel("random key");
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


template<typename T>
static void MapForEach(benchmark::State& state)
{
	std::string value("test string test string test string");

	T mp;
	for (int i = 0; i < state.max_iterations; ++i)
	{
		std::string key(value + std::to_string(rand() % state.max_iterations));
		mp.insert(std::pair(key, value));
	}
	
	int i = 0;
	for (auto _ : state)
	{
		for (const auto& _ : mp)
		{
			++i;
		}
	}
}
BENCHMARK_TEMPLATE(MapForEach, std::map<std::string, std::string>)->Iterations(10000);
BENCHMARK_TEMPLATE(MapForEach, std::unordered_map<std::string, std::string>)->Iterations(10000);


template<typename T>
static void MapForIt(benchmark::State& state)
{
	std::string value("test string test string test string");

	T mp;
	for (int i = 0; i < state.max_iterations; ++i)
	{
		std::string key(value + std::to_string(rand() % state.max_iterations));
		mp.insert(std::pair(key, value));
	}

	int i = 0;
	for (auto _ : state)
	{
		for (auto it = std::begin(mp), it_end = std::end(mp); it != it_end; ++it)
		{
			++i;
		}
	}
}
BENCHMARK_TEMPLATE(MapForIt, std::map<std::string, std::string>)->Iterations(10000);
BENCHMARK_TEMPLATE(MapForIt, std::unordered_map<std::string, std::string>)->Iterations(10000);

#endif