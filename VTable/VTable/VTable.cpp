
#include "pch.h"
#include <iostream>
#include <iomanip>

using namespace std;

int nop() {
	static int nop_x; return ++nop_x; // Don't remove me, compiler!
};


const unsigned int MEMORY_SIZE = 64;

void PrintMemory(const unsigned char memory[],
	const char label[] = "contents")
{
	cout << "Memory " << label << ": " << endl;
	unsigned int lines = MEMORY_SIZE / 8;
	for (size_t i = 0; i < lines; i++)
	{
		for (size_t j = 0; j < 8; j++)
			cout << setw(2) << setfill('0') << uppercase << hex
			<< static_cast<int> (memory[i * 8 + j]) << " ";
		cout << endl;
	}
}

void cleanMemory(unsigned char memory[])
{
	memset(memory, 0x11, MEMORY_SIZE * sizeof(unsigned char));
}


class A
{
public:
	unsigned long long content_A;
	A(const unsigned char memory[])
		: content_A(0xAAAAAAAAAAAAAAAAull)
	{
		cout << " A() constructord" << endl;
		PrintMemory(memory, " in A()");
	};
	~A(void)
	{
		cout << " ~A() destructor" << endl;
	};

	virtual void function(void) { nop(); };
};


class B : public A
{
public:
	unsigned long long content_B;
	B(const unsigned char memory[])
		: A(memory)
		, content_B(0xBBBBBBBBBBBBBBBBull)
	{
		cout << " B() constructor" << endl;
		PrintMemory(memory, "in B()");
	};
	~B(void)
	{
		cout << " ~B() destructor" << endl;
	};

	virtual void function(void) override { nop(); };
};


class C : public B, virtual public A
{
public:
	unsigned long long content_C;
	C(const unsigned char memory[])
		: A(memory)
		, B(memory)
		, content_C(0xCCCCCCCCCCCCCCCCull)
	{
		cout << " C() constructor" << endl;
		PrintMemory(memory, "in C()");
	};
	~C(void)
	{
		cout << " ~C() destructor" << endl;
	};

	virtual void function(void) override { nop(); };
};



int main()
{
	unsigned char memory[MEMORY_SIZE];
	cleanMemory(memory);

	new (memory) B(memory);
	reinterpret_cast<B*>(memory)->~B();

	cout << " --------- virtual --------- " << endl;
	cleanMemory(memory);

	new (memory) C(memory);
	reinterpret_cast<C*>(memory)->~C();

	return 0;
};
