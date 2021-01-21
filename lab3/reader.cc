/*
 * @Author: tekky
 * @Date: 2020-11-25 16:59:27
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:05:23
 */

#include "shmringbuffer.hpp"
#include "utils.hh"

int main(int argc, char const *argv[]) {
  if (argc != 1) {
    fprintf(stderr, "error argument\n");
    exit(1);
  }
  const int semid = std::stoi(argv[0]);
  ShmRingBuffer<Message> buf(semid);
  // FILE * fp = fopen("outfile.out", "w");
  // int fd = open("outfile.out", O_CREAT | O_TRUNC | O_WRONLY);
  while (true) {
    printf("%s\n", buf.pop_front().msg);
    // fprintf(fp, "%s\n", buf.pop_front().msg);
    // write(fd, buf.pop_front().msg, MAX_SIZE);
  }
  return 0;
}
