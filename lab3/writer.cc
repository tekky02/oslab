/*
 * @Author: tekky 
 * @Date: 2020-11-25 16:50:05 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:26:23
 */

#include <fstream>
#include "shmringbuffer.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 1) {
        fprintf(stderr, "error argument\n");
        exit(1);
    }
    const int semid = std::atoi(argv[0]);
    ShmRingBuffer<Message> buf(semid, Auth::owner);
    Message msg;
    std::ifstream fin("infile.out");
    while (!fin.eof()) {
        if (fin.eof()) break;
        fin.getline(msg.msg, MAX_SIZE);
        buf.push_back(msg);
    }
    fin.close();
    return 0;
}
