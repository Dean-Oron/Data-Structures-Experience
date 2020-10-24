/******************************************************************************/
/* 						ThreadPool                  						  */
/*						Author - Dean Oron									  */
/*						Date - 10.08.20 								      */
/*						Reviewer - Yehuda       						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_THREAD_POOL_HPP
#define ILRD_RD8586_THREAD_POOL_HPP

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <queue>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "waitable_queue.hpp"
#include "priority_queue.hpp"
#include "logger.hpp"

namespace ilrd
{
class ThreadPoolInterface
{
    public:
    virtual void PushToTerminationQueue() = 0;
    virtual void ExecuteTask() = 0;
};

class ThreadPool: public ThreadPoolInterface, private boost::noncopyable
{
    class ActiveThread;
public:
    enum Priority
    {
        HIGH = 1,
        MIDDLE,
        LOW
    };
    class Task;
    typedef boost::function< void(void) > ActionFunc;
    typedef std::pair<int, boost::shared_ptr<Task> > PriorityAndTask;
    typedef boost::posix_time::milliseconds Millisec;
    
    class Task
    {
    public:
        explicit Task(ActionFunc func):m_exec_func(func){}
        virtual ~Task();
        virtual void operator()();
    private:
        ActionFunc m_exec_func;
    };

    explicit ThreadPool(size_t num_of_threads);
    ~ThreadPool();
    void AddTask(boost::shared_ptr< ThreadPool::Task > task, int priority = 2);
    bool SetThreadsAmount(size_t new_amount); //Set a new amount of threads
    void Start();
    void Stop(); //Exits all threads
    void Pause(); //Pause all threads
    void Resume(); //Resume all threads to work again
private:   
    class ActiveThread
    {
        public:
            explicit ActiveThread(ThreadPoolInterface& interface);
            ~ActiveThread(); //Join the m_thread for task to finish
            void PopAndExecute(); //Popping the task from queue and execute
            void StopAT();
            boost::thread& GetThread();
        private:

            ThreadPoolInterface& m_thread_pool;
            boost::thread m_thread; 
            bool m_is_active;
    };

    struct CompareFunc
    {
        bool operator()(PriorityAndTask task1, PriorityAndTask task2)
        {
            return(task1.first > task2.first);
        }
    };

    typedef boost::shared_ptr<ActiveThread> ActiveThreadPtr;
    std::map<boost::thread::id, ActiveThreadPtr> m_threads; // vector of all Active objects
    WaitableQueue<PriorityQueue<PriorityAndTask, std::vector<PriorityAndTask>, 
    CompareFunc >, PriorityAndTask> m_tasks;
    boost::condition_variable m_cond;
    WaitableQueue< std::queue< ActiveThreadPtr >,  ActiveThreadPtr >m_joining_queue;
    boost::mutex m_pause_mutex;
    size_t m_total_num;
    bool m_paused;

    void PauseThread();    
    void StopThread();
    void ExecuteTask();
    void InitializeThread(size_t amount);
    void PushToTerminationQueue();
};

}//namespace ilrd

#endif // ILRD_RD8586_THREAD_POOL_HPP
