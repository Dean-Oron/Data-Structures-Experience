/******************************************************************************/
/* 						PriorityQueue API                 					  */
/*						Author - Dean Oron									  */
/*						Date - 06.08.20 								      */
/*						Reviewer - Ali Hudson    						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_PRIORITYQUEUE_HPP
#define ILRD_RD8586_PRIORITYQUEUE_HPP

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

namespace ilrd
{

template <typename T, typename Container = std::vector<T>,
  typename Compare = std::less<typename Container::value_type>>
class PriorityQueue : private std::priority_queue<T, Container, Compare>
{
public:
    explicit PriorityQueue(const Compare& comp = Compare(),
                         const Container& ctnr = Container()):PriorityQueue::priority_queue(comp, ctnr)
    {
    }

    /*template <class InputIterator>
    PriorityQueue(InputIterator first, InputIterator last,
                         const Compare& comp = Compare(),
                         const Container& ctnr = Container());*/ //not mandatory (bonus)

    void push (const T& val);
    void pop();
    const T& front() const;
    bool empty() const;
};

template <typename T, typename Container, typename Compare>
void PriorityQueue<T, Container, Compare>::push(const T& value)
{
    priority_queue<T, Container, Compare>::push(value);
}

template <typename T, typename Container, typename Compare>
void PriorityQueue<T, Container, Compare>::pop()
{
    priority_queue<T, Container, Compare>::pop();
}

template <typename T, typename Container, typename Compare>
const T& PriorityQueue<T, Container, Compare>::front() const
{
    return (priority_queue<T, Container, Compare>::top());
}

template <typename T, typename Container, typename Compare>
bool PriorityQueue<T, Container, Compare>::empty() const
{
    return (priority_queue<T, Container, Compare>::empty());
}

} // namespce ilrd

#endif // ILRD_RD8586_PRIORITYQUEUE_HPP