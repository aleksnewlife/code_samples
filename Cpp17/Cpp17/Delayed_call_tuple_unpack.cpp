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



template<typename TObj, typename ...TArgs>
struct ObjectCallBack
{
    using TMethod = void(TObj::*)(TArgs...);

    ObjectCallBack(TObj* obj, TMethod meth)
        : _obj(obj)
        , _meth(meth)
    {

    }

    void operator()(TArgs... args)
    {
        (_obj->*_meth)(args...);
    }

    TObj* _obj;
    TMethod _meth;
};


template<typename TObj, void(TObj::*TMeth)(int, float, double)>
struct ObjectCallBack2
{
    ObjectCallBack2(TObj* obj)
        : _obj(obj)
    {

    }
       
    void operator()(int a, float b, double c)
    {
        (_obj->*TMeth)(a, b, c);
    }

    TObj* _obj;
};



template<typename TObj, auto TMeth, typename ...TArgs>
struct ObjectCallBack3
{

    ObjectCallBack3(TObj* obj)
        : _obj(obj)
    {

    }

    void operator()(TArgs... args)
    {
        (_obj->*TMeth)(args...);
    }

    TObj* _obj;
};



double foo(int x, float y, double z)
{
  return x + y + z;
}


class TestClass
{
public:
    TestClass()
    {

    }

    void methodOne()
    {
        std::cout << "\tcalled TestClass.methodOne()" << std::endl;
    }

    void methodTwo(int a, float b, double c)
    {
        std::cout << "\tcalled TestClass.methodTwo() " << a << " " << b << " " << c << std::endl;
    }
};

void test_simpleSavedCallback(void)
{
  std::tuple<int, float, double> t = std::make_tuple(1, 1.2f, 5);
  save_it_for_later<double, int, float, double> saved (&foo, t);
  std::cout << saved.delayed_dispatch() << std::endl;
}

void test_objectCallback()
{
    TestClass obj;

    ObjectCallBack<TestClass> callback(&obj, &TestClass::methodOne);
    callback();

    ObjectCallBack2<TestClass, &TestClass::methodTwo> callback2(&obj);
    callback2(1000000, 2000000.f, 10000000.0);

    ObjectCallBack3<TestClass, &TestClass::methodTwo, int, float, double> callback3(&obj);
    callback3(1, 2.f, 3.0);
}

