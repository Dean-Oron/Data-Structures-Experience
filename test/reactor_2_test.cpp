/******************************************************************************/
/* 						Reactor implementation         						  */
/*						Author - Dean Oron									  */
/*						Date - 12.07.20 								      */
/*						Reviewer - Tehila    							      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "reactor.hpp"

using namespace std;

class Listener2: public IListener
{
public:
    Listener2():IListener(){}
    vector<ModeAndHandle> Listen(const vector<ModeAndHandle>& handle);
};

vector<ModeAndHandle> Listener2::Listen(const vector<ModeAndHandle>& handle)
{
    fd_set read_set;
    fd_set write_set;
    fd_set exception_set;
    int max_fd = 0;
    int value;

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exception_set);
    // Putting the handlers into file descriptors lists
    for_each(handle.begin(), handle.end(), SetInsertion(&read_set, &write_set,
                                &exception_set, max_fd));

    
    value = select(FD_SETSIZE, &read_set, &write_set, &exception_set, NULL);

    cout << "select returns " << value << endl;
    ModeAndHandle to_insert;
    vector<ModeAndHandle> ready_pairs;

    for (int i = 0; i <= max_fd; ++i)
    {
        if (FD_ISSET(i, &read_set))
        {
            to_insert.first = READ;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }

        else if (FD_ISSET(i, &write_set))
        {
            to_insert.first = WRITE;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }
        else if (FD_ISSET(i, &exception_set))
        {
            to_insert.first = EXCEPTION;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }
    }

    return (ready_pairs);
}

class STDINFunctor
{
public:
    STDINFunctor(Reactor& reactor_): m_reactor(reactor_)
    {}

    void operator()(int handle)
    {
        char buffer[4];
        string exit = "exit";
        read(handle, buffer, 4);

        if (0 == exit.compare(buffer))
        {
            m_reactor.Stop();
        }

        cout << buffer << endl;
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
        //cout << "check" << endl;
        char buffer[10];
        FILE *file = fopen("./reactor.txt", "r+");
        read(handle, buffer, 10);
        fflush(file);
        cout << buffer << endl;
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
    char buffer[4];
    read(handle, buffer, 4);

    cout << buffer << endl;    
}

int main()
{
    Listener2 listener;
    Reactor my_reactor(&listener);
    Callback<Source<int>> c1(callback_func, Death);

    int fd = open ("./reactor.txt", O_CREAT);

    //check the stdin file descriptor
    my_reactor.Add({READ, STDIN_FILENO}, &c1);
    //my_reactor.Add({READ, fd}, TXTFunctor(my_reactor));

    my_reactor.Run();

    return (0);
}