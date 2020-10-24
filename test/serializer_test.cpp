#include <iostream>
#include <fstream>

#include "shape_lib.hpp"
#include "serilizer.hpp"
#include "factory.hpp"
#include "singleton.hpp"
#include "point.hpp"

int main ()
{
    Point p1(1, 1);
    Color c1(100,100,100);
    CircleParams c_params(p1, 180, c1, 200);
    Circle* circle = new Circle(c_params);
    
    std::fstream fs;
    fs.open("test.txt", std::ios::out);

    Serializer<Shape> seri;

    seri.Add<Circle>();

    seri.Serialize(*circle, fs);

    fs.close();

    fs.open("test.txt", std::ios::in);
    std::cout << "after serilizing is info is" << fs.tellg() << std::endl;
    boost::shared_ptr<Shape> circle_ptr(seri.Deserialize(fs));
    
    int x = 0;
    fs >> x;
    std::cout << "x is " << x <<std::endl;

    std::cout << circle_ptr->GetPosition().GetX() << std::endl;

    std::cout << "memcmp returns: " << memcmp(circle, circle_ptr.get(), sizeof(Circle)) << std::endl;

    std::fstream fs2;
    fs2.open("test2.txt", std::ios::out);

    seri.Serialize(*circle_ptr, fs2);

    return(0);
}