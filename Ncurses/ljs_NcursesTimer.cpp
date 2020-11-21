#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
using namespace std;

int main() {
    initscr();
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
    sleep(2);
    endwin();
    return 0;
}
