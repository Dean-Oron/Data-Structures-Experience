#include "logger.hpp"
#include "singleton.hpp"

using namespace ilrd;

int main()
{
    LOG_DEBUG("Debug logging test");
    LOG_ERROR("Error logging test");
    LOG_INFO("Info logging test");
    LOG_WARNING("Warning logging test"); 

    Singleton<Logger>::CleanUp();

    return (0);
}