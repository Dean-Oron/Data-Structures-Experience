#include <iostream>

#include "point.hpp"
#include "circle.hpp"
#include "factory.hpp"

using namespace ilrd;
extern "C"
{
int symbol_trail = 100;

Point point_symbol(7, 7);

void foo_symbol(int num)
{
    std::cout << "the number is:" << num << std::endl;
}

}