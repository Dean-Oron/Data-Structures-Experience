#include <iostream>         // ostream
#include "storage.hpp"      // storage


#include <iostream>
#include <cstring>

#include "storage.hpp"
#include "protocol.hpp"
#include "command.hpp"
#include "factory.hpp"
using namespace ilrd;

int main()
{
    Storage < MAX_BLOCK_SIZE > storage1(100);
    Factory<int, BaseCommand,Params, BaseCommand* (*)(Params)> fact;
    Request *request = static_cast<Request*>(operator new(sizeof(Request) + MAX_BLOCK_SIZE));
    Response* response = NULL;
    Response* response2 = NULL;
    size_t responseSize = offsetof(Response, m_data[0]) + MAX_BLOCK_SIZE;
    response = (Response*)operator new(responseSize);
    response2 = (Response*)operator new(responseSize);

    char str[] = "Hello";
    
    request->m_mode = 0;
    request->m_uid = 12345;
    request->m_index = 1;
    std::memcpy(request->m_data, str, sizeof(str));

    Params par = {storage1, *response, *request};
    Params par2 = {storage1, *response2, *request};

    fact.Add(1, WriteBuilder);
    fact.Add(0, ReadBuilder);
    //Write
    WriteCommand* write_com = (WriteCommand*)fact.Create(1, par2);
    (*write_com)();
    //Read
    ReadCommand* read_com = (ReadCommand*)fact.Create(0, par);
    (*read_com)();

    std::cout << par.m_response.m_data << std::endl;

    operator delete (request);
    operator delete (response);
    operator delete (response2);

    delete write_com;
    delete read_com;

    return(0);
}
