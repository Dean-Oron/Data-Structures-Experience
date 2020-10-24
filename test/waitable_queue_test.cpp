#include <iostream>
#include <boost/thread.hpp>
#include <queue>

#include "waitable_queue.hpp"
#include "singleton.hpp"

using namespace ilrd;
int x = 7;
int front;
int timeout = 3000;

typedef Singleton<WaitableQueue<std::queue<int>, int>> waitable_queue;

void PushThread()
{
    waitable_queue::GetInstance()->Push(x);
    std::cout << "pushed x to queue " << std::endl;
}

void PopThread()
{
    waitable_queue::GetInstance()->Pop(front);
    //std::cout << "in regular pop. pop val is: " << front << std::endl;
}

void TimedPop()
{
    WaitableQueue<std::queue<int>, int>::Milisec ms(5000);
    waitable_queue::GetInstance()->Pop(front, ms);
}

int main()
{
    boost::thread pushThreads[5];
    boost::thread popThreads[5];

    int i = 0;
    
    boost::thread popTimeThread(TimedPop);
    popTimeThread.join();

    for (i = 0; i < 5; ++i)
    {
        pushThreads[i] = boost::thread(PushThread);
    }

    for (i = 0; i < 5; ++i)
    {
        popThreads[i] = boost::thread(PopThread);
    }

    for (i = 0; i < 5; ++i)
    {
        pushThreads[i].join();
        popThreads[i].join();
    } 

    return (0);
}