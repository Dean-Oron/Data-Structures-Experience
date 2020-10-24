///////////////////////////////////////////////////////////////////////BS"S**///
//	Title:		Master Storage Manager
//	Authour:	Dean Oron
//	Reviewer:	Shmuel Sinder
//	Version:	05.09.2020.0
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


#include "protocol.hpp"
#include "master_storage_manager.hpp"

#define BLOCK_SIZE      (4096) 

using namespace ilrd;

MasterStorageManager::MasterStorageManager(size_t num_of_minion, size_t num_of_blocks)
 : m_num_of_minions(num_of_minion), m_num_of_block(num_of_blocks), m_is_active(false)
{}

void MasterStorageManager::ReplaceMinion()
{
    size_t minion_id = 0;

    std::list<boost::shared_ptr<MinionInfo> >:: iterator list_it;
    for (list_it = m_minions_list.begin(); list_it != m_minions_list.end(); ++list_it)
    {
        if ((*list_it)->GetStatus() == false)
        {
            minion_id = (*list_it)->GetId();
            (*list_it) = m_bkup_minions_list.front();
            m_bkup_minions_list.pop_front();
            break;
        }
    }

    //replace in the map the minion_id pointer for the new mionion pointer

    switch (minion_id)
    {
    case 1: //bkup is minion 3
        //Write in minion 1 all odd block in minion 3
        /* code */
        break;
    case 2: //bkup is minion 4
        //Write in minion 2 all even block in minion 2
        /* code */
        break;
    case 3: //bkup is minion 1
        //Write in minion 3 all odd block in minion 1
        /* code */
        break;
    case 4: //bkup is minion 2
        //Write in minion 4 all even block in minion 2
        /* code */
        break;
    
    default:
        break;
    }

    
}

void MasterStorageManager::InitMap()
{
    std::cerr << "InitMap\n";

    std::list<boost::shared_ptr<MinionInfo> >::const_iterator list_it = m_minions_list.begin();
    boost::shared_ptr<MinionInfo> minion1 = *list_it;
    list_it++;
    boost::shared_ptr<MinionInfo> minion2 = *list_it;
    list_it++;
    boost::shared_ptr<MinionInfo> minion_bkup1 = *list_it;
    list_it++;
    boost::shared_ptr<MinionInfo> minion_bkup2 = *list_it;

    for (size_t i = 0; i < m_num_of_block; ++i)
    {
        if (i % 2 == 0)
        {
            m_offset_map[i].push_back(minion1);
            m_offset_map[i].push_back(minion_bkup1);
        }
        else
        {
            m_offset_map[i].push_back(minion2);
            m_offset_map[i].push_back(minion_bkup2);
        }
    }
}

void MasterStorageManager::RegisterNewMinion(boost::shared_ptr<MinionInfo> minion)
{
    static bool is_map_init = false;

    std::cerr << "RegisterNewMinion\n";

    // Init the map first
    if (!is_map_init)
    {
        // push 4 minioninfo into the minion_list
        m_minions_list.push_back(minion);
        
        // only continue after you have 4 minoins in the list
        if (m_minions_list.size() != m_num_of_minions)
        {
            return;
        }

        // now you can initialize the map
        InitMap();
        is_map_init = true;
    }

    // any extre minion will go to the backup list
    m_bkup_minions_list.push_back(minion);

    // check if there is a need of replace mionion and replace
    ReplaceMinion();
}

void MasterStorageManager::UnregisterMinion(boost::shared_ptr<MinionInfo> minion)
{
    std::cout << "UnregisterMinion\n";
    minion->SetStatus(false);
}

Request* MasterStorageManager::BuiltRequest(Request::MODE mode, uint64_t uid, uint64_t index)
{
    Request *request = 
                    (Request*)operator new(MSG_SIZE + Request().RequestSize());

    request->m_mode = mode;
    request->m_uid = uid;
    request->m_index = index;

    return request;

}

void MasterStorageManager::Write(size_t offset, char *data)
{
    assert(m_minions_list.size() >= 4);
    assert(offset <= m_num_of_block);
    offset = offset % m_num_of_block;

    bool is_data_lost = true;

    std::list<boost::shared_ptr<MinionInfo> >::const_iterator list_it;
    
    // look for one active minion
    for (   list_it = m_offset_map[offset].begin(); 
            list_it != m_offset_map[offset].end(); ++list_it)
    {
        if ((*list_it)->GetStatus() == true)
        {
            // build the request with READ
            Request* request = BuiltRequest(Request::WRITE, 
                (*list_it)->GetId(),
                offset);

            memcpy((char*)request->m_data, data, MSG_SIZE);
            
            // Call MionionInfo Send funct
            (*list_it)->Send(offset, (char*)request);
            // one was found so data is not lost
            is_data_lost = false;

            delete request;
        }
    }
            
    if (is_data_lost)
    {
        throw(std::runtime_error("no active minion found - data lost"));
    }

}
void MasterStorageManager::Read(size_t offset)
{
    assert(m_minions_list.size() >= 4);
    assert(offset <= m_num_of_block);

    bool is_data_lost = true;

    // look for one active minion
    std::list<boost::shared_ptr<MinionInfo> >::const_iterator list_it;

    for (   list_it = m_offset_map[offset].begin(); 
            list_it != m_offset_map[offset].end(); ++list_it)
    {
        if ((*list_it)->GetStatus() == true)
        {
            // build the request with READ
            Request* request = BuiltRequest(Request::READ, 
                (*list_it)->GetId(),
                offset);
            // Call MionionInfo Send funct
            (*list_it)->Send(offset, (char*)request);
            // one was found so data is not lost
            is_data_lost = false;

            delete request;

            // once you find an active minion you break
            break;
        }
    }
            
    if (is_data_lost)
    {
        throw(std::runtime_error("no active minion found - data lost"));
    }

}

