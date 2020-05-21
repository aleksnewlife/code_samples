#pragma once

#include <string>
#include <iostream>


#include <map>

extern std::map<int, std::string> INDEX_TO_TYPE;


template<int typeID>
class Object
{
public:
	static inline int __counter;

	Object(const char* dummy)
	{
		++__counter;
	}

	~Object()
	{
		
	}
};

const Object<1> STR1 = "dummy";
static const Object<2> STR2 = "dummy";
static const inline Object<3> STR3 = "dummy";
static inline Object<4> STR4 = "dummy";

//namespace NP_ONE
//{
//	const Object<1> STR1 = "dummy";
//	static const Object<2> STR2 = "dummy";
//	static const inline Object<3> STR3 = "dummy";
//	static inline Object<4> STR4 = "dummy";
//};


class DeclareInClass
{
public:
	const Object<5> STR5 = "dummy";
	static const Object<6> STR6;
	static const inline Object<7> STR7 = "dummy";
	static inline Object<8> STR8 = "dummy";
};


void printObjectCounters();

void testDeclareInHeader();


//// Test from Ivan Mantrov:
/*
������ ��: 18.05.2020 9:03
����� ��������� ���������� Visual Studio 2017 �� ������ 15.9.23, �� ��� ����� ����������, 
����������� � ��������� � ���� ����������� ��� ������ �������� Unified Editor:

�� �������� ����� ����������� �������� �� ���� ������������� static inline variable, � ��������� �������������, � ������:
1)	����� ������������ Colour (������� �������� ����������� struct ColourRGBAFloat) ��� �������������;
2)	������������� �������� BW_MOO_DLL (������ __declspec(dllimport) / __declspec(dllexport)) ��� ���������� ������.

��� ������� ������������� ��� � ����������: ��� ������ ������������ ��� ��� __declspec(dllimport) ������� �� ����������.
���������� ������ �� �������� �� ������ Debug/Release, x64/x86.

��� �� �������, ������ ������ ����������, ���� ��� ���� ������������ ���� ���. �������� ������ ��������� ������������� :)

*/

#define EXPORT

#ifdef EXPORT
#define DLL_DEF __declspec(dllexport)
#else
#define DLL_DEF __declspec(dllimport)
#endif
class DLL_DEF Test
{
public:
	struct DLL_DEF My
	{
		My()
		{
			x = 10;
		}
		int x;
	};
	static inline const My m = My();
};