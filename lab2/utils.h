/*
 * @Author: tekky 
 * @Date: 2020-11-03 18:34:25 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-03 20:57:08
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/sem.h>
#include <pthread.h>

typedef void* (*Handler)(void *);

#define TOTAL 100 // total number of tickets.

// definition for the unimplement type union semun
union semun {
    int               val;   // value for SETALL.
    struct semid_ds * buf;   // buffer for IPC_STAT, IPC_SET.
    unsigned short  * array; // array for SETALL, GEGALL.
    struct seminfo  * __buf; // buffer for IPC_INFO.
};

typedef struct {
    int semid; 
    pthread_t ptid[2];
} Argument;

typedef struct {
    int count;
    char msg[10];
} Message;

enum {
// if print_1 or print_2 can sell now, avoid selling
// on the middle of number++.
    CAN_SELL,  

    P1_TURN,        // print_1's turn to sell.
    P2_TURN,        // print_2's turn to sell.
    P1_OR_P2_DONE   // p_1 or p_2 has sold 1 ticket.
};

void P(int semid, int index);

void V(int semid, int index);

void report_work(void *); // report some job someone has done.

#endif /* __UTILS_H__ */