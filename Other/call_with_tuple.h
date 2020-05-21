#pragma once

#include <cstddef>
#include <utility>
#include <tuple>

template <class TObj, class TFunc, class TRet, class Tuple, std::size_t... I>
constexpr TRet call_with_tuple(TObj* obj, TFunc func, Tuple&& t, std::index_sequence<I...>)
{
	return (obj->*func)(std::get<I>(std::forward<Tuple>(t))...);
}

template <class T, typename TRet, typename...Args>
class CallFunc
{
	CallFunc() = delete;
public:
	using TCallback = TRet(T::*)(Args...);
	using TParamTuple = std::tuple<Args...>;

	CallFunc(T* obj, TCallback memFunction)
		: _bCallWithTuple(false)
		, _obj(obj)
		, _func(memFunction)
	{

	}

	CallFunc(T* obj, TCallback memFunction, Args...args)
		: _bCallWithTuple(true)
		, _obj(obj)
		, _func(memFunction)
	{
		_params = std::make_tuple(args...);
	}

	TRet invoke(Args...args)
	{
		if (!_bCallWithTuple && _obj && _func)
		{
			return (_obj->*_func)(args...);
		}
		return (TRet)0;
	}

	TRet invoke2()
	{
		if (_bCallWithTuple && _obj && _func)
		{
			return call_with_tuple<T, TCallback, TRet, TParamTuple>(_obj, _func, 
				std::forward<TParamTuple>(_params),
				std::make_index_sequence<std::tuple_size_v<std::decay_t<TParamTuple>>>{}
			);
		}
		return (TRet)0;
	}

private:
	bool _bCallWithTuple;
	T* _obj;
	TCallback _func;
	TParamTuple _params;

};


