#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int a=10;
    pid_t pid = fork();
    int status = 0;
    if (pid == -1) {
        perror("fork() failed.\n");
        exit(-1);
    } else if (pid == 0) {
        printf("I'm child, %d\n", getpid());
        printf("child a : %d\n",a);
    } else {
        printf("I'm parent, %d\n", getpid());
        //wait(&status);
        printf("parent a: %d\n", a);
    }

    return 0;
}
