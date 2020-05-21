#include <stdio.h>


class A
{
public:
    A()
        //: _int(98989898)
        : _float(97797979.f)
    {
        _int = 98989898;
        printf("\'_int = 98989898;\' : result: int = %d, float = %f, pint = %p\n", _int, _float, (void*)_pint);
        _float = 98989898.f;
        printf("\'_float = 98989898.f;\' : result: int = %d, float = %f, pint = %p\n", _int, _float, (void*)_pint);
        _pint = (int*)0xff00ff0ff;
        printf("\'_pint = (int*)0xff00ff0ff;\' : result: int = %d, float = %f, pint = %p\n", _int, _float, (void*)_pint);
    }
    ~A()
    {

    }

private:
    union
    {
        int   _int;
        float _float;
        int*  _pint;
    };
};


int main()
{
    printf("Hello!\n");
    A a;

    return 0;
}