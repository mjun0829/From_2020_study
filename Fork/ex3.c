#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 2;i++){
        int status = 0;
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork() failed.\n");
            exit(-1);
        } else if (pid == 0) {
            printf("I'm child process, %d\n", getpid());
            printf("My parent's pid is %d\n", getppid());
            printf("a?");
            int a=-1;
            scanf("%d", &a);
            printf("a : %d\n", a);
        } else {
            printf("Parent's pid is %d\n", getpid());
            printf("wait for child process...\n");
            wait(&status);
            printf("terminated.\n");
        }
    }
        return 0;
}
