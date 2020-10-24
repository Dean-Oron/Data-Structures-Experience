#include <iostream>
#include "dispatcher.hpp"

#include "logger.hpp"

void func1(int num)
{
    std::cout << "my number is " << num << std::endl;
}

void func2(int num)
{
    std::cout << "my number is " << num << std::endl;
}

void func3(int num)
{
    std::cout << "my number is " << num << std::endl;
}

void func4(int num)
{
    std::cout << "my number is " << num << std::endl;
}

int main()
{
    Dispatcher<int> dispatch;
    Callback<Dispatcher<int>> *c1 = new Callback<Dispatcher<int>>(func1);
    Callback<Dispatcher<int>> *c2 = new Callback<Dispatcher<int>>(func2);
    Callback<Dispatcher<int>> *c3 = new Callback<Dispatcher<int>>(func3);
    Callback<Dispatcher<int>> *c4 = new Callback<Dispatcher<int>>(func4);

    dispatch.Subscribe(c1);
    dispatch.Subscribe(c2);
    dispatch.Subscribe(c3);
    dispatch.Subscribe(c4);

    dispatch.Notify(7);

    return (0);
}