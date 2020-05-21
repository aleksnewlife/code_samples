#pragma once
#include <variant>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <type_traits>

/*
	TVariant operations version 1.0 (04.04.2019) (c) Aleksandr Gladkiy
	TVariant operations version 1.1 (04.04.2019) (c) Aleksandr Gladkiy (added unary operations)
*/

using TVariant = std::variant<bool, int, unsigned int, float, double, long, std::string>;

enum class BinaryOperations {
	Unknown = 0,

	_Compare = 0x0001,
	_Boolean = 0x0002,
	_Math = 0x0004,

	_Boolean_Compare = _Boolean | _Compare,
	_Compare_Math = _Compare | _Math,
	_All = _Compare | _Boolean | _Math,
	// Compare
	Equal		= _Compare | 0x0100,
	NotEqual	= _Compare | 0x0200,
	Less		= _Compare | 0x0300,
	More		= _Compare | 0x0400,
	MoreEqual	= _Compare | 0x0500,
	LessEqual	= _Compare | 0x0600,
	// Boolean
	And			= _Boolean | 0x0100,
	Or			= _Boolean | 0x0200,
	Xor			= _Boolean | 0x0300,
	// Not, - special operaton

	// Math
	Multiply	= _Math | 0x0100,
	Divide		= _Math | 0x0200,
	Add			= _Math | 0x0300,
	Subtract	= _Math | 0x0400,
};

enum class UnaryOperations {
	Unknown = 0,
	_Boolean = 0x0001,

	Not = _Boolean | 0x0100
};

struct VariantErrorCodes
{
	// Bits: 06 05 0403 02 01 - 
	// 01 - Error Type
	// 02 - Error Code
	// 0403 - operation index (2 bytes)
	// 05 - Fisrt type index (type A)
	// 06 - Second type index (type B)

	static const unsigned int NO_ERROR = 0;
	static const unsigned int ERROR = 1;
	static const unsigned int WARNING = 2;
	// Range 1-255
	static const unsigned int BAD_OPERATION = 1;
	static const unsigned int FIRST_TYPE_POSIBLE_LOST_DATA = 2;
	static const unsigned int SECOND_TYPE_POSIBLE_LOST_DATA = 3;
	static const unsigned int UNSUITABLE_TYPES = 4;
	static const unsigned int SIGNED_UNSIGNED_MISMATCH = 5;

	// Offsets
	static const unsigned int OPERATION_ERRORS_CODE_offset = 1 * 8; // bits
	static const unsigned int OPERATION_INDEX_offset = 2 * 8; // bits
	static const unsigned int TYPEA_INDEX_offset = 4 * 8; // bits
	static const unsigned int TYPEB_INDEX_offset = 5 * 8; // bits
	// Masks
	static const long long __ONE_BYTE_MASK = 0xff;
	static const long long __TWO_BYTE_MASK = 0xffff;


	static long long packErrorCode(unsigned int errorType, unsigned int errorCode, //BinaryOperations op, 
		unsigned int typeIndexA = 0, unsigned int typeIndexB = 0)
	{
		long long errorValue = (errorType & __ONE_BYTE_MASK);
		errorValue = errorValue | ((errorCode & __ONE_BYTE_MASK) << OPERATION_ERRORS_CODE_offset);
		//errorValue = errorValue | (((long long)op & __TWO_BYTE_MASK) << OPERATION_INDEX_offset);
		errorValue = errorValue | ((long long)(typeIndexA & __ONE_BYTE_MASK) << TYPEA_INDEX_offset);
		errorValue = errorValue | ((long long)(typeIndexB & __ONE_BYTE_MASK) << TYPEB_INDEX_offset);
		return errorValue;
	}

	static void addOperationCode(long long& errorValue, long long operationCode)
	{
		errorValue = errorValue | ((operationCode & __TWO_BYTE_MASK) << OPERATION_INDEX_offset);
	}

	static void addTypes(long long& errorValue, unsigned int typeIndexA, unsigned int typeIndexB)
	{
		errorValue = errorValue | ((long long)(typeIndexA & __ONE_BYTE_MASK) << TYPEA_INDEX_offset);
		errorValue = errorValue | ((long long)(typeIndexB & __ONE_BYTE_MASK) << TYPEB_INDEX_offset);
	}

	static std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> unpackErrorValue(long long errorValue)
	{
		unsigned int errorType = errorValue & __ONE_BYTE_MASK;
		unsigned int errorCode = (errorValue >> OPERATION_ERRORS_CODE_offset) & __ONE_BYTE_MASK;
		unsigned int operationIndex = (errorValue >> OPERATION_INDEX_offset) & __TWO_BYTE_MASK;
		unsigned int typeIndexA = (errorValue >> TYPEA_INDEX_offset) & __ONE_BYTE_MASK;
		unsigned int typeIndexB = (errorValue >> TYPEB_INDEX_offset) & __ONE_BYTE_MASK;
		return { errorType, errorCode, typeIndexA, typeIndexB, operationIndex };
	}
};


template<typename T, UnaryOperations operation>
auto unaryOperator(const T& val)
{
	if constexpr (operation == UnaryOperations::Not)
	{
		return !val;
	}
	else
	{
		return val;
	}
}


template<typename TA, typename TB, BinaryOperations operation>
auto binaryOperator(const TA& a, const TB& b)
{
	// Compare
	if constexpr (operation == BinaryOperations::Equal)
	{
		return a == b;
	}
	else if constexpr (operation == BinaryOperations::NotEqual)
	{
		return a != b;
	}
	else if constexpr (operation == BinaryOperations::Less)
	{
		return a < b;
	}
	else if constexpr (operation == BinaryOperations::More)
	{
		return a > b;
	}
	else if constexpr (operation == BinaryOperations::MoreEqual)
	{
		return a >= b;
	}
	else if constexpr (operation == BinaryOperations::LessEqual)
	{
		return a <= b;
	}

	// Boolean
	else if constexpr (operation == BinaryOperations::And)
	{
		return a && b;
	}
	else if constexpr (operation == BinaryOperations::Or)
	{
		return a || b;
	}
	else if constexpr (operation == BinaryOperations::Xor)
	{
		return a ^ b;
	}

	// Math
	else if constexpr (operation == BinaryOperations::Add)
	{
		return a + b;
	}
	else if constexpr (operation == BinaryOperations::Subtract)
	{
		return a - b;
	}
	else if constexpr (operation == BinaryOperations::Multiply)
	{
		return a * b;
	}
	else if constexpr (operation == BinaryOperations::Divide)
	{
		return a / b;
	}
	else
	{
		return a;
	}
}


#define ALLOW_OPERATION_MASK(MASK) ((unsigned int)operation & (unsigned int)MASK) > 0
#define ALLOW_OPERATION(OP) operation == OP

template<UnaryOperations operation>
struct UnaryOperation
{
	static std::pair<TVariant, long long> perform(TVariant& v1)
	{
		static auto predicat = [](auto& val) -> std::pair<TVariant, long long>
		{
			long long errorValue = VariantErrorCodes::NO_ERROR;
			using T = std::decay_t<decltype(val)>;
			T res = val;
			if constexpr (std::is_same_v<T, bool>)
			{
				if constexpr (ALLOW_OPERATION_MASK(UnaryOperations::_Boolean))
				{
					res = unaryOperator<T, operation>(val);
				}
				else
				{
					errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
				}
			}
			else
			{
				errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
			}

			VariantErrorCodes::addOperationCode(errorValue, (long long)operation);
			return std::make_pair(res, errorValue);
		};

		std::pair<TVariant, long long> res = std::visit(predicat, v1);
		VariantErrorCodes::addTypes(res.second, v1.index(), 0);
		return res;
	}
};


template<BinaryOperations operation>
struct BinaryOperation
{
	static const BinaryOperations op_value = operation;

	static std::pair<TVariant, long long> perform(TVariant& v1, TVariant& v2)
	{

		static auto predicat = [](auto& a, auto& b) -> std::pair<TVariant, long long>
		{
			long long errorValue = VariantErrorCodes::NO_ERROR;
			using TA = std::decay_t<decltype(a)>;
			using TB = std::decay_t<decltype(b)>;
			TA res = a;

			if constexpr (std::is_same_v<TA, std::string> && std::is_same_v<TB, std::string>)
			{
				if constexpr (ALLOW_OPERATION(BinaryOperations::Add))
				{
					res = binaryOperator<TA, TB, operation>(a, b);
				}
				else
				{
					errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
				}
			}
			else if constexpr (std::is_same_v<TA, bool> && std::is_same_v<TB, bool>)
			{
				if constexpr (ALLOW_OPERATION_MASK(BinaryOperations::_Boolean_Compare))
				{
					res = binaryOperator<TA, TB, operation>(a, b);
				}
				else
				{
					errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
				}
			}
			else if constexpr (std::is_floating_point_v<TA> && std::is_floating_point_v<TB>)
			{
				if constexpr (ALLOW_OPERATION_MASK(BinaryOperations::_Compare_Math))
				{
					if constexpr (sizeof(TA) < sizeof(TB))
					{
						errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::WARNING, VariantErrorCodes::SECOND_TYPE_POSIBLE_LOST_DATA);
						res = binaryOperator<TA, TA, operation>(a, (TA&)b);
					}
					else
					{
						res = binaryOperator<TA, TB, operation>(a, b);
					}
				}
				else
				{
					errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
				}
			}
			else if constexpr (std::is_integral_v<TA> && std::is_integral_v<TB>)
			{
				// About mixing types: https://stackoverflow.com/questions/25609091/what-happens-when-i-mix-signed-and-unsigned-types

				if constexpr (ALLOW_OPERATION_MASK(BinaryOperations::_Compare_Math))
				{
					if constexpr (sizeof(TA) > sizeof(TB))
					{
						res = binaryOperator<TA, TA, operation>(a, (TA&)b);
					}
					else if constexpr (sizeof(TA) < sizeof(TB))
					{
						res = binaryOperator<TB, TB, operation>((TB&)a, b);
					}
					else if constexpr (std::is_unsigned_v<TA> && std::is_signed_v<TB>)
					{
						errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::WARNING, VariantErrorCodes::SIGNED_UNSIGNED_MISMATCH);
						res = binaryOperator<TA, TA, operation>(a, (TA&)b);
					}
					else if constexpr (std::is_signed_v<TA> && std::is_unsigned_v<TB>)
					{
						errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::WARNING, VariantErrorCodes::SIGNED_UNSIGNED_MISMATCH);
						res = binaryOperator<TB, TB, operation>((TB&)a, b);
					}
					else
					{
						res = binaryOperator<TA, TB, operation>(a, b);
					}
				}
				else
				{
					errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::BAD_OPERATION);
				}
			}
			else
			{
				errorValue = VariantErrorCodes::packErrorCode(VariantErrorCodes::ERROR, VariantErrorCodes::UNSUITABLE_TYPES);
			}

			VariantErrorCodes::addOperationCode(errorValue, (long long)operation);
			return std::make_pair(res, errorValue);
		};

		std::pair<TVariant, long long> res = std::visit(predicat, v1, v2);
		VariantErrorCodes::addTypes(res.second, v1.index(), v2.index());
		return res;
	};
};

// Helpers
extern std::vector<const char*> TVARIANT_TYPE_TO_TYPE_NAME;
extern std::unordered_map<BinaryOperations, const char*> BINARI_OPERATION_TO_NAME;
extern std::unordered_map<UnaryOperations, const char*> UNARY_OPERATION_TO_NAME;


extern std::string variantToStr(TVariant v1);
extern unsigned int getVariantErrorText(long long errorValue, TVariant v1, TVariant v2, std::string& outText); // returns ErrorType
extern unsigned int getVariantErrorText(long long errorValue, TVariant v1, std::string& outText); // returns ErrorType
extern bool testVariantType(); // Tests for TVariant
