/******************************************************************************/
/* 						Scheduler implementation         					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <boost/bind.hpp>   // bind
#include <boost/chrono.hpp> // chrono things
#include <boost/chrono/system_clocks.hpp>

#include "scheduler.hpp"

using namespace ilrd;
using namespace std;

Scheduler::Scheduler(Reactor& reactor)
try : m_timer(reactor, boost::bind(&Scheduler::InvokeCurrentTask, this, _1))
{
}
catch (...)
{
    throw std::runtime_error("scheduler create failed");
}

void Scheduler::ScheduleAction(MicroSecs microseconds, ActionFunc function)
{
    Task new_task;
    new_task.m_timepoint = Now() + microseconds;
    new_task.m_function = function;

    if (IsSetTimerNeeded(new_task))
    {
        SetTimer(microseconds);
    }

    m_tasks.push(new_task);
}

void Scheduler::ScheduleAction(TimePoint timepoint, ActionFunc function)
{
    Task new_task;
    new_task.m_timepoint = timepoint;
    new_task.m_function = function;

    if (IsSetTimerNeeded(new_task))
    {
        SetTimer(GetDuration(timepoint));
    }

    m_tasks.push(new_task);
}

void Scheduler::InvokeCurrentTask(int fd)
{
    const Task current = m_tasks.top();
    m_tasks.pop();

    //Invocation callback
    current.m_function(fd);

    if (!m_tasks.empty())
    {
        SetTimer(GetDurationForNextTask());
    }
    else
    {
        m_timer.Unset();
    }
}

bool Scheduler::IsSetTimerNeeded(Task& new_task) const
{
    return (m_tasks.empty() || m_tasks.top() < new_task);
}

void Scheduler::SetTimer(MicroSecs time)
{
    try
    {
        m_timer.Set(time);
    }
    catch (...)
    {
        throw std::runtime_error("can't add action to scheduler");
    }
}