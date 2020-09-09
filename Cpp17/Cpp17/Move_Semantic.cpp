#include "pch.h"

#include <stdio.h>
#include <utility>
#include "Move_Semantic.h"

class A
{
public:
	A()
	{
		_val = new int(99999999);
		printf("A() %p\n", this);
	}

	A(const A& other)
	{
		_val = new int(0);
		*_val = *other._val;
		printf("A(A&) %p\n", this);
	}

	A& operator=(const A& other)
	{
		_val = new int(0);
		*_val = *other._val;
		printf("A::operator=(A&) %p\n", this);
		return *this;
	}

	A(A&& other)
	{
		_val = other._val;
		other._val = nullptr;
		printf("A(A&&) %p\n", this);
	}

	A& operator=(A&& other)
	{
		delete _val;
		_val = other._val;
		other._val = nullptr;
		printf("A::operator=(A&&) %p\n", this);
		return *this;
	}    
	
	~A()
	{
		if (_val)
		{
			printf("~A() %p\t_val = %d \n", this, *_val);
		}
		else
		{
			printf("~A() %p\t_val = None \n", this);
		}
		delete _val;
	}

	int* _val = nullptr;

};


void funcV(A a_val)
{
	static int __temp = *a_val._val;
	printf("\t funcV(A) - A passed by value\n");
}

void func(A& a_val)
{
	static int __temp = *a_val._val;
	printf("\t func(A&) - A passed by reference\n");
}

void func(A&& a_val)
{
	static int __temp = *a_val._val;
	printf("\t func(A&&) - A passed by rval\n");
}


 void test_MoveSemantic()
{
	{
		printf("Test #1\n");

		printf("A a; -- "); 
		A a;	// Constructor

		printf("A a2(a); -- ");
		A a2(a);	// Copy Constructor
		//equivalent:  A a3 = a;	// Copy Constructor

		printf("A a3; a3 = a; -- ");
		A a3;
		a3 = a;	// Copy Operator
		printf(".\n");

		printf("A a4(std::move(a)); -- ");
		A a4(std::move(a));	  // Move Constructor

		printf("A a5 = std::move(a2); -- ");
		A a5 = std::move(a2);    // Move Constructor

		printf("A a6; a6 = std::move(a3); -- ");
		A a6;
		a6 = std::move(a3);     // Move Operator
		printf(".\n");
	}
	printf("---------\n");
	{
		printf("Test #2\n");
		A a;
		func(a);    // passed by reference
		func(std::move(a));     // passed by rval
		funcV(a);   // passed by value
	}
	printf("---------\n");

	printf("- OK -\n");
}