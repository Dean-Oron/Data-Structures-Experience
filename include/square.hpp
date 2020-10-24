/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_SQUARE_HPP
#define ILRD_RD8586_SQUARE_HPP

#include "point.hpp"
#include "shape.hpp"

namespace ilrd
{

class SquareParams: public ShapeParams
{
public:
    SquareParams(Point point, double angle, Color color, double edge):ShapeParams(point, angle, color), m_edge(edge){}

    double m_edge;
};

class Square: public Shape
{
public:
    Square();
    Square(SquareParams params);
    
    inline const double& GetEdge() const;
    inline void SetEdge(const double& edge);

    void DrawInternal();

private:
    SquareParams m_params;
};

const double& Square::GetEdge() const
{
    return (m_params.m_edge);
}

void Square::SetEdge(const double& edge) 
{
    m_params.m_edge = edge;
}

} //ilrd
#endif //ILRD_RD8586_LINE_HPP