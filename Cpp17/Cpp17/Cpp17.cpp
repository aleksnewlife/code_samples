// Cpp17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Variant.h"
#include "DeclareInHeader.h"
#include "FoldExpressions.h"
#include "Delayed_call_tuple_unpack.h"
#include "Independent_Dependendt_calls.h"

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
			���������/����������/������� � header:

			InternalLinking - ���������� ����������
			ExternalLinking - ������� ����������
			HybridLinking - ��������� ����������, ��������� � Cpp17


			��� ��������� � �� ����� (� ��� ������� �����), ����������� � ����������� ������, �� ����� ����� static ������ ������������
				const - (InternalLinkin) g��� ������ ������� ���������� ���� (��� �������� � ������� ������������ ���������� ���������-��������)
				static const - (InternalLinkin) ��� ������ ������� ���������� ���� (�� ���� �� ����� ������� � ��������� �����������)
				static const inline - (InternalLinkin) ��� ������ ������� ���������� ����

			static inline - (InternalLinkin) ��� ������ ������� ���������� ���� - ���� ��� "static var", � ��� ������� inline ����� ������������
			
			var - (ExternalLinking) �� ��������� "int var;" �������� ����������� � ������������, �� ����� ��� ��������� ������� � ������ ������� ����������
				����� ������������� � ������ ����� �������� �� ������ �������� ������� ������� � ������ .cpp
			extern var - (ExternalLinking) ��� "extern int var;" �������� ������ �����������, �� ����� �������� ���������� � ����� ������� ���������� 
				��� ���������� ��� "int var;" ��� "int var = 348957;"
			function - (ExternalLinking) (����� � �� ������ "extern") ��� ������� ��� � ��� ���������� ������������ ������� ����������, �� ����� 
				��� ����������� ������� � ����� ������� ���������� (.cpp), ��������� ������ � ��� ����, ��� �� ����������


			��� ���������� � ������ �� ��������� �������� � ����, �� ����� ����� ��������� static ��� �������� � ����������� ������:
			const - in class - ���� ��� ���������� ������ (���������� ������ � ������ ���������� ������� ������)
			static const - in class - (ExternalLinking) ���� ��� ����, ��� ��� ���������� � ������� �����, � ����������� � ���� ����� ������� ���������� (.cpp)
			static const inline - in class - (HybridLinking) ��������� ��� ������ ������� ���������� ���� ���������, 
				�� ������������ ����� ������ ���������

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


	return 0;
}
