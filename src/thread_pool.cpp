#include <iostream>
#include <utility>
#include "thread_pool.hpp"

#define TIME (4000)
namespace ilrd
{
/**********************************TASK****************************************/
    ThreadPool::Task::~Task()
    {
    }

    void ThreadPool::Task::operator()()
    {
        m_exec_func();
    }

/*******************************ActiveThread***********************************/
    void DefaultFunc(){}

    ThreadPool::ActiveThread::ActiveThread(ThreadPoolInterface& interface): 
            m_thread_pool(interface), m_is_active(true)
    {       
    }

    void ThreadPool::ActiveThread::PopAndExecute()
    {
        while (m_is_active)
        {
            m_thread_pool.ExecuteTask();
        }

        m_thread_pool.PushToTerminationQueue();
    }

    void ThreadPool::ActiveThread::StopAT()
    { 
        m_is_active = false;
    }

    boost::thread& ThreadPool::ActiveThread::GetThread()
    {
        return m_thread;
    }
    
    ThreadPool::ActiveThread::~ActiveThread()
    { 
        m_thread.join();       
    }

/*******************************ThreadPool*************************************/
    ThreadPool::ThreadPool(size_t num_of_threads): m_total_num(num_of_threads)
    {
    }

    ThreadPool::~ThreadPool()
    {
        LOG_INFO("In threadPool destructor");
        Stop();        
    }

    void ThreadPool::StopThread()
    {
        m_threads.find(boost::this_thread::get_id())->second->StopAT();    
    }

    void ThreadPool::PauseThread()
    {
        boost::unique_lock<boost::mutex> lock(m_pause_mutex);
        while (m_paused)
        {
            m_cond.wait(lock);
        }           
    }

    void ThreadPool::ExecuteTask()
    {
        boost::shared_ptr<ThreadPool::Task> task(new ThreadPool::Task(DefaultFunc));
        PriorityAndTask prioritized_task (MIDDLE, task);
        m_tasks.Pop(prioritized_task, (Millisec)TIME);
        prioritized_task.second->operator()();
    }

    void  ThreadPool::PushToTerminationQueue()
    {
        m_joining_queue.Push(m_threads.find(boost::this_thread::get_id())->second);
    }


    bool ThreadPool::SetThreadsAmount(size_t new_amount)
    {
        size_t i = 0;
        if (new_amount > m_total_num)
        {
            InitializeThread(new_amount - m_total_num);
        }
        else
        {
            for (i = 0;  i < (m_total_num - new_amount); ++i)
            {
                boost::shared_ptr< ThreadPool::Task > task( new ThreadPool::Task 
                (boost::bind(&ThreadPool::StopThread, this)));
                AddTask(task, ThreadPool::LOW+1);  
            }

            for (i = 0;  i < (m_total_num - new_amount); ++i)
            {
                ActiveThreadPtr pointer;
                m_joining_queue.Pop(pointer);                  
            }
        }

        m_total_num = new_amount;
        return 0;
    }
    
    
    void  ThreadPool::AddTask(boost::shared_ptr< ThreadPool::Task > task,
                int priority)
    {
        m_tasks.Push(PriorityAndTask(priority, task));
    } 

    void ThreadPool::Start()
    {
         InitializeThread(m_total_num);
    }

    void ThreadPool::InitializeThread(size_t amount)
    {
        size_t i = 0;
        for (i = 0;  i < amount; ++i)
        {         
            try
            {
                boost::shared_ptr<ThreadPool::ActiveThread> thread
                    (new ThreadPool::ActiveThread(*this));
                thread->GetThread() = boost::thread
                (boost::bind(&ThreadPool::ActiveThread::PopAndExecute,  
                thread.get()));              
                m_threads.insert(std::pair<boost::thread::id,
                    ActiveThreadPtr>(thread->GetThread().get_id(), thread));
            }
            catch(...)
            {
                throw std::runtime_error("thread creation failed\n");
            }  
        }
    }

    void ThreadPool::Stop()
    {
        SetThreadsAmount(0);
        LOG_INFO("ThreadPool was stopped");
    }

    void ThreadPool::Resume()
    {
        m_paused = false;        
        m_cond.notify_all();
        LOG_INFO("ThreadPool was resumued");
    }

    void ThreadPool::Pause()
    {
        size_t i = 0;
        m_paused = true;
        for (i = 0; i < m_total_num; ++i)
        {
                boost::shared_ptr< ThreadPool::Task > task( new ThreadPool::Task 
                                            (boost::bind(&ThreadPool::PauseThread, this)));
                AddTask(task, HIGH -1);                 
        }   
        LOG_INFO("ThreadPool was paused");
    }
}