#include "call_with_tuple.h"
#include <iostream>

using namespace std;

class TestClass
{
public:
	TestClass() = default;

	int Foo(int x, int y, char* msg)
	{
		cout << "TestClass::Foo( " << x << " " << y << " " << msg << ") \n";
		return (x * 1000) + y;
	}
};

int main(int agsc, char** args)
{
	{
		TestClass testClass;
		CallFunc<TestClass, int, int, int, char*> callfunc(&testClass, &TestClass::Foo);

		cout << callfunc.invoke(100, 101, "102") << "\n";
	}

	{
		TestClass testClass;
		CallFunc<TestClass, int, int, int, char*> callfunc(&testClass, &TestClass::Foo, 200, 201, "202");

		cout << callfunc.invoke2() << "\n";
	}


	return 0;
}