#pragma once


#include <string>
#include <variant>


using TID = unsigned int;

using TVariant = std::variant<TID, int, float, std::string>;


struct TValueHolder
{
	TVariant _value;
	int _intTypeError = 0;

	template<typename T>
	T* getValuePtr()
	{
		_intTypeError = 0;
		return std::get_if<T>(&_value);
	}

	template<typename T>
	const T& getValue()
	{
		return _value;
	}

	int copmareTypes(const TValueHolder& a)
	{
		return _value.index() - a._value.index();
	}

	bool operator==(const TValueHolder& a)
	{
		if ((_intTypeError = copmareTypes(a)) != 0)
			return false;

		return _value == a._value;
	}

	bool operator!=(const TValueHolder& a)
	{
		if ((_intTypeError = copmareTypes(a)) != 0)
			return false;

		return _value != a._value;
	}

	bool operator>(const TValueHolder& a)
	{
		if ((_intTypeError = copmareTypes(a)) != 0)
			return false;

		return _value > a._value;
	}

	bool operator<(const TValueHolder& a)
	{
		if ((_intTypeError = copmareTypes(a)) != 0)
			return false;
	
		return _value < a._value;
	}


};