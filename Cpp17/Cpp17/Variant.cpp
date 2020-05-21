#include "pch.h"
#include "Variant.h"

#include <functional>

std::vector<const char*> TVARIANT_TYPE_TO_TYPE_NAME = { "bool", "int", "unsigned int", "float", "double", "long", "string" };

std::unordered_map<BinaryOperations, const char*> BINARI_OPERATION_TO_NAME = {
	{ BinaryOperations::Unknown, "Unknown" },
	{ BinaryOperations::_Compare, "_Compare" },
	{ BinaryOperations::_Boolean, "_Boolean" },
	{ BinaryOperations::_Math, "_Math" },
	{ BinaryOperations::Equal, "==" },
	{ BinaryOperations::NotEqual, "!=" },
	{ BinaryOperations::Less, "<" },
	{ BinaryOperations::More, ">" },
	{ BinaryOperations::MoreEqual, ">=" },
	{ BinaryOperations::LessEqual, "<=" },
	{ BinaryOperations::And, "And" },
	{ BinaryOperations::Or, "Or" },
	{ BinaryOperations::Xor, "Xor" },
	{ BinaryOperations::Multiply, "*" },
	{ BinaryOperations::Divide, "/" },
	{ BinaryOperations::Add, "+" },
	{ BinaryOperations::Subtract, "-" },
};


std::unordered_map<UnaryOperations, const char*> UNARY_OPERATION_TO_NAME = {
	{ UnaryOperations::Unknown, "Unknown" },
	{ UnaryOperations::_Boolean, "_Boolean" },
	{ UnaryOperations::Not, "Not" },
};


std::unordered_map<unsigned int, std::pair<const char*, const char*>> ERROR_TYPE_TO_STR = {
	{ VariantErrorCodes::NO_ERROR, { "", "OK " } },
	{ VariantErrorCodes::ERROR, { "ERROR", "ERR" } },
	{ VariantErrorCodes::WARNING, { "WARNING", "WRN" } },
};

std::unordered_map<unsigned int, const char*> ERROR_TO_STR = {
	{ VariantErrorCodes::NO_ERROR, "NO_ERROR" },
	{ VariantErrorCodes::BAD_OPERATION, "BAD_OPERATION" },
	{ VariantErrorCodes::FIRST_TYPE_POSIBLE_LOST_DATA, "FIRST_TYPE_POSIBLE_LOST_DATA" },
	{ VariantErrorCodes::SECOND_TYPE_POSIBLE_LOST_DATA, "SECOND_TYPE_POSIBLE_LOST_DATA" },
	{ VariantErrorCodes::UNSUITABLE_TYPES, "UNSUITABLE_TYPES" },
	{ VariantErrorCodes::SIGNED_UNSIGNED_MISMATCH, "SIGNED_UNSIGNED_MISMATCH" },
};



std::string variantToStr(TVariant v1)
{
	auto convertVarintToStr = [](auto& v) -> std::string
	{
		using T = std::decay_t<decltype(v)>;
		if constexpr (std::is_same_v<T, std::string>)
		{
			return v;
		}
		else
		{
			return std::to_string(v);
		}
	};

	return std::visit(convertVarintToStr, v1);
}

unsigned int getVariantErrorText(long long errorValue, TVariant v1, std::string& outText) // returns ErrorType
{
	if (errorValue && 0xff)
	{
		try
		{

			std::string v1_str = variantToStr(v1);

			auto[errorType, errorCode, typeIndexA, typeIndexB, operationIndex] = VariantErrorCodes::unpackErrorValue(errorValue);
			if (errorType == VariantErrorCodes::NO_ERROR)
			{
				return VariantErrorCodes::NO_ERROR;
			}
			std::string& res = outText;
			res += ERROR_TYPE_TO_STR[errorType].first;
			res += ": ( ";
			res += UNARY_OPERATION_TO_NAME[(UnaryOperations)operationIndex];
			res += " " + v1_str + " ) :: ( ";
			res += TVARIANT_TYPE_TO_TYPE_NAME[typeIndexA];
			res += " ) ::: ";
			res += ERROR_TO_STR[errorCode];
			return errorType;
		}
		catch (...)
		{
			std::cout << "Exception in getVariantErrorText\n";
		}
	}
	return VariantErrorCodes::NO_ERROR;
}

unsigned int getVariantErrorText(long long errorValue, TVariant v1, TVariant v2, std::string& outText) // returns ErrorType
{
	if (errorValue && 0xff)
	{
		try
		{

			std::string v1_str = variantToStr(v1);
			std::string v2_str = variantToStr(v2);

			auto[errorType, errorCode, typeIndexA, typeIndexB, operationIndex] = VariantErrorCodes::unpackErrorValue(errorValue);
			if (errorType == VariantErrorCodes::NO_ERROR)
			{
				return VariantErrorCodes::NO_ERROR;
			}
			std::string& res = outText;
			res += ERROR_TYPE_TO_STR[errorType].first;
			res += ": ( " + v1_str + " ";
			res += BINARI_OPERATION_TO_NAME[(BinaryOperations)operationIndex];
			res += " " + v2_str + " )";
			res += " :: ( ";
			res += TVARIANT_TYPE_TO_TYPE_NAME[typeIndexA];
			res += " :: ";
			res += TVARIANT_TYPE_TO_TYPE_NAME[typeIndexB];
			res += " ) ::: ";
			res += ERROR_TO_STR[errorCode];
			return errorType;
		}
		catch (...)
		{
			std::cout << "Exception in getVariantErrorText\n";
		}
	}

	return VariantErrorCodes::NO_ERROR;
}



using TUnPerformerResult = std::tuple<UnaryOperations, TVariant, long long>;

template<UnaryOperations operation>
TUnPerformerResult performUnOperation(TVariant& v1)
{
	auto[vRes, errorValue] = UnaryOperation<operation>::perform(v1);
	return { operation, vRes, errorValue };
}


using TBinPerformerResult = std::tuple<BinaryOperations, TVariant, long long>;

template<BinaryOperations operation>
TBinPerformerResult performBinOperation(TVariant& v1, TVariant& v2)
{
	auto[vRes, errorValue] = BinaryOperation<operation>::perform(v1, v2);
	return { operation, vRes, errorValue };
}


bool testVariantType()
{
	unsigned int RES_OK = VariantErrorCodes::NO_ERROR;
	unsigned int RES_ER = VariantErrorCodes::ERROR;
	unsigned int RES_WR = VariantErrorCodes::WARNING;

	bool variantErrorOccurred = false;
	// Unary Operations
	{
		using TUnOperationMethod = std::function<TUnPerformerResult(TVariant&)>;

		std::vector<std::tuple<unsigned int, TUnOperationMethod, TVariant, std::string>> inVec = {
			{ RES_OK, performUnOperation<UnaryOperations::Not>, true, std::string("0") },
			{ RES_ER, performUnOperation<UnaryOperations::Not>, 234U, std::string("0") },
			{ RES_ER, performUnOperation<UnaryOperations::Not>, 845674, std::string("0") },
			{ RES_ER, performUnOperation<UnaryOperations::Not>, 3453.f, std::string("0") },
			{ RES_ER, performUnOperation<UnaryOperations::Not>, 3094583.0, std::string("0") },
			{ RES_ER, performUnOperation<UnaryOperations::Not>, std::string("teststr"), std::string("0") },
		};

		for (auto& tp : inVec)
		{
			auto[requaredRes, operationPred, v1, reqRes] = tp;
			auto res = operationPred(v1);
			std::string res_str = variantToStr(std::get<1>(res));

			std::string errorText;
			unsigned int errorType = getVariantErrorText(std::get<2>(res), v1, errorText);
			if (errorType == VariantErrorCodes::ERROR)
			{
				res_str = "?????????";
			}
			else if (res_str.compare(reqRes) != 0)
			{
				res_str = "BADRESULT " + res_str;
			}

			std::string resPrefix = ERROR_TYPE_TO_STR[errorType].second;
			resPrefix += "\t";

			variantErrorOccurred = !(errorType == requaredRes);
			std::string validationPrefix = !variantErrorOccurred ? "   " : ">  ";

			std::cout << validationPrefix << resPrefix <<
				UNARY_OPERATION_TO_NAME[std::get<0>(res)] << " " << variantToStr(v1) << " = " << res_str << "\t ::: " <<
				TVARIANT_TYPE_TO_TYPE_NAME[v1.index()] << " | " << errorText <<
				std::endl;

		} // for
	}

	std::cout << "\n\n";

	// Binary Operations
	{

		using TBinOperationMethod = std::function<TBinPerformerResult(TVariant&, TVariant&)>;

		std::vector<std::tuple<unsigned int, TBinOperationMethod, TVariant, TVariant, std::string>> inVec = {
			// String ops
			{ RES_OK, performBinOperation<BinaryOperations::Add>, std::string("first_"), std::string("second"), std::string("first_second") },
			{ RES_ER, performBinOperation<BinaryOperations::Subtract>, std::string("first_"), std::string("second"), std::string("first_second") },
			{ RES_ER, performBinOperation<BinaryOperations::Multiply>, std::string("first_"), std::string("second"), std::string("first_second") },
			{ RES_ER, performBinOperation<BinaryOperations::Add>, std::string("first_"), 1U, std::string("error") },

			// Boolean ops
			{ RES_OK, performBinOperation<BinaryOperations::Or>, true, false, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::And>, true, false, std::string("0") },
			{ RES_OK, performBinOperation<BinaryOperations::Xor>, false, true, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::Equal>, true, true, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::NotEqual>, false, true, std::string("1") },
			{ RES_ER, performBinOperation<BinaryOperations::Add>, true, true, std::string("1") },
			{ RES_ER, performBinOperation<BinaryOperations::Add>, true, true, std::string("1") },

			// General arithmetics
			{ RES_OK, performBinOperation<BinaryOperations::Add>, 1111111, 2222222, std::string("3333333") },
			{ RES_OK, performBinOperation<BinaryOperations::Add>, 1111111U, 2222222U, std::string("3333333") },
			{ RES_OK, performBinOperation<BinaryOperations::Add>, 1111111L, 2222222L, std::string("3333333") },
			{ RES_OK, performBinOperation<BinaryOperations::Add>, 1111111.f, 2222222.f, std::string("3333333.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Add>, 1111111.0, 2222222.0, std::string("3333333.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Subtract>, 1111111, 2222222, std::string("-1111111") },
			{ RES_OK, performBinOperation<BinaryOperations::Subtract>, 2222222U, 1111111U, std::string("1111111") },
			{ RES_OK, performBinOperation<BinaryOperations::Subtract>, 1111111L, 2222222L, std::string("-1111111") },
			{ RES_OK, performBinOperation<BinaryOperations::Subtract>, 1111111.f, 2222222.f, std::string("-1111111.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Subtract>, 1111111.0, 2222222.0, std::string("-1111111.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11, 22, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 22U, 11U, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11L, 22L, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11.f, 22.f, std::string("242.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11.0, 22.0, std::string("242.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11, 22, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 22U, 11U, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11L, 22L, std::string("242") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11.f, 22.f, std::string("242.000000") },
			{ RES_OK, performBinOperation<BinaryOperations::Multiply>, 11.0, 22.0, std::string("242.000000") },

			// Compare untegral and float
			{ RES_OK, performBinOperation<BinaryOperations::Equal>, 100, 100, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::NotEqual>, 100, 50, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::More>, 100, 50, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::Less>, 100, 50, std::string("0") },
			{ RES_OK, performBinOperation<BinaryOperations::MoreEqual>, 100, 50, std::string("1") },
			{ RES_OK, performBinOperation<BinaryOperations::LessEqual>, 50, 150, std::string("1") },

			// Type mismatch
			{ RES_WR, performBinOperation<BinaryOperations::LessEqual>, 50, 150U, std::string("1") },
			{ RES_WR, performBinOperation<BinaryOperations::LessEqual>, -50, 150U, std::string("1") },
			{ RES_WR, performBinOperation<BinaryOperations::LessEqual>, 150U, -50, std::string("1") },
			{ RES_WR, performBinOperation<BinaryOperations::Add>, 150U, -50, std::string("100") },
			{ RES_WR, performBinOperation<BinaryOperations::Multiply>, 4294967295U, -2, std::string("100") },
			{ RES_WR, performBinOperation<BinaryOperations::Subtract>, 4U, 8, std::string("100") },
			{ RES_WR, performBinOperation<BinaryOperations::Divide>, 10000000U, -2, std::string("100") },

		};

		for (auto& tp : inVec)
		{
			auto[requaredRes, operationPred, v1, v2, reqRes] = tp;
			auto res = operationPred(v1, v2);
			std::string res_str = variantToStr(std::get<1>(res));

			std::string errorText;
			unsigned int errorType = getVariantErrorText(std::get<2>(res), v1, v2, errorText);
			if (errorType == VariantErrorCodes::ERROR)
			{
				res_str = "?????????";
			}
			else if (res_str.compare(reqRes) != 0)
			{
				res_str = "BADRESULT " + res_str;
			}

			std::string resPrefix = ERROR_TYPE_TO_STR[errorType].second;
			resPrefix += "\t";
			
			variantErrorOccurred = !(errorType == requaredRes);
			std::string validationPrefix = !variantErrorOccurred ? "   " : ">  ";
		
			std::cout << validationPrefix << resPrefix <<
				variantToStr(v1) << " " << BINARI_OPERATION_TO_NAME[std::get<0>(res)] << " " << variantToStr(v2) << " = " << res_str << "\t ::: " <<
				TVARIANT_TYPE_TO_TYPE_NAME[v1.index()] << "  " << TVARIANT_TYPE_TO_TYPE_NAME[v2.index()] << " | " << errorText <<
				std::endl;
		
		} // for
	}

	if (variantErrorOccurred)
	{
		std::cout << "Error: TVariant errors occurred\n";
	}
	return variantErrorOccurred;
}
