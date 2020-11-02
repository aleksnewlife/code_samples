
#include "Derectives.h" // Use it for benchmark enabling
#include "benchmark/benchmark.h"

#include <iostream>

// TODO: copy, move, ref std::shared_ptr


int main(int argc, char** argv)
{
    std::cout << "Benchmarks: \n";

	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
	::benchmark::RunSpecifiedBenchmarks();
}
