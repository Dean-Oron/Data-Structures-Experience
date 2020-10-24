#include <string>
#include "shared_object.hpp"
#include "point.hpp"

using namespace ilrd;
typedef void (*trail_func_t)(int);

int main()
{
    trail_func_t foo;
    std::string sym1("symbol_trail");
    std::string sym2("foo_symbol");
    std::string sym3("point_symbol");
    Point *p1 = new Point();

    SharedObject dll("/home/dean/Desktop/Dean/git/cpp/project/lib/libproject.so");
    int *x = dll.LoadSymbol<int>(sym1);
    *(void**)&foo = dll.LoadSymbol<void (*) (int)>(sym2);
    p1 = dll.LoadSymbol<Point>(sym3);
    
    std::cout << "x value is: " << *x << std::endl;
    std::cout << "the point is" << p1->GetX() << ", " << p1->GetY() << std::endl;

    foo(200);
}