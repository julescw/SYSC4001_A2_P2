#include <stdio.h>
#include <unistd.h>
#define KILLSWITCH -100

int main(){
  int p1counter = 0;
  while(1) {
      printf("Child Process (PID: %d), Counter: %d\n", getpid(), p1counter);
      p1counter--;
      if(p1counter % 3 == 0){
        printf("%d is a multiple of 3.\n", p1counter);
      }
      sleep(1); 
      if(p1counter == KILLSWITCH){
          kill(getpid(), SIGKILL);
      }
  }
}
