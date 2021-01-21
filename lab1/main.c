/*
 * @Author: tekky
 * @Date:   2020-11-03 11:14:15
 * @Last Modified by:   tekky
 * @Last Modified time: 2020-11-03 11:18:10
 */

#include <signal.h>   // for signal
#include <stdio.h>    // for standard I/O
#include <stdlib.h>   // for exit
#include <sys/wait.h> // avoid implicit declaration 'waitpid' warning.
#include <unistd.h>   // for *nix APIs .. sleep, read, write

pid_t pid1, pid2;
int msg1, msg2;

void parent_sig_handler(int sig) {
  kill(pid1, SIGUSR1);
  kill(pid2, SIGUSR2);
  waitpid(pid1, 0, 0); // wait child1 to terminate...
  waitpid(pid2, 0, 0); // wait child2 to terminate...
  fprintf(stdout, "%s\n", "Parent Process is Killed!");
  exit(0);
}

void child1_sig_handler(int sig) {
  if (sig == SIGUSR1) {
    fprintf(stdout, "\nChild Process 1 received %d messages.\n", msg1);
    fprintf(stdout, "%s\n", "Child Process 1 is killed by Parent!");
    exit(0);
  }
}

void child2_sig_handler(int sig) {
  if (sig == SIGUSR2) {
    fprintf(stdout, "\nChild Proccess 2 received %d messages.\n", msg2);
    fprintf(stdout, "%s\n", "Child Process 2 is killed by Parent!");
    exit(0);
  }
}

int main(int argc, char const *argv[]) {
  int fd[2]; // pipe file descriptor.

  signal(SIGINT, parent_sig_handler);

  if (pipe(fd) < 0) {
    fprintf(stderr, "%s\n", "pipe error!");
    exit(1);
  }

  if ((pid1 = fork()) < 0) {
    fprintf(stderr, "%s\n", "fork error with child1");
    exit(1);
  } else if (pid1 == 0) {
    // child1...
    signal(SIGUSR1, child1_sig_handler);
    signal(SIGINT, child1_sig_handler);

    close(fd[1]); // close the write end.
    char buf[32];
    for (;;) {
      int n = read(fd[0], buf, 32);
      write(STDOUT_FILENO, buf, n);
      msg1++;
    }
  }

  if ((pid2 = fork()) < 0) {
    fprintf(stderr, "%s\n", "fork error with child2");
    exit(2);
  } else if (pid2 == 0) {
    // child2...
    signal(SIGUSR2, child2_sig_handler);
    signal(SIGINT, child2_sig_handler);

    close(fd[1]); // close the write end.
    char buf[32];

    for (;;) {
      int n = read(fd[0], buf, 32);
      write(STDOUT_FILENO, buf, n);
      msg2++;
    }
  }

  close(fd[0]); // parent process use only the read end.

  char buf[32];
  int x = 1;
  for (;;) {
    int n = snprintf(buf, 32, "I send you %d times.\n", x);
    write(fd[1], buf, n);
    sleep(1);
    ++x;
  }
  return 0;
}
