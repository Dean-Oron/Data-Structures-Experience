#include <iostream>

#include "dir_monitor.hpp"
using namespace ilrd;


void CallbackFunc(string str)
{
    std::cout <<  "from callback test file : " << str << std::endl;
}

int main()
{
    string lib_path("/home/dean/Desktop/Dean/git/cpp/project/lib");
    Callback<Dispatcher<string> >cb(&CallbackFunc);
    DirMonitor moni(lib_path);

    moni.Subscribe(&cb);
    moni.StartMonitoring();
    
    sleep(100);

    return (0);
}