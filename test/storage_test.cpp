#include <iostream>

#include "storage.hpp"

using namespace ilrd;
using namespace std;

int main()
{
    Storage<13> store1(8);

    char write[] = "dean is tired";
    store1.Write(0, write);

    char read[20];
    store1.Read(0, read);

    cout << read << endl;

    return (0);
}