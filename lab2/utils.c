/*
 * @Author: tekky 
 * @Date: 2020-11-03 18:41:25 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-03 21:14:26
 */

#include <stdio.h>
#include "utils.h"

void P(int semid, int index) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = index;
    buf.sem_op = -1;
    semop(semid, &buf, 1);
}

void V(int semid, int index) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = index;
    buf.sem_op = 1;
    semop(semid, &buf, 1);
}

// report work one has done...
void report_work(void * arg) {
    Message msg = *((Message *)arg);
    fprintf(stdout, "%s I have sold %d tickets.\n", msg.msg, msg.count);
}