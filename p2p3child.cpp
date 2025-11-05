#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define KILLSWITCH -500

int main(){
  int p1counter = 0;
  while(1) {
      printf("Child Process (PID: %d), Counter: %d\n", getpid(), p1counter);
      if(p1counter % 3 == 0){
        printf("%d is a multiple of 3.\n", p1counter);
      }
      sleep(1); 
      if(p1counter < KILLSWITCH){
          kill(getpid(), SIGKILL);
      }
      p1counter--;
  }
}
