#include <string>
#include <stdio.h>
#include "loader.hpp"
#include "point.hpp"
#include "factory.hpp"
#include "singleton.hpp"
#include "shape_lib.hpp"

using namespace ilrd;
typedef void (*trail_func_t)(int);
#define PLUG_IN_PATH ("/home/dean/Desktop/Dean/git/cpp/project/test/plug_in/")

int main()
{
    std::string class_name;
    DLLoader loader(PLUG_IN_PATH);

    while (1)
    {
        std::cout << "Insert a class name to build" << std::endl;
        std::cin >> class_name;

        ShapeParams params(Point(1, 1), 180, Color(1,1,1));
        Shape *tri = NULL;

        try
        {
            tri = Singleton<Factory<string, Shape, ShapeParams> >::GetInstance()->Create(class_name, params);
        }
        
        catch(const std::exception& e)
        {
            std::cout << "didn't find class of the name " << class_name << '\n';
        }

        if (tri)
        {
            std::cout << tri->GetAngle() <<std::endl;
        }
    }



    return (0);
}