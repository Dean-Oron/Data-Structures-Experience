/******************************************************************************/
/* 						FDTimer implementation            					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_FD_TIMER_HPP
#define ILRD_RD8586_FD_TIMER_HPP

#include <boost/chrono.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "source_callback.hpp"
#include "reactor.hpp"

const int MILLION = 1000000;
const int THOUSAND = 1000;

//using namespace boost::chrono;
namespace ilrd
{

// use #include <sys/timerfd.h> in the implementation
class FDTimer : private boost::noncopyable
{
public:

    // typedefs for inner types
    typedef Callback< Source< int > >::CallbackPointer ActionFunc;
    typedef boost::chrono::milliseconds MicroSecs;

    // FDTimer Constructor
    // Receives: reactor - a reference to a reactor to work with
    //           callback_func - a handler function to invoke when the reactor
    //                           calls timer file descriptors' handler
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             created
    explicit FDTimer(Reactor& reactor, ActionFunc callback_func);

    // FDTimer Destructor
    // Exceptions: no exceptions
    ~FDTimer();

    // Set
    // Receives: microseconds - a chrono::microseconds object with the time
    //           requested from now
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             written
    void Set(MicroSecs microseconds);

    // Unset
    // Exceptions: throws runtime_error if timer file descriptor can't be
    //             written
    void Unset();
    

private:

    inline void UpdateReactorDead();
    Reactor& m_reactor;
    Callback< Source< int > > m_callback;
    int m_fd;
};


////////////////////////////////////////////////////////////////////////////////
///   inline functions implementation   ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline void UpdateReactorDead()
{
    LOG_WRN("reactor is not alive");
}

typedef boost::chrono::system_clock::time_point TimePoint;

// Return: TimePoint object with current system time
inline TimePoint Now()
{
    return (boost::chrono::system_clock::now());
}

// Get Duration
// Receives: timePoint - requested timepoint in future
// Return: MicroSecs object with the duration of time from now to timePoint

inline FDTimer::MicroSecs GetDuration(TimePoint timePoint)
{
    return (boost::chrono::duration_cast< FDTimer::MicroSecs >(timePoint - ilrd::Now()));
}

// Microseconds To Seconds
// Receives: microSecs - microseconds to convert
// Return: time_t for microseconds in seconds
inline time_t MicroToSecs(FDTimer::MicroSecs microSecs)
{
    return (microSecs.count() / MILLION);
}

// Microseconds to Nanos
// Receives: microSecs - microseconds to convert
// Return: time_t for microseconds in nanoseconds
inline time_t MicroToNanos(FDTimer::MicroSecs microSecs)
{
    return ((microSecs.count() % MILLION) * THOUSAND);
}

} // namespace ilrd

#endif /* ILRD_RD8586_FD_TIMER_HPP */


