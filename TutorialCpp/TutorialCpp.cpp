#include <iostream>
#include "kdtree.h"
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include <functional>

using namespace std::placeholders;
using namespace std::chrono_literals;



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

double func(double a,double b)
{
    return a + b;
}
int main()
{
    auto f = std::bind(func, 2, _1);
    double tes  = f(5,3);
   
    return -1;
}

