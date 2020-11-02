#include "Derectives.h"
#include "benchmark/benchmark.h"
#include <vector>
#include <deque>

// TODO: vector ves deque (insert in begining, inserty in end, remove from mid)

#ifdef VECTOR_VS_DEQUE

template<typename T>
static void VecDeqInsertInBeg(benchmark::State& state)
{
	T cont;

	for (auto _ : state)
	{
		cont.insert(std::begin(cont), 9999999);
	}
}
BENCHMARK_TEMPLATE(VecDeqInsertInBeg, std::vector<int>);
BENCHMARK_TEMPLATE(VecDeqInsertInBeg, std::deque<int>);


template<typename T>
static void VecDeqInsertInEnd(benchmark::State& state)
{
	T cont;

	for (auto _ : state)
	{
		cont.push_back(9999999);
	}
}
BENCHMARK_TEMPLATE(VecDeqInsertInEnd, std::vector<int>);
BENCHMARK_TEMPLATE(VecDeqInsertInEnd, std::deque<int>);


template<typename T>
static void VecDeqInsertInMid(benchmark::State& state)
{
	T cont(1000, 0);

	for (auto _ : state)
	{
		auto it = std::begin(cont);
		std::advance(it, 500);
		cont.insert(it, 9999999);
	}
}
BENCHMARK_TEMPLATE(VecDeqInsertInMid, std::vector<int>);
BENCHMARK_TEMPLATE(VecDeqInsertInMid, std::deque<int>);

#endif