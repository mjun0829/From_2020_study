#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
using namespace std;

int main() {
    initscr();
    //noecho(); // 입력을 보이지 않게함
    curs_set(FALSE); // CURSOR를 보이지 않게함
    double count = 10;
    int input=0;
    while (count > 0) {
        clock_t start = clock();

        nodelay(stdscr,TRUE); // 입력대기를 하지 않게함
        input=getch();
        printw("%.2f", count);
        mvprintw(10,2,"%d",input);
        refresh();
        clear();

        while (1) {
            if ((double)(clock() - start) / (CLOCKS_PER_SEC) >= 0.008)
                break;
        }
        count = count - 0.01;

    }
    nodelay(stdscr,FALSE);

    sleep(2);
    endwin();
    return 0;
}
