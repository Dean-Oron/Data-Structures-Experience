/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_SHAPE_HPP
#define ILRD_RD8586_SHAPE_HPP

#include <iostream>
#include "glut_utils.h"
#include "point.hpp"
#include "color.hpp"


namespace ilrd
{

class ShapeParams
{
public:
    ShapeParams():m_point(Point()), m_angle(0), m_color(Color()){}
    ShapeParams(Point point, double angle, Color color):m_point(point), m_angle(angle), m_color(color){}

    Point m_point;
    double m_angle;
    Color m_color;
};

class Shape
{
public:
    //default Ctor
    Shape();
    //Ctor
    Shape(ShapeParams params);

    inline const Point& GetPosition() const;
    inline void SetPosition(const Point& pos);

    inline Color GetColor() const;
    inline void SetColor(const Color& rgb);

    inline const double& GetAngle() const;
    inline void SetAngle(const double& angle);

    void Draw();

    virtual void DrawInternal();
    virtual void Rotate(const double& angle);
    virtual void Revolve(const Point& pivot, const double& angle);
    virtual void Move(const double move_x, const double move_y);

    virtual std::istream& operator<<(std::istream& is)  
    {
        return(is);
    }
    virtual std::ostream& operator>>(std::ostream& os) const 
    {
        return (os);
    }

private:

    ShapeParams m_params;
};

void Shape::SetPosition(const Point& pos)
{
    m_params.m_point = pos;
}

void Shape::SetColor(const Color& rgb)
{
    m_params.m_color = rgb;
}

void Shape::SetAngle(const double& angle)
{
    m_params.m_angle = angle;
}

const Point& Shape::GetPosition() const
{
    return (m_params.m_point);
}

Color Shape::GetColor() const
{
    return (m_params.m_color);
}

const double& Shape::GetAngle() const
{
    return (m_params.m_angle);
}

} // ilrd
#endif //ILRD_RD8586_SHAPE_HPP