/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_ANIMAL
#define ILRD_RD8586_ANIMAL
#include <iostream>
#include <string>

using namespace std;

namespace ilrd
{
class AnimalParams
{
public:
    AnimalParams(string name = "name", int legs = 2, int eyes = 2, bool fly = 0, string sound = "non")
    : m_name(name), m_legs(legs), m_eyes(eyes), m_can_fly(fly), m_sound(sound) 
    {

    }
    
    string m_name;
    int m_legs;
    int m_eyes;
    bool m_can_fly;
    string m_sound;
};

class IAnimal
{
public:
    IAnimal(AnimalParams params): m_params(params){}

    virtual ~IAnimal() = 0;

    string GetName()
    {
        return(m_params.m_name);
    }
    
    string GetSound()
    {
        return (m_params.m_sound);
    }
    int GetLegs()
    {
        return (m_params.m_legs);
    }

    virtual bool CanFly() = 0; 

private:
    AnimalParams m_params;
};

IAnimal::~IAnimal(){}

class Dog: public IAnimal
{
    public:
    Dog(AnimalParams params):IAnimal(params)
    {
    }

    bool CanFly()
    {
        return (0);
    }
};

class Cat: public IAnimal
{
    public:
    Cat(AnimalParams params):IAnimal(params)
    {

    }

    bool CanFly()
    {
        return (0);
    }
};

class Fly: public IAnimal
{
    public:
    Fly(AnimalParams params):IAnimal(params)
    {
        
    }

    bool CanFly()
    {
        return (1);
    }
};

class Octopus: public IAnimal
{
    public:
    Octopus(AnimalParams params):IAnimal(params)
    {

    }

    bool CanFly()
    {
        return (0);
    }
};

}// ilrd namespace

#endif// ILRD_RD8586_ANIMAL