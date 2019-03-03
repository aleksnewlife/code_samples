#pragma once

#include "CommonTypes.h"
#include <iostream>

template<typename TVal1, typename TVal2>
void compareVariant(TVal1 val_1, TVal2 val_2)
{
	// Compare
	TValueHolder v1, v2;

	v1._value = val_1;
	v2._value = val_2;

	std::cout << "TEST values: " << val_1 << " : " << val_2 << std::endl;

	bool res = (v1._value == v2._value);
	std::cout << "(v1._value == v2._value) : " << res << std::endl;

	res = (v1 == v2);
	std::cout << "(v1 == v2) : " << res << " :: Error " << v1._intTypeError << std::endl;

	res = (v1 != v2);
	std::cout << "(v1 != v2) : " << res << " :: Error " << v1._intTypeError << std::endl;

	res = (v1 > v2);
	std::cout << "(v1 > v2) : " << res << " :: Error " << v1._intTypeError << std::endl;

	res = (v1 < v2);
	std::cout << "(v1 < v2) : " << res << " :: Error " << v1._intTypeError << std::endl;

}
