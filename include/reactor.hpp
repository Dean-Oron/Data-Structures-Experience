/******************************************************************************/
/* 						Reactor implementation           					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_ADVANCED_REACTOR_HPP__
#define __ILRD_RD8586_ADVANCED_REACTOR_HPP__

#include <iostream>
#include <vector>
#include <queue>
#include <map> 
#include <boost/function.hpp>
#include <utility>
#include <sys/select.h>
#include <cassert>

#include "source_callback.hpp"
#include "logger.hpp"

using namespace std;

// The type of a handle is system specific.
// We're using UNIX so an handle is represented by an integer

namespace ilrd
{
typedef int Handle;

enum MODE
{
    READ,
    WRITE,
    EXCEPTION
};

typedef boost::function<void(Handle)> HandleFunc;
typedef std::pair<MODE, Handle> ModeAndHandle;

// Keeping the max fd updated
void FDMax(int& fd, int& max_fd);

// Functor to insert all the fds to the proper set list
class SetInsertion
{
public:
    SetInsertion(fd_set *read_set, fd_set *write_set, fd_set *exception_set,
                                                                int& max_fd):
    m_read_set(read_set), m_write_set(write_set),
    m_exception_set(exception_set), m_max_fd(max_fd)
    {}

    void operator()(ModeAndHandle handle_mode)
    {
        if (handle_mode.first == WRITE)
        {
            FDMax(handle_mode.second, m_max_fd);
            FD_SET(handle_mode.second, m_write_set);
        }
        else if (handle_mode.first == READ)
        {
            FDMax(handle_mode.second, m_max_fd);
            FD_SET(handle_mode.second, m_read_set);
        }
        else if (handle_mode.first == EXCEPTION)
        {
            FDMax(handle_mode.second, m_max_fd);
            FD_SET(handle_mode.second, m_exception_set);
        }
    }

private:
    fd_set *m_read_set;
    fd_set *m_write_set;
    fd_set *m_exception_set;
    int& m_max_fd;
};


//The user may derieve from this class to define his own Listener classes
class IListener
{
public:
    virtual ~IListener(){}
    virtual vector<ModeAndHandle> Listen(const vector<ModeAndHandle>& handle) = 0;
};

// Registration interface of the Reactor
class Reactor
{
public:
    Reactor(IListener *listener): m_stop(0), m_Listener(listener){}
    void Add(ModeAndHandle mode_and_handle, Callback<Source<int> > *callback);
    void Remove(ModeAndHandle mode_and_handle);
    void Run();
    void Stop();    // Can be called only from the handler/run
                    // if there is nothing to listen to (if the map is empty)
    ~Reactor();

private:
    Reactor(const Reactor& other);
    Reactor& operator=(const Reactor& other);
    bool m_stop;
    std::map<ModeAndHandle, Source<int> *> m_EventHandlers;
    IListener *m_Listener;
};

//Inserting new callback and link it to a new simple source
inline void Reactor::Add(ModeAndHandle mode_and_handle, Callback<Source<int> > *callback)
{
    Source<int> *simple_src = new Source<int>;
    simple_src->Subscribe(callback);
    m_EventHandlers.insert({mode_and_handle, simple_src});
}
}// namespace ilrd
#endif//ILRD_ADVANCED_REACTOR