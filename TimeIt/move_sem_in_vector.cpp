
#include "benchmark/benchmark.h"
#include <string>
#include <iostream>

class Element_Bad
{
public:
	static int _Move;
	static int _Copy;

	Element_Bad()
	{
	}

	explicit Element_Bad(std::string& s)
		: _str(s)
	{
	}

	Element_Bad(const Element_Bad& other)
	{
		_str = other._str;
		++_Copy;
	}

	Element_Bad& operator=(const Element_Bad& other)
	{
		_str = other._str;
		++_Copy;
	}

	Element_Bad(Element_Bad&& other) // can throw
	{
		_str = std::move(other._str);
		++_Move;
	}

	Element_Bad& operator=(Element_Bad&& other)	// can throw
	{
		_str = std::move(other._str);
		++_Move;
	}

private:
	std::string _str;
};

int Element_Bad::_Move;
int Element_Bad::_Copy;


class Element_Good
{
public:
	static int _Move;
	static int _Copy;

	Element_Good()
	{
	}

	explicit Element_Good(std::string& s)
		: _str(s)
	{
	}

	Element_Good(const Element_Good& other)
	{
		_str = other._str;
		++_Copy;
	}

	Element_Good& operator=(const Element_Good& other)
	{
		_str = other._str;
		++_Copy;
	}

	Element_Good(Element_Good&& other) noexcept // can throw
	{
		_str = std::move(other._str);
		++_Move;
	}

	Element_Good& operator=(Element_Good&& other) noexcept // can throw
	{
		_str = std::move(other._str);
		++_Move;
	}

private:
	std::string _str;
};

int Element_Good::_Move;
int Element_Good::_Copy;


template<typename T>
static void BadResizeVector_WithCopy(benchmark::State& state)
{
	std::string test_str("test string test string test string");
	std::vector<T> vec;
	for (auto _ : state)
	{
		vec.emplace_back(T());
	}

	state.SetLabel(std::string("copy: ") + std::to_string(T::_Copy) + std::string("; move: ") + std::to_string(T::_Move));
}

BENCHMARK_TEMPLATE(BadResizeVector_WithCopy, Element_Bad)->Iterations(1000000);
BENCHMARK_TEMPLATE(BadResizeVector_WithCopy, Element_Good)->Iterations(1000000);
