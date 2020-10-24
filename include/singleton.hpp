#ifndef ILRD_8586_SINGELTON_HPP
#define ILRD_8586_SINGELTON_HPP

#include <iostream>
#include <boost/atomic.hpp>
#include <atomic>
#include <pthread.h> 

#include "boost/core/noncopyable.hpp"

namespace ilrd
{

template <typename T>
class Singleton: private boost::noncopyable
{
public:

    static T* GetInstance();
    static void CleanUp();

private:
    Singleton();
    static T* m_instance;
    static int m_in_proccess;
};

template <typename T> 
T* Singleton<T>::m_instance = NULL;

template < typename T >
int Singleton< T >::m_in_proccess = false;

template <typename T>
T* Singleton<T>::GetInstance()
{
    if (!m_instance)
    {
        if (__atomic_fetch_or(&m_in_proccess, true,
                              __ATOMIC_SEQ_CST) == false)
        {
            __atomic_store_n(&m_instance, new T, __ATOMIC_SEQ_CST);
            m_in_proccess = false;
        }
        else
        {
            while (m_in_proccess == true)
                ;
        }
    }

    return (m_instance);
}

template <typename T> 
void Singleton<T>::CleanUp()
{
    delete m_instance;
    m_instance = NULL;
    m_in_proccess = false;
}

} // namespace ilrd

#endif // ILRD_8586_SINGELTON_HPP