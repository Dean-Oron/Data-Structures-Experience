#include <iostream>

#include "thread_pool.hpp"

using namespace ilrd;
#define THREADS_NUM (10)

void Task1()
{
    std::cout << "I'm task: 1"  << std::endl;
}

void Task2()
{
    std::cout << "I'm task: 2" << std::endl;
}

void Task3()
{
    std::cout << "I'm task: 3" << std::endl;
}

void Task4()
{
    std::cout << "I'm task: 4" << std::endl;
}

int main ()
{
    boost::shared_ptr<ThreadPool::Task> task_ptr1 (new ThreadPool::Task(Task1));
    boost::shared_ptr<ThreadPool::Task> task_ptr2 (new ThreadPool::Task(Task2));
    boost::shared_ptr<ThreadPool::Task> task_ptr3 (new ThreadPool::Task(Task3));
    boost::shared_ptr<ThreadPool::Task> task_ptr4 (new ThreadPool::Task(Task4));

    ThreadPool my_pool(3);

    my_pool.AddTask(task_ptr1);
    my_pool.AddTask(task_ptr2);
    my_pool.AddTask(task_ptr3);
    my_pool.AddTask(task_ptr4);
    
    my_pool.Start();

    my_pool.Pause();
    my_pool.Resume();
    my_pool.Stop();

    my_pool.SetThreadsAmount(7);

    my_pool.AddTask(task_ptr1);
    my_pool.AddTask(task_ptr2);
    my_pool.AddTask(task_ptr3);
    my_pool.AddTask(task_ptr4);

    return (0);
}