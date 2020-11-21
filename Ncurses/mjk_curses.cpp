#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
using namespace std;
//재승이 기반으로만듬
// 랜덤 화살표값 초기화 및 생성함수
void make_rand(int a[], int b) {
    srand(time(NULL));
    for (int x = 0; x < b; x++) {
        a[x] = 0;
        a[x] = rand() % 4;
    }
}
//유사 메인함수
void func3() {
    srand(time(NULL));
    int ow = 10;
    int col = 10;
    int max_for = 5; //반복문횟수(랜덤값생성 및 여러곳 )
    int rand_storage[max_for]; //랜덤값저장배열(갯수바꾸면  동적배열쓰면될듯 )
    int max_x = 0; //화면 최대치 (아직쓸모없 )
    int max_y = 0;
    int temp = 0; //화면 와이 축 변수
    int end = 0;  //반복문 실행/종료변수

    make_rand(rand_storage, max_for);
    initscr();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, max_y, max_x);
    keypad(stdscr, TRUE);
    //색깔 세팅 파/빨/초/노
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, 0);
    while (end == 0) {
        for (int x = 0; x < max_for;) {
            // 랜덤값에 따라 화살표 화면에 표시 초기버전이라 사이즈만맞춰둠
            switch (rand_storage[x]) {
            case 0:
                attron(COLOR_PAIR(1));
                mvprintw(temp, 0, "  /\\\n //\\\\\n///\\\\\\");
                x++;
                temp = temp + 3;
                continue;
            case 1:
                attron(COLOR_PAIR(2));
                mvprintw(temp, 0, "\\\\\\///\n \\\\//\n  \\/");
                x++;
                temp = temp + 3;
                continue;
            case 2:
                attron(COLOR_PAIR(3));
                mvprintw(temp, 0, " <<===\n<<<===\n <<===");
                x++;
                temp = temp + 3;
                continue;
            case 3:
                attron(COLOR_PAIR(4));
                mvprintw(temp, 0, "===>> \n===>>>\n===>> ");
                x++;
                temp = temp + 3;
                continue;
            }
        }
        refresh();
        temp = 0;
        for (int count = 0;;) {
            int input = getch();

            switch (input) {
            //만약 입력값이 현재 배열값의 순서에 맞으면 공백으로 밀어버림
            case KEY_UP:
                if (rand_storage[count] == 0) {
                    mvprintw(temp, 0, "      \n      \n      ");
                    count++;
                    temp = temp + 3;
                }
                refresh();
                continue;
            case KEY_DOWN:
                if (rand_storage[count] == 1) {
                    mvprintw(temp, 0, "      \n      \n      ");
                    count++;
                    temp = temp + 3;
                }
                refresh();
                continue;
            case KEY_LEFT:
                if (rand_storage[count] == 2) {
                    mvprintw(temp, 0, "      \n      \n      ");
                    count++;
                    temp = temp + 3;
                }
                refresh();
                continue;
            case KEY_RIGHT:
                if (rand_storage[count] == 3) {
                    mvprintw(temp, 0, "      \n      \n      ");
                    count++;
                    temp = temp + 3;
                }
                refresh();
                continue;
            }
            if (input == 'q') {
                end++;
                break;
            }
        }
    }
    endwin();
}

int main() {

    func3();
    return 0;
}
