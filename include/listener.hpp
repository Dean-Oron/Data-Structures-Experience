/******************************************************************************/
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer - Amir     							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef __ILRD_RD8586_LISTENER_HPP__
#define __ILRD_RD8586_LISTENER_HPP__

#include "reactor.hpp"

using namespace std;
namespace ilrd
{
class Listener2: public IListener
{
public:
    Listener2():IListener(){}
    vector<ModeAndHandle> Listen(const vector<ModeAndHandle>& handle);
};

}// ilrd namespace
#endif // __ILRD_RD8586_LISTENER_HPP__