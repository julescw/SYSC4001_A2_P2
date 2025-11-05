#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

/**
 *
 * @file part2problem3.cpp
 * @author Matthew Sanii
 * @author Jules Wong
 *
 */

int main() {
  
    pid_t pid;
    pid = fork();
    
    if (pid == 0) {
        execlp("./p2p3child", "./p2p3child", (char *)NULL);
        perror("exec failed");
    } else if (pid > 0) {
        int p1counter = 0;
        while(1) {
            printf("Parent Process (PID: %d), Counter: %d\n", getpid(), p1counter);
            if(p1counter % 3 == 0){
              printf("%d is a multiple of 3.\n", p1counter);
            }
            p1counter++;
            wait(NULL);
            printf("Killing Process 1.\n");
            kill(getpid(), SIGKILL);
        }
    }
}
