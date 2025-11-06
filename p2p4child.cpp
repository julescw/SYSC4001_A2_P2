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

// Wait (P)
void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop wait failed");
        exit(1);
    }
}

// Signal (V)
void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop signal failed");
        exit(1);
    }
}


int main() {
    key_t shmkey = ftok(".", 'R');
    key_t semkey = ftok(".", 'S');
    if (shmkey == -1 || semkey == -1) {
        perror("ftok failed");
        exit(1);
    }
    int shmid = shmget(shmkey, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    int *counter = (int *)shmat(shmid, NULL, 0);
    if (counter == (int *)-1) {
        perror("shmat failed");
        exit(1);
    }
    int semid = semget(semkey, 1, 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }
    int multiple = counter[0];
    while (1) {
        sem_wait(semid); 
        int count = counter[1];
        printf("Child Process (PID: %d), Counter: %d\n", getpid(), count);
        if (count % multiple == 0) {
            printf("%d is a multiple of %d.\n", count, multiple);
        }
        counter[1] = ++count;
        sem_signal(semid);
        sleep(1);
        if (counter[1] > KILLSWITCH) {
            printf("Killing Child Process (PID: %d)\n", getpid());
            if (shmdt(counter) == -1)
                perror("shmdt failed");
            exit(0);
        }
    }
    return 0;
}
