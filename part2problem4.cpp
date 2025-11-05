#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
#define KILLSWITCH 500
#define SHM_SIZE (2 * sizeof(int))

/**
 *
 * @file part2problem4.cpp
 * @author Matthew Sanii
 * @author Jules Wong
 *
 */

int main() {
  key_t key;
  pid_t pid;    
  int shmid;
  int *counter;
  int multiple = 3;
  key = ftok(".", 'R');
  if (key == -1) {
    perror("ftok");
    exit(1);
  }
  shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget failed");
    exit(1);
  }
  
    counter = (int *)shmat(shmid, NULL, 0);
    if (counter == (int *) -1) {
        perror("shmat failed");
        exit(1);
    }

    pid = fork();
    counter[0] = multiple;
    counter[1] = 0;

    if (pid == 0) {
      execlp("./p2p4child", "./p2p4child", (char *)NULL);
      perror("exec failed");
    } else if (pid > 0) {
        while(1) {
            int count = counter[1];
            printf("Parent Process (PID: %d), Counter: %d\n", getpid(), count);
            if(count % mult == 0){
              printf("%d is a multiple of %d.\n", count, mult);
            }
            count++;
            counter[1] = count;
            sleep(1);
            if(counter[1] > 500){
              printf("Killing Process 1.\n");
              kill(getpid(), SIGKILL);
              if (shmdt(counter) == -1) {
                perror("shmdt failed");
                exit(1);
            }}
        }
    }
}
