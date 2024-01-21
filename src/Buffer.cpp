//
//

#include "../h/Buffer.hpp"

buffer::buffer() : readCursor(0), writeCursor(0), size(0)
{
    _sem::createSemaphore(&mutex, 1);
    _sem::createSemaphore(&space_available, BUFFER_SIZE);
    _sem::createSemaphore(&item_available, 0);
}

buffer::~buffer()
{
    delete mutex;
    delete space_available;
    delete item_available;
}




void buffer::append(char c)
{
    space_available->wait();
    mutex->wait();

    

    data[writeCursor] = c;
    this->size++;
    writeCursor = (writeCursor+1) % BUFFER_SIZE;

    mutex->signal();
    item_available->signal();
}


char buffer::take()
{
    item_available->wait();
    mutex->wait();

  

    char c = data[readCursor];
    readCursor = (readCursor +1) % BUFFER_SIZE;
    this->size--;

    mutex->signal();
    space_available->signal();

    return  c;
}


/*
 * There are BLOCK_SIZE and NUM_OF_BLOCKS variables on the June 2016 deadline solution
 * because in that solution the get method was implemented by purchasing the entire block of data.
 *
 * In my case, since I buy character by character, NUM_OF_BLOCKS is the same as BLOCK_SIZE because one character
 * is placed in one element of this array. The conclusion is that then the menu BLOCK_SIZE = 1
 * */