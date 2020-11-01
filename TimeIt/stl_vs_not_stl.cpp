
#include "benchmark/benchmark.h"


// Benchmark: std::find_if vs for //

static void BM_FindIf(benchmark::State& state)
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
BENCHMARK(BM_FindIf);


static void BM_FindIfLoop(benchmark::State& state)
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
BENCHMARK(BM_FindIfLoop);


static void BM_FindIfLoop2(benchmark::State& state)
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
BENCHMARK(BM_FindIfLoop2);


static void BM_FindIfLoopC(benchmark::State& state)
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
BENCHMARK(BM_FindIfLoopC);
