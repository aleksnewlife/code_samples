
#include "benchmark/benchmark.h"

#include <iostream>


int main(int argc, char** argv)
{
    std::cout << "Benchmarks: \n";

	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
	::benchmark::RunSpecifiedBenchmarks();
}
