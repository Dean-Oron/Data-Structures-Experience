#include <iostream>

#include "reactor.hpp"

namespace ilrd
{
void FDMax(int& fd, int& max_fd)
{
    if (fd > max_fd)
    {
        max_fd = fd;
    }
}

Reactor::~Reactor()
{
    for (map<ModeAndHandle, Source<int> *>::iterator iter = m_EventHandlers.begin();
                        iter != m_EventHandlers.end(); ++iter)
    {
        delete iter->second;
    }
}

void Reactor::Run()
{
    m_stop = 0;
    // Will run until stop changes to 1
    while (!m_stop && !m_EventHandlers.empty())
    {
        vector<ModeAndHandle> all_handles;
        vector<ModeAndHandle> ready_handles;
    
    // Filling the vector with handels
        for (map<ModeAndHandle, Source<int> *>::iterator iter = m_EventHandlers.begin();
                            iter != m_EventHandlers.end(); ++iter) 
        { 
            all_handles.push_back(iter->first); 
        }
        ready_handles = m_Listener->Listen(all_handles);

        for (vector<ModeAndHandle>::iterator v_iter = ready_handles.begin();
                                        v_iter != ready_handles.end(); ++v_iter) 
        { 
            m_EventHandlers[*v_iter]->Notify((*v_iter).second); //create key if not exist 

            // Preventing function callback running after Stop() function
            if (m_stop)
            {
                return;
            }
        }
    }
}

void Reactor::Stop()
{
    m_stop = 1;
}
}// ilrd