#include "pch.h"
#include <iostream>

struct Object { };

struct Keyword
{
	template<typename T>
	static void check(T val) 
	{ 
		std::cout << " Check called: " << val << std::endl;
	}
};

template<class...Keywords>
struct Validator : Object
{
	void check(Keywords...kwargs)
	{

		(Keyword::check(kwargs), ...);
	}
};

void test_fold_expressions()
{
	Validator< int, float, double > validator{ };

	validator.check(123, 234.f, 345.0);
}
