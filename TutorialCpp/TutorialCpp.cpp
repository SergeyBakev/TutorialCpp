#include <iostream>
#include "kdtree.h"
#include <vector>

double sum()
{
    std::cout << "Sum\n";
    return 0;
}

double fake_sum()
{
    std::cout << "Sum\n";
    return 0;
}

//using FPtr = decltype(&Foo::sum);

typedef double(*f)(void);

//struct Trigger
//{
//    FPtr f1_;
//    FPtr f2_;
//} trigger{ &Foo::sum,&Foo::fake_sum };

struct Trigger2
{
    using Type1 = f;
    using Type2 = f;
    Type1 f1_;
    Type2 f2_;
};

struct test
{
    __int64 t = 1;
    __int64 t2 = 2;
    __int64 t3 = 3;
};
template<typename T, typename R>
void* void_cast(R(T::* f)())
{
    union
    {
        R(T::* pf)();
        void* p;
    };
    pf = f;
    return p;
}
constexpr Trigger2 trigger2{ sum,fake_sum };
int main()
{
    ((f)&trigger2)();
    
  



    return 0;
}

