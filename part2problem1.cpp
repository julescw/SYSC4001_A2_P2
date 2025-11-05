#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#define KILLSWITCH 100

/**
 *
 * @file part2problem1.cpp
 * @author Matthew Sanii
 * @author Jules Wong
 *
 */

int main() {

    int context = 0;

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        int p1counter = 0;
        while(1) {
            printf("Child Process (PID: %d), Counter: %d\n", getpid(), p1counter++);
            sleep(1); 
            if(p1counter == KILLSWITCH){
                kill(getpid(), SIGKILL);
            }
        }
    } else if (pid > 0) {
        int p2counter = 0;
        while(1) {
            printf("Parent Process (PID: %d), Counter: %d\n", getpid(), p2counter++);
            sleep(1); 
            if(p2counter == KILLSWITCH){
                kill(getpid(), SIGKILL);
            }
        }
    }
}

