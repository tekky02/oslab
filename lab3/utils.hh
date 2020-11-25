/*
 * @Author: tekky 
 * @Date: 2020-11-25 17:00:53 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:11:13
 */

#ifndef __UTILS_HH__
#define __UTILS_HH__

#define DEFAULT_PATH "/shm_ring_buffer"
#define MAX_SIZE 256
#define SEM_KEY "/tmp_sem_key"
#define SEM_ID 127

typedef struct {
    char msg[MAX_SIZE];
} Message;

#endif /* __UTILS_HH__ */