/*
 * @Author: tekky 
 * @Date: 2020-11-25 16:59:27 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:05:23
 */

#include "utils.hh"
#include "shmringbuffer.hpp"

int main(int argc, char const *argv[])
{
    if (argc != 1) {
        fprintf(stderr, "error argument\n");
        exit(1);
    }
    const int semid = std::stoi(argv[0]);
    ShmRingBuffer<Message> buf(semid);
    while (true) {
        printf("%s\n", buf.pop_front().msg);
    }
    return 0;
}
