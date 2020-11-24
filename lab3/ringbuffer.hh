/*
 * @Author: tekky 
 * @Date: 2020-11-11 18:18:22 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-11 18:48:27
 */

#ifndef __RINGBUFFER_HH__
#define __RINGBUFFER_HH__

#include <vector>

struct Buffer {
    char * data;
    unsigned size;
};

class RingBuffer {

public: // interface...
    explicit RingBuffer(unsigned size);
    ~RingBuffer();

private: // mothods...

private: // data members...
    unsigned in;    // write pointer.
    unsigned out;   // read pointer.
    std::vector<Buffer> buf;
};

#endif /* __RINGBUFFER_HH__ */