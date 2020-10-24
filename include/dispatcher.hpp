/******************************************************************************/
/* 						Dispatcher implementation           				  */
/*						Author - Dean Oron									  */
/*						Date -           								      */
/******************************************************************************/

#ifndef __ILRD_RD8586_DISPATCHER_HPP__
#define __ILRD_RD8586_DISPATCHER_HPP__

#include <iostream>
#include <boost/function.hpp> /* function */
#include <boost/noncopyable.hpp> //non-copyable classes

#include "logger.hpp"

using namespace std;

// A defult function to bind with callback death function
static void EmptyFunc()
{
    
}

template < typename DISPATCHER >
class Callback;

template < typename T >
class Dispatcher : private boost::noncopyable
{
public:
    Dispatcher()
    {
    }
    ~Dispatcher();
    typedef T dataType; // nested type

    void Subscribe(Callback<Dispatcher <T> >* callback);
    void Unsubscribe(Callback<Dispatcher <T> >* callback);
    void Notify(dataType data);

private:
    std::list<Callback<Dispatcher <T> > *> m_callbacks;
};

template < typename DISPATCHER >
class Callback : private boost::noncopyable
{
public:
    typedef boost::function< void(typename DISPATCHER::dataType) > CallbackPointer;
    typedef boost::function< void() > DeathPointer;

    Callback(const CallbackPointer& callback_func, const DeathPointer& death_func = &EmptyFunc); 

    ~Callback();

private:
    void Link(DISPATCHER* dispatch);
    void Unlink();
    void Invoke(typename DISPATCHER::dataType data);
    CallbackPointer m_callback_func;
    DeathPointer m_death_func; 

    DISPATCHER* m_dispatcher;
    
    struct FriendHelper{typedef DISPATCHER MyDispatcher;};
    friend class FriendHelper::MyDispatcher;
};

template < typename T >
class InvokeFunctor
{
public:
    InvokeFunctor(T data):m_data(data){}
    void operator()(Callback< T > *callback)
    {
        callback->Invoke(m_data);
    }

private:
    T m_data;
};

template < typename T >
class UnlinkFunctor
{
public:
    void operator()(Callback< T > *callback)
    {
        callback->Unlink();
    }
};

template < typename SOURCE >
Callback<SOURCE>::Callback(const CallbackPointer& callback_func,const DeathPointer& death_func)
    : m_callback_func(callback_func), m_death_func(death_func), m_dispatcher(NULL)
{
    if (!callback_func)
    {
        LOG_ERROR("callback function was not supplied");
    }
    if (!death_func)
    {
        LOG_WARNING("death_func was not supplied");
    }
}

template < typename DISPATCHER >
Callback<DISPATCHER>::~Callback()
{
    if (m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
    }
}

template < typename T >
void Dispatcher<T>::Subscribe(Callback<Dispatcher <T> >* callback)
{
    assert(callback);

    if (!callback)
    {
        LOG_ERROR("Did not recieved callback to subscribe");
    }

    m_callbacks.push_back(callback);
    
    callback->Link(this);
}

template < typename DISPATCHER >
inline void Callback<DISPATCHER>::Link(DISPATCHER *dispatcher)
{
    assert(!m_dispatcher);
    assert(dispatcher);

    if (!dispatcher)
    {
        LOG_ERROR("Dispatcher was not sent properly");
    }

    m_dispatcher = dispatcher;
}

template < typename T >
void Dispatcher<T>::Unsubscribe(Callback<Dispatcher <T> >* callback)
{
    if (!callback)
    {
        LOG_WARNING("Did not recieved callback to unsubscribe");
    }
    
    callback->Unlink();
    m_callbacks.remove(callback);
}

template < typename DISPATCHER >
inline void Callback<DISPATCHER>::Unlink()
{
    assert(m_dispatcher);

    m_dispatcher = NULL;
    m_death_func();
}

template < typename T >
inline void Dispatcher<T>::Notify(dataType handle)
{
    if (!m_callbacks.empty())
    {
        typename std::list<Callback<Dispatcher<T> > *>::iterator it = m_callbacks.begin();
        for (; it != m_callbacks.end(); ++it)
        {
            (*it)->Invoke(handle);
        }
    }
    else
    {
        LOG_WARNING("No subscriber is registered to dispatch");
    }
}

template < typename DISPATCHER >
inline void Callback<DISPATCHER>::Invoke(typename DISPATCHER::dataType handle)
{
    assert(m_dispatcher);

    m_callback_func(handle);
}

template < typename T >
Dispatcher<T>::~Dispatcher()
{   
    if(!m_callbacks.empty())
    {
        typename std::list<Callback<Dispatcher<T> > *>::iterator it = m_callbacks.begin();

        for (; it != m_callbacks.end(); ++it)
        {
            (*it)->Unlink();
        }

    }
}

#endif // __ILRD_RD8586_DISPATCHER_HPP__