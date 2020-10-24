/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer -                  					      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_DIR_MONITOR_HPP__
#define __ILRD_RD8586_DIR_MONITOR_HPP__
#include<map>
#include<string>
#include <iostream>
#include <boost/bind.hpp>
#include "dispatcher.hpp"

namespace ilrd
{

class DirMonitor
{   
public:
    explicit DirMonitor(std::string dir_path): m_path(dir_path), m_running(true){}
    ~DirMonitor()
    {
        m_running = false;
    }

    void Subscribe(Callback<Dispatcher<std::string> > *callback);
    void UnSubscribe(Callback<Dispatcher<std::string> > *callback);
    void StartMonitoring();

private:
    std::string m_path;
    Dispatcher<std::string> m_dispatcher;
    bool m_running;
    void Running();
};

}// ilrd namespace
#endif//__ILRD_RD8586_DIR_MONITOR_HPP__