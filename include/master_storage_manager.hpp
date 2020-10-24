#ifndef __ILRD_RD8586_MASTER_STORAGE_MANAGER_HPP__
#define __ILRD_RD8586_MASTER_STORAGE_MANAGER_HPP__
///////////////////////////////////////////////////////////////////////BS"S**///
//	Title:		Master Storage Manager
//	Authour:	Dean Oron
//	Reviewer:	Shmuel Sinder
//	Version:	05.09.2020.0
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <boost/noncopyable.hpp>// boost::noncopyable
#include <boost/shared_ptr.hpp> // shared pointer 

#include "minion_info.hpp"
#include "protocol.hpp"

namespace ilrd
{
struct MinionInfo; // should be defined ASAP within the communicator

class MasterStorageManager: private boost::noncopyable
{
public:
    typedef int id_minion_t;//each minion will recive it own id
    typedef size_t offset_t;//index of data segment from the request 
    typedef std::list<boost::shared_ptr<MinionInfo> > minion_list_t;

    MasterStorageManager(size_t num_of_minion, size_t num_of_blocks);
    // ~MasterStorageManager(); default 
    
    // RegisterNewMinion
    // Recieve - A minion with its info
    // Responsible to update the m_offset_map, m_minions_list.
    void RegisterNewMinion(boost::shared_ptr<MinionInfo> minion);

    // UnregisterMinion
    // Receive - minion_id to remove
    // It will duplicate the information to the available minion.
    // Responsible to update the m_offset_map, m_minions_list.
    void UnregisterMinion(boost::shared_ptr<MinionInfo> minion);

    //This method find the relevant minions to write to and send the data to them
    void Write(size_t offset, char *data);

    //This method find the minion to read from and return the data.
    void Read(size_t offset);
    
private:
    //map of pairs of minions that store the related offsets.
    std::map<offset_t, minion_list_t > m_offset_map;

    //list of all existing minions in our system.
    minion_list_t m_minions_list;
    minion_list_t m_bkup_minions_list;

    size_t m_num_of_minions;
    size_t m_num_of_block;
    bool m_is_active;

    void InitMap();
    void ReplaceMinion();

    static Request* BuiltRequest(Request::MODE mode, uint64_t uid, uint64_t index);
};
} // namespace ilrd

#endif /* __ILRD_RD8586_MASTER_STORAGE_MANAGER_HPP__ */