/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_RECTANGLE_HPP
#define ILRD_RD8586_RECTANGLE_HPP

#include "point.hpp"
#include "shape.hpp"

namespace ilrd
{

class RectangleParams: public ShapeParams
{
public:
    RectangleParams();
    RectangleParams(Point point, double angle, Color color, double h, double w)
                    :ShapeParams(point, angle, color), m_height(h), m_width(w){}

    double m_height;
    double m_width;
};

class Rectangle: public Shape
{
public:
    Rectangle();
    Rectangle(RectangleParams params);
    
    inline const double& GetWidth() const;
    inline const double& GetHeight() const;
    inline void SetWidth(const double& width);
    inline void SetHeight(const double& height);

    void DrawInternal();
    
private:
    double m_height;
    double m_width;
};



const double& Rectangle::GetWidth() const
{
    return (m_width);
}

const double& Rectangle::GetHeight() const
{
    return (m_height);
}

void Rectangle::SetWidth(const double& w) 
{
    m_width = w;
}

void Rectangle::SetHeight(const double& h) 
{
    m_height = h;
}

} //ilrd
#endif //ILRD_RD8586_LINE_HPP