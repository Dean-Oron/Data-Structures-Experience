/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 20.06.20									      */
/*						Reviewer - Ivenna   							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_PLUG_N_PLAY_HPP__
#define __ILRD_RD8586_PLUG_N_PLAY_HPP__
#include<string>
#include <iostream>
#include <boost/bind.hpp>
#include "dispatcher.hpp"
#include "shared_object.hpp"
#include "dir_monitor.hpp"

namespace ilrd
{
class DLLoader
{
public:
    explicit DLLoader(const std::string& plug_in_path);
    ~DLLoader();
    
private:
    void AddSharedObj(const std::string lib_name);
    
    std::string m_plug_in_path;
    std::list<SharedObject *> m_libs;
    DirMonitor m_dirmonitor;
    Callback<Dispatcher <std::string> > m_callback;
};

}// ilrd namespace
#endif//__ILRD_RD8586_PLUG_N_PLAY_HPP__