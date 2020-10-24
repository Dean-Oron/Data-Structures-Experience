#include <iostream>
#include <boost/thread.hpp>
#include <queue>
#include<time.h>

#include "waitable_queue.hpp"
#include "singleton.hpp"
#include "priority_queue.hpp"

using namespace ilrd;

typedef Singleton<WaitableQueue<PriorityQueue<int>, int>> waitable_priority_queue;

WaitableQueue<std::queue<int>, int>::Milisec ms(5000);

class PushFunctor
{
public:
    PushFunctor():m_data(0){}
    void operator()()
    {
        //srand(time(NULL));
        m_data = rand() % 100;
        
        waitable_priority_queue::GetInstance()->Push(m_data);
        std::cout << "pushed " << m_data << " to queue " << std::endl;        
    }

private:
    int m_data;
};

class PopFunctor
{
public:
    PopFunctor():m_data(0){}
    void operator()()
    {
        waitable_priority_queue::GetInstance()->Pop(m_data);
    }

private:
    int m_data;
};

class PopTimedFunctor
{
public:
    PopTimedFunctor(WaitableQueue<PriorityQueue<int>, int>::Milisec timeout):m_time(timeout){}
    void operator()()
    {
        waitable_priority_queue::GetInstance()->Pop(m_data, m_time);
    }

private:
    int m_data;
    WaitableQueue<PriorityQueue<int>, int>::Milisec m_time;
};

int main()
{
    boost::thread pushThreads[5];
    boost::thread popThreads[5];

    int i = 0;
    
    boost::thread popTimeThread;

    popTimeThread = boost::thread(PopTimedFunctor(ms));

    popTimeThread.join();

    for (i = 0; i < 4; ++i)
    {
        pushThreads[i] = boost::thread(PushFunctor());
    }
 
/*     for (i = 0; i < 5; ++i)
    {
        popThreads[i] = boost::thread(PopFunctor());
    } */

    for (i = 0; i < 5; ++i)
    {
        pushThreads[i].join();
        popThreads[i].join();
    }  

    int front = 0;
    for (i = 0; i < 4; ++i)
    {
        waitable_priority_queue::GetInstance()->Pop(front);
        std::cout << front << std::endl;
    }
    
    return (0);
}