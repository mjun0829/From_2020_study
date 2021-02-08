#include "ljs_Alkanoid.hpp"
#include <ncurses.h>
#include <unistd.h>
using namespace Alkanoid;

Ball::Ball() {
    X = 17;
    Y = 68;
    JumpX = LEFT;
    JumpY = UP;
}

Ball &Ball::operator=(const Ball NewBall) {
    X = NewBall.GetX();
    Y = NewBall.GetY();
    JumpX = NewBall.GetJumpX();
    JumpY = NewBall.GetJumpY();

    return *this;
}

bool Ball::IsNextWallX() {
    return X == LEFTMAX+1 || X == RIGHTMAX-1; }
bool Ball::IsNextWallY() {
    return Y == UPMAX+1 || Y == DOWNMAX-1; }

void Ball::ReflectionX() {
    if(JumpX==RIGHT){
        JumpX = LEFT;
    } else {
        JumpY = RIGHT;
    }
}

void Ball::ReflectionY() {
    if(JumpY==DOWN){
        JumpY = UP;
    } else {
        JumpY = DOWN;
    }
}

void Board::InitSetting() {
    initscr();
    noecho();
    keypad(stdscr,true);
    nodelay(stdscr, true);
    SetWinPtr(MakeWinPtr());
    SetBall(MakeBall());
}

WINDOW *Board::MakeWinPtr() {
    return newwin(35,70,10,10); }

Ball Board::MakeBall() { return Ball(); }

void Board::DrawPlayBoard() {
    /*
    while(1){
        mvwprintw(GetScrPtr(), 0, 0, "*");
    for (int i = 1; i < RIGHTMAX - 1; i++) {
        mvwprintw(GetScrPtr(), 0, i, "-");
    }
    mvwprintw(GetScrPtr(), 0, RIGHTMAX-1, "*");

    for (int i = 1; i < DOWNMAX - 1;i++){
        mvwprintw(GetScrPtr(), i,0,"|");
        mvwprintw(GetScrPtr(), i, RIGHTMAX - 1, "|");
    }

    mvwprintw(GetScrPtr(), DOWNMAX-1, 0, "*");
    for (int i = 1; i < RIGHTMAX - 1; i++) {
        mvwprintw(GetScrPtr(), DOWNMAX-1, i, "-");
    }
    mvwprintw(GetScrPtr(), DOWNMAX-1, RIGHTMAX-1, "*");

    mvwprintw(GetScrPtr(), GetBall().GetY(), GetBall().GetX(), "&");

    wrefresh(GetScrPtr());
    GetBall().Move();
    sleep(1);
    }
    */
    mvprintw(68, 17, "&");
    wrefresh(stdscr);
    sleep(5);
    endwin();
}
