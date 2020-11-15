#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

int main() {
    initscr();
    curs_set(FALSE);
    noecho();
    keypad(stdscr,TRUE);
    int ch;

    srand(time(NULL));

    vector<int> answer;
    vector<int> user;
    vector<int>::iterator it;

    for(int i=0;i<5;i++){
        int temp=rand() % 4;
        answer.push_back(temp);
    }
    for(it=answer.begin();it!=answer.end();it++){
        if(*it==0)
            printw("UP ");
        else if(*it==1)
            printw("DOWN ");
        else if(*it == 2)
            printw("RIGHT ");
        else if(*it==3)
            printw("LEFT ");
    }
    printw("\n");
    nodelay(stdscr,TRUE);

    int clocks=100000;

    while(1){
        ch=getch();
            switch(ch){
                case KEY_UP:
                printw("UP\n");
                user.push_back(0);
                break;
                case KEY_DOWN:
                printw("DOWN\n");
                user.push_back(1);
                break;
                case KEY_RIGHT:
                printw("RIGHT\n");
                user.push_back(2);
                break;
                case KEY_LEFT:
                printw("LEFT\n");
                user.push_back(3);
                break;
            }
        mvprintw(5,0,"%d \n",clocks);
        refresh();
        clocks--;
        if(clocks<0){
            printw("FAILED\n");
            sleep(4);
            endwin();
            exit(-1);
        }
        if(user.size()==5)
        break;
    }
    int count=0;
    for(it=user.begin();it!=user.end();it++){
        if((*it)!=answer[count]){
            printw("\nFAILED\n");
            refresh();
            break;
        }
        count++;
    }
    if(it==user.end()){
        printw("\nSUCCESSED\n");
        refresh();
    }
    sleep(2);

    endwin();
    return 0;
}
