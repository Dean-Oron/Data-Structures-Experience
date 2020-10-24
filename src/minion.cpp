#include <iostream>
#include <boost/bind.hpp>

#include "minion.hpp"
#include "master_communicator.hpp"
#include "reactor.hpp"
#include "storage.hpp"
#include "listener.hpp"

using namespace ilrd;

Minion::Minion(int port, int num_of_blocks, int master_port)
:m_listener(), m_reactor(&m_listener), 
m_comm(port, m_reactor, boost::bind(&Minion::OnRequest, this, _1)), m_storage(num_of_blocks)
{
    (void)master_port;
    CommandFactory::GetInstance()->Add(0, ReadBuilder);
    CommandFactory::GetInstance()->Add(1, WriteBuilder);
}

void Minion::Run()
{
    m_reactor.Run();
}

void Minion::OnRequest(const Request& request) 
{
    size_t responseSize = offsetof(Response, m_data[0]) + (1 - request.m_mode) * MAX_BLOCK_SIZE;

    Response* response = NULL;
    response = (Response*)operator new(responseSize);
    memset(response, 0, responseSize);

    Params params{m_storage, *response, request};
    BaseCommand* command = CommandFactory::GetInstance()->Create(request.m_mode, params);

    (*command)();

    m_comm.Reply(*response); 
}