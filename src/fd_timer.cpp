/******************************************************************************/
/* 						Scheduler implementation         					  */
/*						Author - Dean Oron									  */
/*						Date - 26.07.20 								      */
/*						Reviewer - Lior Cohen   						      */
/*						Open Lab 86											  */
/******************************************************************************/

#include <sys/timerfd.h> // timerfd_create, timerfd_set
#include <boost/bind.hpp>                 // boost::bind
#include <boost/chrono/system_clocks.hpp> // boost::chrono::system_clock

#include "fd_timer.hpp" // FDTimer Class
#include"logger.hpp"

using namespace ilrd;
using namespace std;

FDTimer::FDTimer(Reactor& reactor, ActionFunc callback_func)
    : m_reactor(reactor),
      m_callback(callback_func),
      m_fd(timerfd_create(CLOCK_MONOTONIC, 0))
{
    if (m_fd < 0)
    {
        LOG_ERROR("Failed to create timer file descripteor");
        throw std::runtime_error("can't create timerfd");
    }

    ModeAndHandle fd_and_mode = {READ, m_fd};

    //Registering the fd_timer reactor
    m_reactor.Add(fd_and_mode, &m_callback);
}

FDTimer::~FDTimer()
{
    if (m_fd >= 0)
    {
        close(m_fd);
    }
}

void FDTimer::Set(MicroSecs microseconds)
{
    if (microseconds.count() < 0)
    {
        microseconds = MicroSecs(500);
    }

    // create itimerspec struct for the time given
    struct itimerspec timerSpec;
    
    //memset(&timerSpec, 0, sizeof(timerSpec));

    timerSpec.it_value.tv_sec = microseconds.count() / THOUSAND;
    timerSpec.it_value.tv_nsec = (microseconds.count() % 1000) * MILLION;

    timerSpec.it_interval.tv_nsec = 0;
    timerSpec.it_interval.tv_sec = 0;

    int timer_set_check = 0;
    
    if ((timer_set_check = timerfd_settime(m_fd, 0, &timerSpec, NULL)) < 0)
    {
        LOG_ERROR("Failed to set the file descriptor timer");
        throw std::runtime_error("timer file descriptor set failed");
    }
}

void FDTimer::Unset()
{
    struct itimerspec timerSpec;
    memset(&timerSpec, 0, sizeof(timerSpec));

    if (timerfd_settime(m_fd, 0, &timerSpec, NULL) < 0)
    {
        LOG_ERROR("Failed to clear fd timer");
        throw std::runtime_error("can't clear timerfd");
    }
}