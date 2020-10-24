/******************************************************************************/
/* 						Master Communicator API            					  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_MINION_HPP__
#define __ILRD_RD8586_MINION_HPP__

#include <boost/noncopyable.hpp> // boost::noncopyable

#include "protocol.hpp"
#include "reactor.hpp"
#include "master_communicator.hpp"
#include "storage.hpp"
#include "listener.hpp"
#include "command.hpp"

namespace ilrd
{
class Minion: private boost::noncopyable
{
public:
    explicit Minion(int port, int num_of_blocks, int master_port);

    //~Minion();
    void Run();
private:

    // OnRequest
    // Receives: Request structure
    // Returns: nothing
    void OnRequest(const Request& request);

    Listener2 m_listener;
    Reactor m_reactor;
    MasterCommunicator m_comm;
    Storage<MAX_BLOCK_SIZE> m_storage;
    //Response m_response;
};

} // namespace ilrd

#endif /* __ILRD_RD8586_Minion_HPP__ */