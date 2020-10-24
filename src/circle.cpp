/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>
#include <math.h>

#include "shape.hpp"
#include "point.hpp"
#include "circle.hpp"
#include "color.hpp"

using namespace ilrd;

Circle::Circle(){}
Circle::Circle(CircleParams params):Shape(params), m_radius(params.m_radius)
{
}

void Circle::Rotate(const double& angle)
{
    this->SetAngle(angle);
}

/*Point& Circle::Revolve(const Point& pivot, const double& angle)
{
    double radius = this->GetPosition().Distance(pivot);

}*/

void Circle::Move(const double move_x, const double move_y)
{
    Point p(this->GetPosition().GetX() + move_x, this->GetPosition().GetY() + move_y);
    this->SetPosition(p);
}

void Circle:: DrawInternal()
{
    DrawCircle(GetColor().GetBlue(), GetPosition().GetX(), GetPosition().GetY(), m_radius);
}

std::istream& Circle::operator<<(std::istream& is)
{
    int x;
    int y;
    double angle = 0;
    char red;
    char green;
    char blue;

    is >> x;
    is >> y;
    is >> angle;
    is >> red;
    is >> green;
    is >> blue;
    is >> m_radius;

    SetPosition(Point(x, y));

    SetAngle(angle);
    SetColor(Color(red, green, blue));

    return (is);
}
    
std::ostream& Circle::operator>>(std::ostream& os) const
{
    os << GetPosition().GetX() << " " << GetPosition().GetY() << " ";
    os << GetAngle() << " ";
    os << GetColor().GetRed() << " " << GetColor().GetGreen() << " " << GetColor().GetBlue() << " ";  
    os << m_radius << " ";

    return (os);
}