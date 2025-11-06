#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define KILLSWITCH 500
#define SHM_SIZE (2 * sizeof(int))

int main(){
  key_t key;
  int shmid;
  int *counter;
  int multiple = 3;

  key = ftok(".", 'R');
  if (key == -1) {
    perror("ftok");
    exit(1);
  }
  shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
      perror("shmget failed");
      exit(1);
  }

  counter = (int *) shmat(shmid, NULL, 0);
  if (counter == (int *) -1) {
    perror("shmat failed");
    exit(1);
  }
  
  int mult = counter[0];
  int count = counter[1];
  while(1) {
    count = counter[1];
      if(count >= 100){
        printf("Child Process (PID: %d), Counter: %d\n", getpid(), count);
        if(count % mult == 0){
          printf("%d is a multiple of %d.\n", count, mult);
        }
        count++;
        counter[1] = count;
        if(count == KILLSWITCH){
          printf("Killing Process 2\n");
          if (shmdt(counter) == -1) {
            perror("shmdt failed");
            exit(1);
          }
          kill(getpid(), SIGKILL);
        }
        sleep(1);
  }
}
}
