#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/**
 *
 * @file part2problem1.cpp
 * @author Matthew Sanii
 *
 */

int main() {

    int context = 0;

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        // This code runs in the child process
        printf("Hello from the child process! My PID is %d, my parent's PID is %d\n", getpid(), getppid());
    } else if (pid > 0) {
        // This code runs in the parent process
        printf("Hello from the parent process! My PID is %d, my child's PID is %d\n", getpid(), pid);
    }
    while(1){
        if(pid==0){
        context +=1;
        printf("%d", context);
    }
    }
}
