#include <iostream>
#include <sys/inotify.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include "dir_monitor.hpp"

namespace ilrd
{
#define EVENT_SIZE (sizeof(struct inotify_event))
void DirMonitor::Subscribe(Callback<Dispatcher<std::string> > *callback)
{
    if (!callback)
    {
        LOG_ERROR("callback function was not arrived to monitor");
    }

    LOG_INFO("Subscribed new callback to dispatcher");
    m_dispatcher.Subscribe(callback);
}
void DirMonitor::UnSubscribe(Callback<Dispatcher<std::string> > *callback)
{
    if (!callback)
    {
        LOG_ERROR("callback function was not arrived to monitor");
    }

    LOG_INFO("UnSubscribed new callback to dispatcher");
    
    m_dispatcher.Unsubscribe(callback);
}

void DirMonitor::StartMonitoring()
{
    LOG_INFO("Started monitoring");
    boost::thread thread{&DirMonitor::Running, this};
}

void DirMonitor::Running()
{
    int fd;
    int watch_descriptor;
    char buf[4096];
    int len;

    if (-1 == (fd = inotify_init()))
    {
        LOG_ERROR("Dir Monitor: inotify_init1 failed");
        exit(EXIT_FAILURE);
    }

    if (-1 == (watch_descriptor = inotify_add_watch(fd, m_path.c_str(), IN_OPEN | IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE | IN_MOVED_FROM)))
    {
        LOG_ERROR("Dir Monitor: inotify_add_watch failed");
        exit(EXIT_FAILURE);
    }
////////////////////////////////////////////////////////
    LOG_DEBUG("Watching now the directory");
    int i = 0;

    boost::filesystem::directory_iterator dirEnd;
    for (boost::filesystem::directory_iterator dirIterator(m_path.c_str());
         dirIterator != dirEnd; ++dirIterator)
    {
        m_dispatcher.Notify(dirIterator->path().filename().c_str());
    }

    while (m_running)
    {
        i = 0;
        len = read(fd, buf, sizeof(buf));
        if (len == -1 && errno != EAGAIN) 
        {
            LOG_ERROR("Dir Monitor: read failed");
            exit(EXIT_FAILURE);
        }

        struct inotify_event *event = (struct inotify_event *)buf;
        
        while(i < len)
        {
            if (event->len)
            {
                if (event->mask & IN_CLOSE_WRITE)
                {
                    LOG_INFO("Notified the loader a Shared Object was added");
                    m_dispatcher.Notify(event->name);
                }
            }

            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, watch_descriptor);
    close(fd);
}

}//namespace ilrd