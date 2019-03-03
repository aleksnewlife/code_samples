#include "pch.h"
#include "CommonTypes.h"
#include "VariantTests.h"
#include <iostream>

void checkVariant()
{
	{
		// get TID value
		TValueHolder vholder;
		vholder._value = TID(99094923);
		auto pValue = vholder.getValuePtr<TID>();
		std::cout << *pValue << std::endl;
	}

	compareVariant(2342342.f, 2342342.f);
	compareVariant(2342342, 2342342.f);
	compareVariant(2342343.f, 2342342.f);
	compareVariant(2342343.f, std::string("dummy str"));

}



int main()
{
    std::cout << "Start!\n";
	checkVariant();
	std::cout << "Finish...\n";
}
