#include "command.hpp"

namespace ilrd
{

WriteCommand::WriteCommand(Params params) : m_params(params)
{
}

void WriteCommand::operator()()
{
    
    m_params.m_response.m_status = m_params.m_storage.Write(m_params.m_request.m_index, (void*)&(m_params.m_request.m_data));
}

ReadCommand::ReadCommand(Params params) : m_params(params)
{
}

void ReadCommand::operator()()
{
    m_params.m_storage.Read(m_params.m_request.m_index, (void *)&m_params.m_response.m_data);
}

BaseCommand* WriteBuilder(Params params)
{
    return (new WriteCommand(params));
}

BaseCommand* ReadBuilder(Params params)
{
    return (new ReadCommand(params));
}

} //namespace ilrd