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
#include "rectangle.hpp"

using namespace ilrd;
using namespace std;


Rectangle::Rectangle(RectangleParams params)
                : Shape(params), m_height(params.m_height), m_width(params.m_width)
{
}

void Rectangle::DrawInternal()
{
    Point p1(this->GetPosition().GetX() - this->GetWidth() / 2, this->GetPosition().GetY() - this->GetHeight() / 2);
    p1 = p1.Revolve(this->GetPosition(), this->GetAngle() * M_PI / 180);

    Point p2(this->GetPosition().GetX() - this->GetWidth() / 2, this->GetPosition().GetY() + this->GetHeight() / 2);
    p2 = p2.Revolve(this->GetPosition(), this->GetAngle() * M_PI / 180);   
    
    Point p3(this->GetPosition().GetX() + this->GetWidth() / 2, this->GetPosition().GetY() + this->GetHeight() / 2);
    p3 = p3.Revolve(this->GetPosition(), this->GetAngle() * M_PI / 180);
    
    Point p4(this->GetPosition().GetX() + this->GetWidth() / 2, this->GetPosition().GetY() - this->GetHeight() / 2);
    p4 = p4.Revolve(this->GetPosition(), this->GetAngle() * M_PI / 180);

    DrawPolygon(GetColor().GetBlue(), 4, (int)p1.GetX(), (int)p1.GetY(), 
                                  (int)p2.GetX(), (int)p2.GetY(), 
                                  (int)p3.GetX(), (int)p3.GetY(), 
                                  (int)p4.GetX(), (int)p4.GetY());
}

