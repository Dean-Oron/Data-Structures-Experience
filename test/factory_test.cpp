#include <iostream>

#include "factory.hpp"
#include "animal.hpp"
#include "singleton.hpp"
using namespace ilrd;
using namespace std;

IAnimal *DogCreation(AnimalParams dog_params)
{
    IAnimal *dog = new Dog(dog_params); 

    return (dog);
}
 
IAnimal *CatCreation(AnimalParams cat_params)
{
    IAnimal *cat = new Cat(cat_params);

    return (cat);
}

IAnimal *OctopusCreation(AnimalParams octopus_params)
{
    IAnimal *octopus = new Octopus(octopus_params);

    return (octopus);
} 

IAnimal *FlyCreation(AnimalParams fly_params)
{
    IAnimal *octopus = new Fly(fly_params);

    return (octopus);
} 

int main ()
{
    AnimalParams dog1_params("Roki", 4, 2, 0, "waff");
    AnimalParams dog2_params("Simmba", 4, 2, 0, "waff");
    AnimalParams cat1_params("Mizi", 4, 2, 0, "miuoo");
    AnimalParams cat2_params("Mave", 4, 2, 0, "miuoo");

    AnimalParams fly1_params("non", 10, 8, 1, "zzzz");
    AnimalParams octopus_params("octy", 8, 2, 0);
    
    Factory<string, IAnimal, AnimalParams> *animal_factory = 
                    Singleton <Factory <string, IAnimal, AnimalParams>>:: GetInstance();

    animal_factory->Add("dog", DogCreation);
    animal_factory->Add("cat", CatCreation);
    animal_factory->Add("fly", FlyCreation);
    //animal_factory->Add("octopus", OctopusCreation);
    
    IAnimal *dog1 = animal_factory->Create("dog", dog1_params);
    IAnimal *dog2 = animal_factory->Create("dog", dog2_params);
    IAnimal *cat1 = animal_factory->Create("cat", cat1_params);
    IAnimal *cat2 = animal_factory->Create("cat", cat2_params);

    try
    {
        animal_factory->Create("octopus", dog1_params);
    }
    
    catch(char const *err)
    {
        cout << err << endl;
    }

    cout << dog2->GetName() << endl;
    
    delete dog1;
    delete dog2;
    delete cat1;
    delete cat2;

    return (0);
}
