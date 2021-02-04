#include <iostream>
#include "kdtree.h"
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include "Trigger.h"

using namespace std::chrono_literals;

#include "Algortithms.h"
using DoubleVector = std::vector<double>;
using DoubleVectors = std::vector<DoubleVector>;

bool IsLessThanLimitVer(std::wstring curentVer, std::wstring limitVer)
{
    Algorithms::String::trim_local(curentVer);
    Algorithms::String::trim_local(limitVer);

    
    std::vector<std::wstring> lexemCurentVer = Algorithms::String::Split(L".",curentVer);
    if (lexemCurentVer.size() <= 2)
        return false;

    if (lexemCurentVer.size() >= 3 && limitVer.empty())
        return true;

    std::vector<std::wstring> lexemLimitVer = Algorithms::String::Split(L".", limitVer);
    if (lexemLimitVer.size() <= 2)
        return false;

    size_t size = lexemCurentVer.size();
    bool rc = true;
    for (size_t i = 0; i < size; i++)
    {
        long long cur = std::stoll(lexemCurentVer[i]);
        long long lim = std::stoll(lexemLimitVer[i]);
        if (cur > lim)
        {
            rc = false;
            break;
        }
    }

    return rc;
}



struct Foo
{
    int a;
    Foo() { a = 10; }
    virtual ~Foo() = default;

    void bar()
    {
        std::cout << "Foo::bar\n";
    }

    void fake_bar()
    {
        std::cout << "Foo::fake_bar\n";
    }

    double bar1()
    {
        std::cout << "Foo::bar1\n";
        return 0.0;
    }

    double fake_bar1()
    {
        std::cout << "Foo::fake_bar1\n";
        return 0.234340;
    }


    double bar2()
    {
        std::cout << "Foo::bar3\n";
        return 0.0;
    }

    double fake_bar2()
    {
        std::cout << "Foo::fake_bar2\n";
        return 0.0;
    }

    double bar3(double bar)
    {
        return bar * 2;
    }

    double fake_bar3(double bar)
    {
        return bar * 2;
    }
};


//DECLARE_FUNC_TYPE(Foo::bar, TestBar);
//DECLARE_FUNC_TYPE(Foo::bar, FakeBar);
//
//DECLARE_ROOT_TRIEGGER(Test, Foo::bar, Foo::fake_bar);
//
//
//#define CALL_TRIGGER(NAME) __TrigerStruct__##NAME._Func1


void watch_dog(bool stop)
{
    std::chrono::duration<double> delta = 3.5s;
    while (!stop)
    {
        
        auto start_sleep = std::chrono::system_clock::now();
        std::this_thread::sleep_for(3s);
        auto end_sleep = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_sleep - start_sleep;
        if (elapsed_seconds > delta)
            std::cout << "Program in debug!!!\n";
    }
    

}

class WatchDog
{
public:

    void Run()
    {
        _thread = std::thread(&WatchDog::Loop,this);
    }

    void Stop()
    {
        _stoped = true;
    }

private:
    void Loop()
    {
        std::chrono::duration<double> delta = 3.5s;
        while (!_stoped)
        {

            auto start_sleep = std::chrono::system_clock::now();
            std::this_thread::sleep_for(3s);
            auto end_sleep = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end_sleep - start_sleep;
            if (elapsed_seconds > delta)
                std::cout << "Program in debug!!!\n";
        }
    }

private:
    bool _stoped = false;
    std::thread _thread;
};

void Test1()
{
    std::cout << "Test1\n";
}

void FakeTest1()
{
    std::cout << "Fake Test2\n";
}

struct Test
{
    
};

namespace A
{
    int test;
}

namespace B
{
    
//    A::test = 3;
    void ttt() { A::test = 3; };
}

extern "C" {
    struct ghgh
    {
        int a;
        int b;
    }test;
}

int main()
{
    int a[3];
    std::cout << test.a;
    std::cout << test.b;
    A::test = 3;
    //int z = 3;
    //Test2 t;
   // Visit(&__TrigerStruct__Test);
}

