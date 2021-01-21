/*
 * @Author: tekky
 * @Date: 2020-11-03 18:48:49
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-03 21:24:20
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int number = 1; // number of ticket, start at 1.

void *calculate(void *); // calculate thread function.
void *print_1(void *);   // print function of thread1.
void *print_2(void *);   // print function of thread2.

int main(int argc, char const *argv[]) {
  int semid = semget(IPC_PRIVATE, 4, IPC_CREAT | 0666);
  if (semid == -1) {
    fprintf(stderr, "%s\n", "semget error!");
    exit(1);
  }
  unsigned short semarr[4] = {
      1, // P1 can sell ticket 1 at the very beginning.
      1, // number=1 -> odd, P1's turn to sell at the very beginning.
      0, // not P2's turn...
      0  // no sell complete yet...
  };
  union semun sem;
  sem.array = (unsigned short *)semarr;
  semctl(semid, 0, SETALL, sem); // initializing semaphores.

  Handler func[3] = {
      print_1,
      print_2,
      calculate,
  };
  pthread_t ptid[3];

  Argument arg[3];
  arg[0].semid = arg[1].semid = arg[2].semid = semid;

  for (int i = 0; i < 3; ++i) {
    pthread_create(&ptid[i], NULL, func[i], (void *)&arg[i]);
    if (i == 2)
      continue;
    arg[2].ptid[i] = ptid[i];
  }

  // waiting all threads done.
  for (int i = 0; i < 3; ++i) {
    pthread_join(ptid[i], NULL);
  }

  semctl(semid, 0, IPC_RMID); // remove semaphores.
  fprintf(stdout, "\n=======all job done=======\n");
  return 0;
}

void *calculate(void *arg) {
  Argument data = *((Argument *)arg);
  int semid = data.semid;
  pthread_t ptid1 = data.ptid[0];
  pthread_t ptid2 = data.ptid[1];
  for (int i = 0; i < TOTAL - 1; ++i) {
    P(semid, P1_OR_P2_DONE);
    number++;
    // number++ has done, can sell now.
    // avoid selling on the middle of number++.
    V(semid, CAN_SELL);
  }

  P(semid, P1_OR_P2_DONE); // NOTE: waiting the last sell done.

  // all tickets sold out, cancel another two threads.
  pthread_cancel(ptid1);
  pthread_cancel(ptid2);
}

void *print_1(void *arg) {

  // enable asynchronous cancel.
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  Message message;
  message.count = 0;
  snprintf(message.msg, 10, "P1:");

  // func report_work will be called on exit.
  pthread_cleanup_push(report_work, (void *)&message);

  int semid = ((Argument *)arg)->semid;
  for (;;) {
    P(semid, P1_TURN);
    P(semid, CAN_SELL);
    fprintf(stdout, "P1 selling ticket %d.\n", number);
    message.count++;
    V(semid, P1_OR_P2_DONE);
    V(semid, P2_TURN);
  }
  pthread_cleanup_pop(0);
}

void *print_2(void *arg) {

  // enable asynchronous cancel.
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  Message message;
  message.count = 0;
  snprintf(message.msg, 10, "P2:");

  pthread_cleanup_push(report_work, (void *)&message);

  int semid = ((Argument *)arg)->semid;
  for (;;) {
    P(semid, P2_TURN);
    P(semid, CAN_SELL);
    fprintf(stdout, "P2 selling ticket %d.\n", number);
    message.count++;
    V(semid, P1_OR_P2_DONE);
    V(semid, P1_TURN);
  }
  pthread_cleanup_pop(0);
}
