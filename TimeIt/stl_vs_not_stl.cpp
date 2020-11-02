#include "Derectives.h"
#include "benchmark/benchmark.h"


#ifdef STL_NOT_STL

// Benchmark: std::find_if vs for //

static void STL_FindIf(benchmark::State& state)
{
	std::vector<int> vec(1000, 0);
	int findVal = 1000;
	vec[999] = findVal;

	for (auto _ : state)
	{
		auto it = std::find_if(std::begin(vec), std::end(vec), [&findVal](const auto& val) {
			return findVal == val;
			});
		if (it != std::end(vec))
		{
			*it = 2000000;
		}
	}
}
BENCHMARK(STL_FindIf)->Iterations(100000);

static void STL_FindIfLoop(benchmark::State& state)
{
	std::vector<int> vec(1000, 0);
	int findVal = 1000;
	vec[999] = findVal;

	for (auto _ : state)
	{
		auto it_end = std::end(vec);
		for (auto it = std::begin(vec); it != it_end; ++it)
		{
			if (*it == findVal)
			{
				*it = 2000000;
				break;
			}
		}
	}
}
BENCHMARK(STL_FindIfLoop)->Iterations(100000);

static void STL_FindIfLoop2(benchmark::State& state)
{
	std::vector<int> vec(1000, 0);
	int findVal = 1000;
	vec[999] = findVal;

	for (auto _ : state)
	{
		auto sz = vec.size();
		for (size_t i = 0; i < sz; ++i)
		{
			if (vec[i] == findVal)
			{
				vec[i] = 2000000;
				break;
			}
		}
	}
}
BENCHMARK(STL_FindIfLoop2)->Iterations(100000);

static void STL_FindIfLoopC(benchmark::State& state)
{
	int vec[1000];
	std::memset((void*)vec, 0, 1000);

	int findVal = 1000;
	vec[999] = findVal;

	for (auto _ : state)
	{
		for (size_t i = 0; i < 1000; ++i)
		{
			if (vec[i] == findVal)
			{
				vec[i] = 2000000;
				break;
			}
		}
	}
}
BENCHMARK(STL_FindIfLoopC)->Iterations(100000);

#endif