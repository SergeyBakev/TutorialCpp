#include <iostream>
#include "kdtree.h"
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include <memory>
#include <functional>

using namespace std::placeholders;
using namespace std::chrono_literals;

using BasePtr = std::shared_ptr<class Base>;
class Base
{
public:
    virtual BasePtr Do() = 0;
};

class AbstractBase : public Base, protected std::enable_shared_from_this<AbstractBase>
{

public:
    virtual BasePtr Do() override 
    {
        OnDo();
        return shared_from_this();
    }

    virtual ~AbstractBase()
    {
        std::cout << "AbstractBase dtor\n";
    }
protected:
    AbstractBase() {};
    virtual void OnDo(){}
};

class A : public AbstractBase
{
public:
    virtual ~A()
    {
        std::cout << "A dtor\n";
    }

 protected:
    virtual void OnDo() override
    {
    
    }
};
int main()
{
    std::shared_ptr<A> a = std::make_shared<A>();
    auto base = a->Do();
    int z = 3;

    {
        std::shared_ptr<A> a = std::make_shared<A>();
        auto base = a->Do();
        int z = 3;
    }
    
    return -1;
}

