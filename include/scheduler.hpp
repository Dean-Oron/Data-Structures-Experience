/******************************************************************************/
/* 						Scheduler Headr File             					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_SCHEDULER_HPP
#define ILRD_RD8586_SCHEDULER_HPP

#include <boost/noncopyable.hpp>
#include <queue>

#include "fd_timer.hpp"

namespace ilrd
{

// Responsibility:
// schedule tasks that will be executed in a future timepoint - interval from
// now or specific time - overload Add method
class Scheduler : private boost::noncopyable
{
public:
    
    typedef FDTimer::MicroSecs MicroSecs;
    typedef boost::chrono::system_clock::time_point TimePoint;
    typedef FDTimer::ActionFunc ActionFunc;

    // Scheduler Constructor
    // Receives: reactor - a reference to a reactor to work with
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             created
    explicit Scheduler(Reactor& reactor);

    // Schedule Action
    // Receives: timepoint - a boost::chrono::system_clock::time_point object
    //                       with the time requested
    //           function - an function/functor to invoke at the timepoint
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             written
    void ScheduleAction(TimePoint timepoint, ActionFunc function);

    // Schedule Action
    // Receives: microseconds - a chrono::microseconds object with the time
    //           requested from now
    //           function - an function/functor to invoke at the timepoint
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             written
    void ScheduleAction(MicroSecs microseconds,
                        ActionFunc function); // boost::chrono::microseconds

private:
    struct Task
    {
        inline bool operator<(const Task& other_) const;
        inline bool operator==(const Task& other_) const;
        TimePoint m_timepoint;
        ActionFunc m_function;
    };

    void InvokeCurrentTask(int fd);
    bool IsSetTimerNeeded(Task& new_task) const;
    void SetTimer(MicroSecs time);
    inline MicroSecs GetDurationForNextTask() const;

    FDTimer m_timer;
    std::priority_queue< Task > m_tasks;
};

////////////////////////////////////////////////////////////////////////////////
///   inline functions implementation   ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline Scheduler::MicroSecs Scheduler::GetDurationForNextTask() const
{
    return (GetDuration(m_tasks.top().m_timepoint));
}

inline bool Scheduler::Task::operator<(const Task& other_) const
{
    return (!(m_timepoint < other_.m_timepoint));
}

inline bool Scheduler::Task::operator==(const Task& other_) const
{
    return (m_timepoint == other_.m_timepoint);
}

} // namespace ilrd

#endif /* ILRD_RD8586_SCHEDULER_HPP */
