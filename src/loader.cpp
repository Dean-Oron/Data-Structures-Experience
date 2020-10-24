#include <dlfcn.h>
#include <sys/inotify.h>

#include "loader.hpp"
#include "logger.hpp"

namespace ilrd
{
DLLoader::DLLoader(const std::string& plug_in_path) 
:m_plug_in_path(plug_in_path), m_dirmonitor(plug_in_path), m_callback(boost::bind(&ilrd::DLLoader::AddSharedObj, this, _1))
{
    m_dirmonitor.Subscribe(&m_callback);
    m_dirmonitor.StartMonitoring();
}

DLLoader:: ~DLLoader()
{}

void DLLoader::AddSharedObj(const std::string lib_name)
{
    LOG_INFO("Entered to AddSharedObj");
    void (*init)();
    std::string lib_path(m_plug_in_path);
    lib_path += lib_name;
    std::string symbol("Init2");

    SharedObject *new_shared_object = new SharedObject(lib_path);
    m_libs.push_front(new_shared_object);
    LOG_INFO("Shared Object was pushed to list");

    *(void**)&init = new_shared_object->LoadSymbol<void (*)()>(symbol);

    LOG_INFO("End of AddSharedObj");
}

}//ilrd