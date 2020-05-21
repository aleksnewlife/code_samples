// Cpp17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Variant.h"
#include "DeclareInHeader.h"
#include "FoldExpressions.h"
#include "Delayed_call_tuple_unpack.h"
#include "Independent_Dependendt_calls.h"
#include "Move_Semantic.h"

#include <iostream>

int main()
{
	/*
		
	*/
	if (true)
	{
		std::cout << ">>>>C++ 17 :: static const/vars/static inline variables \n\n"; 
		testDeclareInHeader();
		/*
			 онстанты/переменные/функции в header:

			InternalLinking - внутренн€€ компоновка
			ExternalLinking - внешн€€ компоновка
			HybridLinking - гибридна€ компоновка, по€вилась в Cpp17


			¬се константы в не блоке (€ так понимаю класа), сохран€ютс€ в статической пам€ти, по этому слово static скорее игнорируетс€
				const - (InternalLinkin) gдл€ каждой единицы компил€ции сво€ (дл€ констант в хеадере используетс€ внутренн€€ компонова-линковка)
				static const - (InternalLinkin) дл€ каждой единицы компил€ции сво€ (по сути не имеет разницы с педыдущим обь€влением)
				static const inline - (InternalLinkin) дл€ каждой единицы компил€ции сво€

			static inline - (InternalLinkin) дл€ каждой единицы компил€ции сво€ - тоже что "static var", € так понимаю inline здесь игнорируетс€
			
			var - (ExternalLinking) по умолчанию "int var;" €вл€етс€ обь€влением и определением, по этому при включении хеадера в разные единицы трансл€ции
				будет дублироватьс€ и линкер будет говорить об ошибке линковки данного символа в разных .cpp
			extern var - (ExternalLinking) это "extern int var;" €вл€етс€ только обь€влением, по этому требудет определить в одной единицы трансл€ции 
				эту переменную как "int var;" или "int var = 348957;"
			function - (ExternalLinking) (можно и со словом "extern") дл€ функций как и дл€ переменных используетс€ внешн€€ компоновка, по этому 
				при определении функции в одной единицы трансл€ции (.cpp), линковщик св€жет с ней всех, кто ее использует


			¬се переменные в классе по умолчанию хран€тс€ в хипе, по этому нужно указывать static дл€ хранени€ в статической пам€ти:
			const - in class - одна дл€ экземпл€ра класса (существует только в рамках экземпл€ра обьекта класса)
			static const - in class - (ExternalLinking) одна дл€ всех, так как обь€вление в хеадере класа, а определение в теле одной единицы трансл€ции (.cpp)
			static const inline - in class - (HybridLinking) создаютс€ дл€ каждой единици трансл€ции свой экземпл€р, 
				Ќќ используетс€ везде первый экземпл€р

		*/

		printObjectCounters();
		std::cout << "(Cpp17.cpp) static const inline, ref: " << (int)&STR3 << std::endl;
		std::cout << "(Cpp17.cpp) static inline, ref: " << (int)&STR4 << std::endl;

		std::cout << "(Cpp17.cpp) static const inline in class, counte ref: " << (int)&Object<7>::__counter << std::endl;
		std::cout << "(Cpp17.cpp) static inline in class, counte ref: " << (int)&Object<8>::__counter << std::endl;

		std::cout << "-------------\n\n";
	}

	if (false)
	{
		std::cout << ">>>>>C++ 17 :: std::variant \n\n";
		testVariantType();
		std::cout << "-------------\n\n";
	}

	if (true)
	{
		std::cout << ">>>>>C++ 17 :: fold expressions \n\n";
		test_fold_expressions();
		std::cout << "-------------\n\n";
	}

	if (true)
	{
		std::cout << ">>>>>C++ 17 :: saved callbacks \n\n";
		test_simpleSavedCallback();
		std::cout << "-------------\n\n";
	}

	if (true)
	{
		std::cout << ">>>>>C++ 17 :: independent dependent calls \n\n";
		test_Independent_Dependent_Calls();
		std::cout << "-------------\n\n";
	}

	if (true)
	{
		std::cout << ">>>>>C++ 11 :: move semantic \n\n";
		test_MoveSemantic();
		std::cout << "-------------\n\n";
	}

	return 0;
}
