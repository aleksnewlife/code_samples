// AlgoNarayany.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "NarayanAlgo.h"

struct SpItem
{
	SpItem(int val)
		: _val(val)
		, _name("*")
	{
	}

	SpItem(int val, const char* name)
		: _val(val)
		, _name(name)
	{
	}

	SpItem& operator=(SpItem& b)
	{
		_val = b._val;
		_name = b._name;
		return *this;
	}

	bool operator>=(SpItem const& b) const
	{
		return _val >= b._val;
	}

	bool operator<=(SpItem const& b) const
	{
		return _val <= b._val;
	}

	operator int() const
	{
		return _val;
	}

	operator const char*() const
	{
		return _name;
	}

	const char* getName() const
	{
		return _name;
	}

protected:
	int _val;
	const char* _name;
};


template<typename TItem>
void printArray(const NarayanAlgo<TItem>& algo)
{
	auto& algoData = algo.getData();
	for (auto& it : algoData)
		std::cout << (const char*)it << " ";
	std::cout << '\n';
}

// Done

int main()
{
	NarayanAlgo<SpItem> algo{ { 1, "A" }, { 2, "B" }, { 3, "C" } };
	printArray(algo);
	while (algo.doStep())
	{
		printArray(algo);
	}

    return 0;
}


