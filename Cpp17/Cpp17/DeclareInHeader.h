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
Письмо от: 18.05.2020 9:03
После недавнего обновления Visual Studio 2017 до версии 15.9.23, те кто успел обновиться, 
столкнулись с проблемой – креш компилятора при сборке солюшена Unified Editor:

Но причиной креша компилятора является не само использование static inline variable, а «стечение обстоятельств», а именно:
1)	вызов конструктора Colour (который является псевдонимом struct ColourRGBAFloat) при инициализации;
2)	использование атрибута BW_MOO_DLL (макрос __declspec(dllimport) / __declspec(dllexport)) при объявлении класса.

Мне удалось воспроизвести баг в «песочнице»: без вызова конструктара или без __declspec(dllimport) падение не происходит.
Компилятор падает не зависимо от сборки Debug/Release, x64/x86.

Как ни странно, клиент танков собирается, хотя там тоже присутствует этот код. Возможно другое «стечение обстоятельств» :)

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