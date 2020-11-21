#include <iostream>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

void func(int &x, int &y) {
    initscr();
    getmaxyx(stdscr, y, x);

    WINDOW *win = newwin(0, 0, x, y);

    wmove(win, 100, 100);
    refresh();
    sleep(1);
    endwin();
}

void func2() {
    initscr();
    mvprintw(0, 0, "Hello, world!");
    refresh();
    sleep(1);
    endwin();
}

void func3() {
    int row = 10;
    int col = 10;
    initscr();
    noecho();
    curs_set(FALSE);

    keypad(stdscr, TRUE);
    while (1) {
        int input = getch();
        clear();
        switch (input) {
        case KEY_UP:
            mvprintw(--row, col, "A");
            continue;
        case KEY_DOWN:
            mvprintw(++row, col, "A");
            continue;
        case KEY_LEFT:
            mvprintw(row, --col, "A");
            continue;
        case KEY_RIGHT:
            mvprintw(row, ++col, "A");
            continue;
        }
        if (input == 'q')
            break;
    }
    endwin();
}

int main() {
    // int x, y;
    // func(x, y);
    func2();
   // func3();
    return 0;
}
