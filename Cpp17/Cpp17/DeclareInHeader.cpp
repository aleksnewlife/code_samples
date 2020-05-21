#include "pch.h"

#include "DeclareInHeader.h"  

#include <string>
#include <iostream>

std::map<int, std::string> INDEX_TO_TYPE = {
	{ 1, "const" },
	{ 2, "static const" },
	{ 3, "static const inline" },
	{ 4, "static inline" },
	{ 5, "const - in class" },
	{ 6, "static const - in class" },
	{ 7, "static const inline - in class" },
	{ 8, "static inline - in class" },
};

//std::map<int, int> OBJECT_COUNTER = {};



const Object<6> DeclareInClass::STR6 = "dummy";



template<int tID>
void PrintCounters() // T t, Args...args
{
	std::cout << "Type: " << INDEX_TO_TYPE[tID] << " : " << Object<tID>::__counter << std::endl;
	//const int nextID = tID + 1;
	PrintCounters<tID-1>();
};

template<>
void PrintCounters<0>()
{
}

void printObjectCounters()
{
	std::cout << "OBJECT_COUNTER:" << std::endl;
	{
		PrintCounters<8>();
	}

	std::cout << "static const inline, ref: " << (int)&STR3 << std::endl;
	std::cout << "static inline, ref: " << (int)&STR4 << std::endl;

	std::cout << "static const inline in class, counte ref: " << (int)&Object<7>::__counter << std::endl;
	std::cout << "static inline in class, counte ref: " << (int)&Object<8>::__counter << std::endl;

	std::cout << "---" << std::endl;
}

void testDeclareInHeader()
{
	DeclareInClass d; // STR6
}
