/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>

#include "shape.hpp"
#include "point.hpp"
#include "glut_utils.h"

using namespace ilrd;

//Default Ctor
Shape::Shape(): m_params(ShapeParams())
{

}

//Shape Ctor
Shape::Shape(ShapeParams params): m_params(ShapeParams(params.m_point, params.m_angle, params.m_color)) {}

void Shape:: Draw() 
{
    this->DrawInternal();
} 

void Shape::Rotate(const double& angle)
{
    this->SetAngle(angle);
}

void Shape::Move(const double move_x, const double move_y)
{
    Point p(this->GetPosition().GetX() + move_x, this->GetPosition().GetY() + move_y);
    this->SetPosition(p); 
}
void Shape::DrawInternal() {}

void Shape::Revolve(const Point& pivot, const double& angle)
{
    Point& new_point = m_params.m_point.Revolve(pivot, angle);

    this->SetPosition(new_point);
}