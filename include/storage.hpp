/******************************************************************************/
/* 						Storage                      						  */
/*						Author - Dean Oron									  */
/*						Date - 17.07.20 								      */
/*						Reviewer - Yehuda       						      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_STORAGE_HPP
#define ILRD_RD8586_STORAGE_HPP

#include <iostream>
#include <string>
#include <fstream>

#include "protocol.hpp"
#include "boost/core/noncopyable.hpp"
#include "logger.hpp"

using namespace std;

namespace ilrd
{
template <size_t BLOCK_SIZE>
class Storage:private boost::noncopyable
{
public:
    // Possible return status: 0 == SUCCESS, 1 == FAILED TO OPEN, 2 == FAILED TO WRITE, 3 == FAILED TO READ, 4 == OUT OF RANGE 
    explicit Storage(size_t num_of_blocks);
    // Write Function
    // Receives: index where to write data, pointer to source.
    // Returns: status of operation        
    int Write(size_t index, const void *src);
    // Read Function
    // Receives: index from where to read, pointer to store read data .
    // Returns: status of operation
    int Read(size_t index, void *buffer) const;

    // GetCapacity Function
    // Receives: nothing.
    // Return: size_t number of blocks     
    inline size_t GetCapacity() const { return m_capacity; }
private:
    size_t m_capacity;
    string m_file_name;
};

template<size_t BLOCK_SIZE>
Storage<BLOCK_SIZE>::Storage(size_t num_of_blocks)
{
    this->m_capacity = num_of_blocks;
    m_file_name = "root_dir.txt";
    fstream file;
    file.open(m_file_name, ios::app);
    file.close();
}

template<size_t BLOCK_SIZE>
int Storage<BLOCK_SIZE>::Write(size_t index, const void *src)
{
    size_t offset =BLOCK_SIZE * index;
    fstream file;
    file.open(m_file_name);
    file.seekp(offset, ios_base::beg);
    file.write((char *)src,BLOCK_SIZE);
    file.close();

    return (0); // read about exit status of read..
}

template<size_t BLOCK_SIZE>
int Storage<BLOCK_SIZE>::Read(size_t index, void *buffer) const
{
    size_t offset =BLOCK_SIZE * index;
    fstream file;
    file.open(m_file_name);
    file.seekg(offset, ios_base::beg);
    file.read((char *)buffer,BLOCK_SIZE);
    file.close();

    return (0);
}

} // namespace ilrd

#endif // ILRD_RD8586_STORAGE_HPP   

