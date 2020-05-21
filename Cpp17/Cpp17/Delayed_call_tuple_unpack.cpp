#include "pch.h"
#include <tuple>
#include <iostream>

// https://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

template <typename RetT, typename ...Args>
struct save_it_for_later
{
    RetT (*func)(Args...);
    std::tuple<Args...> params;

    save_it_for_later(RetT (*f)(Args...), std::tuple<Args...> par) : func { f }, params { par } {}

    RetT delayed_dispatch()
    {
        return callFunc(std::index_sequence_for<Args...>{});
    }

    template<std::size_t... Is>
    RetT callFunc(std::index_sequence<Is...>)
    {
        return func(std::get<Is>(params) ...);
    }
};

double foo(int x, float y, double z)
{
  return x + y + z;
}

int test_simpleSavedCallback(void)
{
  std::tuple<int, float, double> t = std::make_tuple(1, 1.2, 5);
  save_it_for_later<double, int, float, double> saved (&foo, t);
  std::cout << saved.delayed_dispatch() << std::endl;
  return 0;
}