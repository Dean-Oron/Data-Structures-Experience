#include <iostream>
#include <vector>
#include <queue>
#include <map> 
#include <boost/function.hpp>
#include <utility>
#include <sys/select.h>
#include <cassert>

#include "listener.hpp"

using namespace ilrd;

vector<ModeAndHandle> Listener2::Listen(const vector<ModeAndHandle>& handle)
{
    fd_set read_set;
    fd_set write_set;
    fd_set exception_set;
    int max_fd = 0;
    int value;

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exception_set);
    // Putting the handlers into file descriptors lists
    for_each(handle.begin(), handle.end(), SetInsertion(&read_set, &write_set,
                                &exception_set, max_fd));

    value = select(FD_SETSIZE, &read_set, &write_set, &exception_set, NULL);

    cout << "select returns " << value << endl;
    ModeAndHandle to_insert;
    vector<ModeAndHandle> ready_pairs;

    for (int i = 0; i <= max_fd; ++i)
    {
        if (FD_ISSET(i, &read_set))
        {
            to_insert.first = READ;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }

        else if (FD_ISSET(i, &write_set))
        {
            to_insert.first = WRITE;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }
        else if (FD_ISSET(i, &exception_set))
        {
            to_insert.first = EXCEPTION;
            to_insert.second = i;
            ready_pairs.push_back(to_insert);
        }
    }

    return (ready_pairs);
}