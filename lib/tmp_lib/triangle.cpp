#include <iostream>
#include <string>
#include "factory.hpp"
#include "singleton.hpp"
#include "shape.hpp"

using namespace ilrd;

void __attribute__((constructor)) Init();

class Triangle: public Shape
{
public:
    Triangle(ShapeParams params):Shape(params), m_a(10), m_b(10), m_c(10) {}

    int m_a;
    int m_b;
    int m_c;
};

//builder function
Shape *TriangleCreate(ShapeParams params)
{
    Shape *new_tri = new Triangle(params);
    
    return (new_tri);
}

void Init() 
{ 
    Singleton<Factory<std::string, Shape, ShapeParams> >::GetInstance()->Add("triangle", TriangleCreate);
    LOG_INFO("Teached the factory how to build triangle!");
}