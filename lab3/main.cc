/*
 * @Author: tekky 
 * @Date: 2020-11-25 14:35:41 
 * @Last Modified by: tekky
 * @Last Modified time: 2020-11-25 17:35:51
 */

#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

#include <signal.h>
#include <sys/wait.h>

#include "utils.hh"
#include "shmringbuffer.hpp"

class Timer {
    std::chrono::time_point<std::chrono::system_clock> start;
public:
    Timer() {
        start = std::chrono::system_clock::now();
    }
    ~Timer() {
        auto end = std::chrono::system_clock::now();
        std::cerr << "time used: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() 
                  << " ms" << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    pid_t read, write;
    const int cap = 20;

    key_t key_sem = ftok(SEM_KEY, SEM_ID);
	int semid = semget(key_sem, 2, IPC_CREAT | 0666);
    unsigned short arr[2] = { cap, 0 };
    union semun sem;
    sem.array = static_cast<unsigned short *>(arr);
    semctl(semid, 0, SETALL, sem);

    // start the timer
    // it will auto print the total time been used before leave main...
    {
        Timer timer;

        if ((write = fork()) < 0) {
            perror("writer fork failed");
            exit(1);
        } else if (write == 0) {
            execl("./writer", std::to_string(semid).c_str(), (char *)0);
        }
        if ((read = fork()) < 0) {
            perror("reader fork failed");
            exit(1);
        } else if (read == 0) {
            execl("./reader", std::to_string(semid).c_str(), (char *)0);
        }
        waitpid(write, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        kill(read, SIGINT);
        semctl(semid, 0, IPC_RMID);
    }
    return 0;
}
