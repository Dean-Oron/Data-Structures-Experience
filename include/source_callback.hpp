/******************************************************************************/
/* 						source_callback implementation         				  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_SOURCE_CALLBACK_HPP__
#define __ILRD_RD8586_SOURCE_CALLBACK_HPP__
#include <iostream>
#include <boost/function.hpp> /* function */
#include <boost/noncopyable.hpp> //non-copyable classes

#include "logger.hpp"

#define LOG_ERR(X) (std::cerr << "ERROR: " << (X) << std::endl)
#define LOG_WRN(X) (std::cerr << "WARNING:" << (X) << std::endl)

using namespace std;

// A defult function to bind with callback death function
static void EmptyFunc()
{

}

template < typename SOURCE >
class Callback;

template < typename T >
class Source : private boost::noncopyable
{
public:
    Source():m_callback(NULL)
    {
    }
    ~Source();
    typedef T dataType; // nested type

    void Subscribe(Callback<Source <T> >* callback);
    void Unsubscribe(Callback<Source <T> >* callback);
    void Notify(dataType data);

private:
    Callback<Source <T> >* m_callback;
};

template < typename SOURCE >
class Callback : private boost::noncopyable
{
public:
    typedef boost::function< void(typename SOURCE::dataType) > CallbackPointer;
    typedef boost::function< void() > DeathPointer;

    Callback(const CallbackPointer& callback_func, const DeathPointer& death_func = &EmptyFunc); // initialize an empty function for death_func

    ~Callback();

private:
    void Link(SOURCE* source);
    void Unlink();
    void Invoke(typename SOURCE::dataType data);
    CallbackPointer m_callback_func;
    DeathPointer m_death_func; 

    SOURCE* m_source;
    
    //friend SOURCE;
    struct FriendHelper{typedef SOURCE MySource;};
    friend class FriendHelper::MySource;
};

//Source Implementation
template < typename T >
void Source<T>::Subscribe(Callback<Source <T> >* callback)
{
    assert(!m_callback);

    if (!callback)
    {
        LOG_ERROR("Did not recieved callback to subscribe");
    }

    m_callback = callback;
    m_callback->Link(this);
}

template < typename T >
Source<T>::~Source()
{   
    if(m_callback)
    {
        m_callback->Unlink();
    }
}

template < typename T >
void Source<T>::Unsubscribe(Callback<Source <T> >* callback)
{
    assert (m_callback == callback);
    if (!callback)
    {
        LOG_ERROR("Did not recieved callback to unsubscribe");
    }
    
    m_callback->Unlink();
    m_callback = NULL;
}

template < typename T >
inline void Source<T>::Notify(dataType handle)
{
    if (m_callback)
    {
        m_callback->Invoke(handle);
    }
    else
    {
        LOG_WRN(handle);
    }
}

//Callback imlementation

template < typename SOURCE >
Callback<SOURCE>::Callback(const CallbackPointer& callback_func,const DeathPointer& death_func)
    : m_callback_func(callback_func), m_death_func(death_func), m_source(NULL)
{
    if (!callback_func)
    {
        LOG_ERR("callback function was not supplied");
    }
    if (!death_func)
    {
        LOG_WRN("death_func was not supplied");
    }
}

template < typename SOURCE >
Callback<SOURCE>::~Callback()
{
    if (m_source)
    {
        m_source->Unsubscribe(this);
    }
}

template < typename SOURCE >
inline void Callback<SOURCE>::Link(SOURCE *source)
{
    assert(!m_source);
    assert(source);

    if (!source)
    {
        LOG_ERR("Source was not sent properly");
    }

    m_source = source;
}

template < typename SOURCE >
inline void Callback<SOURCE>::Invoke(typename SOURCE::dataType handle)
{
    assert(m_source);

    m_callback_func(handle);
}

template < typename SOURCE >
inline void Callback<SOURCE>::Unlink()
{
    assert(m_source);

    m_source = NULL;
    m_death_func();
}

#endif // __ILRD_RD8586_SOURCE_CALLBACK_HPP__
