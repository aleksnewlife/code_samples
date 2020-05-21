
#include <memory>
#include <vector>
#include <iostream>

struct A
{
    explicit A(int a)
        : _a(a)
    {

    }

    ~A()
    {

    }

    int _a;
};




int main(int argc, char const *argv[])
{
    std::vector<std::unique_ptr<A>> _vec;

    auto p = std::make_unique<A>(99090909);
    //std::unique_ptr<A> p2(p);             // ERROR: using copy deleted constructor
    std::unique_ptr<A> p2(std::move(p));    // OK: using move constructor
    std::cout << p2->_a << std::endl;


    _vec.emplace_back(std::move(std::make_unique<A>(435353433)));
    std::cout << _vec[0]->_a << std::endl;

    /* code */
    return 0;
}