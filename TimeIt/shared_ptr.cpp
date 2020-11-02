#include "Derectives.h"
#include "benchmark/benchmark.h"

#include <memory>


#ifdef SHARED_PTR

class TestSharedPtr
{
};


std::shared_ptr<TestSharedPtr> _copy;

int spPassByValue(std::shared_ptr<TestSharedPtr> val)
{
	_copy = val;
	return 0;
}

int spPassByRef(std::shared_ptr<TestSharedPtr>& val)
{
	_copy = val;
	return 0;
}


static void PassSharedPtr_Value(benchmark::State& state)
{
	auto ptr = std::make_shared<TestSharedPtr>();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(spPassByValue(ptr));
		
	}
}
BENCHMARK(PassSharedPtr_Value);


static void PassSharedPtr_Ref(benchmark::State& state)
{
	auto ptr = std::make_shared<TestSharedPtr>();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(spPassByRef(ptr));
	}
}
BENCHMARK(PassSharedPtr_Ref);


#endif