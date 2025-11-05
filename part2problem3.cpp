#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

/**
 *
 * @file part2problem2.cpp
 * @author Matthew Sanii
 * @author Jules Wong
 *
 */

int main() {
  
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        execl("./p2p3child", "p2p3child", NULL);
        perror("exec failed");
        exit(1);
      
    } else if (pid > 0) {
        int p2counter = 0;
        while(1) {
            printf("Parent Process (PID: %d), Counter: %d\n", getpid(), p2counter--);
            if(p2counter % 3 == 0){
              printf("%d is a multiple of 3.\n", p2counter);
            }
            sleep(1); 
            if(p2counter == KILLSWITCH){
                kill(getpid(), SIGKILL);
            }
        }
    }
}
