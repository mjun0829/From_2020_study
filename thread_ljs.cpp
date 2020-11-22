#include <iostream>
#include <pthread.h>
#include <time.h>
#include <ncurses.h>

void *timer(void *arg);
void *insert(void *arg);

int main() {
    int *status;
    pthread_t tid1;
    pthread_t tid2;

    if(pthread_create(&tid1,NULL,timer,NULL)){
        perror("pthread_create() error!\n");
        exit(-1);
    }
    if(pthread_create(&tid2,NULL,insert,NULL)){
        perror("pthread_create() error!\n");
        exit(-1);
    }
    initscr();
    pthread_join(tid1,(void**)&status);
    pthread_join(tid2,(void**)&status);
    endwin();
    return 0;
}

void *timer(void *arg){
double count = 10;
    while (count > 0) {
        clock_t start = clock();
        printw("%.2f", count);
        refresh();
        clear();
        while (1) {
            if ((double)(clock() - start) / (CLOCKS_PER_SEC) >= 0.01)
                break;
        }
        count = count - 0.01;
    }
}

void *insert(void *arg){
    printw("hi");
    refresh();
}
