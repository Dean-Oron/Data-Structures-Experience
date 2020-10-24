/******************************************************************************/
/* 						Scheduler implementation         					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "reactor.hpp"
#include "scheduler.hpp"
#include "listener.hpp"

using namespace std;
using namespace ilrd;

class STDINFunctor
{
public:
    STDINFunctor(Reactor& reactor_): m_reactor(reactor_)
    {}

    void operator()(int handle)
    {
        char buffer[4];
        string exit = "exit";
        ssize_t n_bytes = read(handle, buffer, 4);

        if (0 == exit.compare(buffer))
        {
            m_reactor.Stop();
        }

        cout << buffer << "was read: "  << n_bytes << " bytes" << endl;
    }
private:
    Reactor& m_reactor;
};

class TXTFunctor
{
public:
    TXTFunctor(Reactor& reactor_): m_reactor(reactor_)
    {}

    void operator()(int handle)
    {
        string exit = "exit";
        char buffer[4];
        FILE *file = fopen("./reactor.txt", "r+");
        ssize_t n_bytes;
        n_bytes = read(handle, buffer, 4);

        if (0 == exit.compare(buffer))
        {
            m_reactor.Stop();
        }
        fflush(file);

        cout << buffer << "was read: "  << n_bytes << " bytes" << endl;
    }
private:
    Reactor& m_reactor;
};

void Death()
{
    cout << "died" << endl;
}

void callback_func(int handle)
{
    size_t n_bytes;
    char buffer[4];
    n_bytes = read(handle, buffer, 4);

    cout << buffer << n_bytes << endl;    
}

time_t now;

void Test1 (int x)
{   
    time(&now);
    cout << "the time now is " << ctime(&now) << endl;
    cout << "im test1" << endl;
    
    (void)x;
}
void Test2 (int x)
{

    time(&now);
    cout << "the time now is " << ctime(&now) << endl;    cout << "im test2" << endl;
    (void)x;
}

void Test3 (int x)
{
    time(&now);
    cout << "the time now is " << ctime(&now) << endl;
    cout << "im test3" << endl;
    cout << "write 'exit' to abort" << endl;
    (void)x;
}

int main()
{
    cout << "entered program" << endl;
    Listener2 listener;
    Reactor my_reactor(&listener);
    Callback<Source<int> > c1(STDINFunctor(my_reactor), Death);
    Callback<Source<int> > c2(TXTFunctor(my_reactor), Death);
    FDTimer::MicroSecs first_time(5000);
    FDTimer::MicroSecs second_time(10000);
    TimePoint absulote_time(Now() + first_time + second_time);

    my_reactor.Add({READ, STDIN_FILENO}, &c1);

    Scheduler s1(my_reactor);

    s1.ScheduleAction(first_time, Test1);
    s1.ScheduleAction(second_time, Test2);
    s1.ScheduleAction(absulote_time, Test3);
    
    my_reactor.Run();

    cout << "check" << endl;
    
    return (0);
}