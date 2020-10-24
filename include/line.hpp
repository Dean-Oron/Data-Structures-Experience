/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_LINE_HPP
#define ILRD_RD8586_LINE_HPP

#include "point.hpp"
#include "shape.hpp"

namespace ilrd
{

class LineParams: public ShapeParams
{
public:
    LineParams():ShapeParams(), m_length(20){}
    LineParams(Point point, double angle, Color color, double len):ShapeParams(point, angle, color), m_length(len){}

    double m_length;
};

class Line : public Shape
{
public:
    Line();
    Line(LineParams params);
    
    inline const double& GetLength() const;
    inline void SetLenght(const double lenght);
    void DrawInternal() const;

    void Rotate(const double& angle);
    //void Revolve(const Point& pivot, const double& angle);
    void Move(const double move_x, const double move_y); 

private:
    LineParams m_params;
};

const double& Line::GetLength() const
{
    return (m_params.m_length);
}
inline void Line::SetLenght(const double length)
{
    m_params.m_length = length;
}

} //ilrd
#endif //ILRD_RD8586_LINE_HPP