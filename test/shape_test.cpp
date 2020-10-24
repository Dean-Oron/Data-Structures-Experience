/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20 								      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "shape_lib.hpp"
#include "factory.hpp"

using namespace ilrd;
using namespace std;

int drawCircle = 1;
int xCircle = 250;
int yCircle = 100;
double rCircle = 100;
const int CIRCLE_AMOUNT = 10; 

static void DrawFunction();
static int KeyboardFunction(unsigned char key, int x, int y);
static int MouseFunction(int button, int state, int x, int y);
static int MotionFunction(int x, int y);
static int TimerFunction();
static int MoveDown(Shape *shape);
static int MoveRevolve(Shape *shape);
static void InitCirclesArr();
void SuccessTouch();
void DeleteCircle(Circle& circle);

typedef Singleton<Factory< string, Shape, ShapeParams >> shapeFact;
Point p1(200, 200);
Point p_base(500, 900);
Point c1_point(500, 0);
Color c1(255, 255, 255);
Color c2(1, 1, 1);
SquareParams square_params(p1, 180, c1, 300);
Shape *s1 = new Square(square_params);
CircleParams c1_params(c1_point, 180, c2, 100.0);
Shape *cir1 = new Circle(c1_params);
RectangleParams rect_params(p_base, 180, c2, 100, 250);
Shape *base = new Rectangle(rect_params);
Shape *cir_arr[CIRCLE_AMOUNT];

int main(int argc, char** argv)
{
    //shapeFact::GetInstance()->Add("circle", )
    InitCirclesArr();
    DrawInit(argc, argv, 2000, 1000, DrawFunction);
    DrawSetKeyboardFunc(KeyboardFunction);
    DrawSetMouseFunc(MouseFunction);
    DrawSetMotionFunc(MotionFunction);
    DrawSetTimerFunc(TimerFunction, 100);

    DrawMainLoop();

    cout << "Its fucking working!!!" << endl;

    return (0);
}

static void DrawFunction()
{
    int i;
    //s1->Draw();
    //cir1->Draw();
    base->Draw();

    for (i = 0; i < CIRCLE_AMOUNT; ++i)
    {
        cir_arr[i]->Draw();
    }
}

static int KeyboardFunction(unsigned char key, int x, int y)
{
    printf("Keyboard: %02x,%d,%d\n", key, x, y);

    if (key == 0x1b /* ESC */)
    {
        return (-1);
    }
    if (key == 0x6b) // k
    {
        if (base->GetPosition().GetX() < 1950)
        base->Move(20,0);
    }
    if (key == 0x64) // d
    {
        if (base->GetPosition().GetX() > 50)
        base->Move(-20,0);  
    }

    return 0;
}

static int MouseFunction(int button, int state, int x, int y)
{
    /* printf("Mouse: %d,%d,%d,%d\n", button, state, x, y); */
    (void)x;
    (void)y;
    if (state == 1 && button == MOUSE_WHEEL_UP)
    {
        rCircle *= 1.1;
        return 1;
    }
    if (state == 1 && button == MOUSE_WHEEL_DOWN)
    {
        rCircle *= 0.9;
        return 1;
    }

    if (button == MOUSE_LEFT)
    {
        drawCircle = state;
        return 1;
    }

    return (0);
}


static int MotionFunction(int x, int y)
{
    printf("Mouse: %d,%d\n", x, y);

    return 0;
}

static int TimerFunction()
{
    int i;

    i = rand() % CIRCLE_AMOUNT;
    MoveDown(cir_arr[i]);
    MoveRevolve(s1);
    SuccessTouch();

    return (1);  /* draw */
}

static int MoveDown(Shape *shape)
{
    int  upper = 100;
    int lower = 10;
    int i = rand() % (upper - lower + 1) + lower; 
    shape->Move(0, i);

    return (0);
}

static int MoveRevolve(Shape *shape)
{
    int angle = 20;
    Point p1(10, 10);
    Point curr_point = shape->GetPosition();
    shape->SetPosition(curr_point.Revolve(curr_point + p1, angle));

    return(0);
} 

static void InitCirclesArr()
{
    int i = 0;
    double radius = 100;
    int upper = 70;
    int lower = 10;
    Point position (1, -100);
    Point move_by(170, 0);
    CircleParams params(position, 0, c2, radius);
    for (i = 0; i < CIRCLE_AMOUNT; ++i)
    {   
        params.m_radius = rand() % (upper - lower + 1) + lower; 
        params.m_point += move_by;

        cir_arr[i] = new Circle(params);
    }
}

void DeleteCircle(Circle& circle)
{
    Point out_of_range(0, 1500);
    circle.SetPosition(out_of_range);
}

void SuccessTouch()
{
    int i;
    int safe_factor = 1;
    double c_radius;
    int c_x;
    int c_y;
    int b_x = base->GetPosition().GetX();
    int b_y = base->GetPosition().GetY();
    double b_width = static_cast<Rectangle *>(base)->GetWidth();
    double b_height = static_cast<Rectangle *>(base)->GetHeight();
    for (i = 0; i < CIRCLE_AMOUNT; ++i)
    {
        c_x = cir_arr[i]->GetPosition().GetX();
        c_y = cir_arr[i]->GetPosition().GetY();
        c_radius = static_cast<Circle *>(cir_arr[i])->GetRadius();
        
        if ((c_x > b_x - (b_width / 2) - c_radius - safe_factor) &&
            (c_x < b_x + (b_width / 2) + c_radius + safe_factor) &&
            (c_y > b_y - (b_height / 2) - c_radius - safe_factor) &&
            (c_y < b_y + (b_height / 2) + c_radius + safe_factor))
            {
                DeleteCircle(*(static_cast<Circle *>(cir_arr[i])));
            }
    }
}