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
//#include "color.hpp"
#include "line.hpp"

using namespace ilrd;

Line::Line(LineParams params): m_params(params)
{
}

void Line::DrawInternal() const
{
    std::cout << "Draw Internal" << std::endl;
    //DrawPolygon(COLOR_MAGENTA, 4, (int)150, (int)400, (int)150, (int)650);
}

void Line::Rotate(const double& angle)
{
    this->SetAngle(angle);
}

/*Point& Line::Revolve(const Point& pivot, const double& angle)
{
    double radius = this->GetPosition().Distance(pivot);

}*/

void Line::Move(const double move_x, const double move_y)
{
    Point p(this->GetPosition().GetX() + move_x, this->GetPosition().GetY() + move_y);
    this->SetPosition(p);
}