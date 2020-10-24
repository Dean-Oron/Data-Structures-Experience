#ifndef __ILRD_RD8586_WAITABLE_QUEUE_HPP__
#define __ILRD_RD8586_WAITABLE_QUEUE_HPP__

#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/noncopyable.hpp>

namespace ilrd
{
template <typename QUEUE, typename T >
class WaitableQueue : private boost::noncopyable
{
public:
    // default Ctor and Dtor
    typedef boost::posix_time::milliseconds Milisec;
    void Push (const T& val);
    void Pop(T& peaked_value); // will use front and pop
    bool Pop(T& peaked_value, Milisec timeout);
    bool Empty() const; //please make sur to use in a thread-safe environment
    const T& front() const;

private:
    QUEUE m_queue;
    boost::condition_variable m_pushflag; //use boost::unique_lock
    boost::mutex m_mutex;//for pop
};

template <typename QUEUE, typename T>
void WaitableQueue<QUEUE, T>::Push(const T& value)
{
    //boost::unique_lock< boost::mutex > lk(m_mutex_pop);

    //std::cout << "within Push before lock" << std::endl;
    m_mutex.lock();
    m_queue.push(value);
    m_mutex.unlock();
    m_pushflag.notify_one();
}

template <typename QUEUE, typename T>
void WaitableQueue<QUEUE, T>::Pop(T& peaked_value)
{
    boost::unique_lock< boost::mutex > lk(m_mutex);
    
    if (Empty()) 
    {   
        m_pushflag.wait(lk);
    }
    
    peaked_value = m_queue.front();
    m_queue.pop();
}

template <typename QUEUE, typename T>
bool WaitableQueue<QUEUE, T>::Pop(T& peaked_value, Milisec timeout)
{
    boost::unique_lock< boost::mutex > lk(m_mutex);
    bool to_return = 1;

    if (Empty()) 
    {
        to_return = m_pushflag.timed_wait(lk, timeout);
    }

    if (to_return)
    {
        peaked_value = m_queue.front();
        m_queue.pop();

        return(to_return);
    }

    std::cout << "queue was empty for more then " << timeout.seconds() << "seconds" <<
    std::endl << " and timeout Pop returned " << to_return << std::endl;

    return (to_return);
}

template <typename QUEUE, typename T>
bool WaitableQueue<QUEUE, T>::Empty() const
{
    return (m_queue.empty());
}

template <typename QUEUE, typename T>
const T& WaitableQueue<QUEUE, T>::front() const
{
    return (m_queue.front());
}

}//ilrd
#endif// __ILRD_RD8586_WAITABLE_QUEUE_HPP__