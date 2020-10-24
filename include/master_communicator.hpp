/******************************************************************************/
/* 						Master Communicator API            					  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_MASTER_COMMUNICATOR_HPP__
#define __ILRD_RD8586_MASTER_COMMUNICATOR_HPP__

#include <boost/noncopyable.hpp> // boost::noncopyable
#include <boost/function.hpp> /* function */

#include "udp_connector.hpp"
#include "reactor.hpp"
#include "protocol.hpp"
#include "source_callback.hpp"

namespace ilrd
{
    class MasterCommunicator : private boost::noncopyable
    {
    public:
        typedef boost::function <void(const Request&)> ActionRequest;

           //struct of minion information 
    struct MinionInfo
    {
        std::string m_ip;
        id_minion_t m_myuid;
        std::list<offset_t> m_indexs_list;
        int m_port;
        bool m_status;
    };

        explicit MasterCommunicator(int port, Reactor& reactor, 
                                              ActionRequest ar_func);

        // ReadRequest Function (added to reactor)
        // Receives: file descriptor.
        // Returns: nothing
        void ReadRequest(int fd);

        // Reply Function
        // Receives: const refernce to class Response.
        // Returns: nothing
        void Reply(const Response& res) const;

    private:
        ActionRequest m_ar_func;
        Reactor& m_reactor;
        UDPConnector m_con;
        const int m_port;
        Callback<Source<int>> m_callback;
        Request *m_request;
        sockaddr_in m_client_addr;
    };

} // namespace ilrd

#endif /* __ILRD_RD8586_MASTER_COMMUNICATOR_HPP__ */