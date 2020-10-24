/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_CIRCLE_HPP
#define ILRD_RD8586_CIRCLE_HPP

#include "shape.hpp"
#include "point.hpp"
#include "color.hpp"

namespace ilrd
{

class CircleParams: public ShapeParams
{
public:
    CircleParams():ShapeParams(), m_radius(10){}
    CircleParams(Point point, double angle, Color color, double radius):ShapeParams(point, angle, color), m_radius(radius){}

    double m_radius;
};

class Circle:public Shape
{
public:
    Circle();
    Circle(CircleParams params);
    
    inline const double& GetRadius() const;

    void DrawInternal();

    void Rotate(const double& angle);
    //Point& Revolve(const Point& pivot, const double& angle);
    void Move(const double move_x, const double move_y);
    std::istream& operator<<(std::istream& is);
    std::ostream& operator>>(std::ostream& os) const ;
    
private:
    double m_radius;
};



///////////////////////////////////////////////////////////////////////////////
//                    INLINE FUNCTIONS IMPLEMENTAION                         //
///////////////////////////////////////////////////////////////////////////////

//Circle *CreateCircle()
const double& Circle::GetRadius() const
{
    return (m_radius);
} 

}

#endif //ILRD_RD8586_CIRCLE_HPP

