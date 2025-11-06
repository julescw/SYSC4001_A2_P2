#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
#define KILLSWITCH 500
#define SHM_SIZE (2 * sizeof(int))

/**
 *
 * @file part2problem5.cpp
 * @author Matthew Sanii
 * @author Jules Wong
 *
 */

   // Semaphore signal (P)
void sem_wait(int semid) {
        struct sembuf sb = {0, -1, 0};
        if (semop(semid, &sb, 1) == -1) {
            perror("semop wait failed");
            exit(1);
        }
    }
    
    // Semaphore signal (V)
void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop signal failed");
        exit(1);
    }
}

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

  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  };
  shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget failed");
    exit(1);
  }

  int semid = semget(key, 1, IPC_CREAT | 0666);
  if (semid == -1) {
    perror("semget failed");
    exit(1);
  }

  union semun sem_union;
  sem_union.val = 1;
  if (semctl(semid, 0, SETVAL, sem_union) == -1) {
    perror("semctl SETVAL failed");
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
            sem_wait(semid);
            int count = counter[1];
            printf("Parent Process (PID: %d), Counter: %d\n", getpid(), count);
            if(count % multiple == 0){
              printf("%d is a multiple of %d.\n", count, multiple);
            }
            count++;
            counter[1] = count;
            sem_signal(semid);
            sleep(1);
            if (counter[1] > KILLSWITCH) {
                printf("Killing Process 1.\n");
                if (shmdt(counter) == -1)
                    perror("shmdt failed");
                shmctl(shmid, IPC_RMID, NULL);
                semctl(semid, 0, IPC_RMID);
                kill(getpid(), SIGKILL);
            }
        }
    }
}
