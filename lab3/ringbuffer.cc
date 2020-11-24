/*
 * @Author: tekky 
 * @Date: 2020-11-11 18:22:42 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 18:49:23
 */

#include "ringbuffer.hh"

RingBuffer::RingBuffer(unsigned size)
    : buf(size)
{
}

RingBuffer::~RingBuffer() {
    
}

