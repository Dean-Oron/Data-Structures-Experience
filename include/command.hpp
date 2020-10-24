/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer -                  					      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_COMMAND_HPP
#define ILRD_RD8586_COMMAND_HPP

#include "protocol.hpp"
#include "storage.hpp"
#include "factory.hpp"
#include "singleton.hpp"

namespace ilrd
{

class BaseCommand
{
public:
    inline virtual ~BaseCommand() = 0;
    virtual void operator()() = 0;
};

struct Params
{
    Storage<MAX_BLOCK_SIZE>& m_storage;
    Response& m_response;
    const Request& m_request;
};

typedef Singleton<Factory <int, BaseCommand, struct Params> > CommandFactory;

class WriteCommand : public BaseCommand
{
public:
    WriteCommand(Params params);
    virtual ~WriteCommand() {}
    virtual void operator()();

private:
    Params m_params;
};

class ReadCommand : public BaseCommand
{
public:
    ReadCommand(Params params);
    virtual ~ReadCommand() {}
    virtual void operator()();
private:
    Params m_params;
};

// inline Definition of destructor interface
inline BaseCommand::~BaseCommand()
{
}

//BUILDERS

BaseCommand* WriteBuilder(Params params);
BaseCommand* ReadBuilder(Params params);

} //ilrd namespace

#endif //ILRD_RD8586_COMMAND_HPP 