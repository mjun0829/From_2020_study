#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>

int ChangeDirection(int NowX, int NowY, int maxX, int maxY, int OldDirection);
bool CheckWall(int NowX, int NowY, int maxX, int maxY);

int main() {
  int wayX[4] = {1, 1, -1, -1};
  int wayY[4] = {1, -1, -1, 1};
  int direction = 0;
  initscr();
  noecho();
  int count = 1;
  int x = count;
  int y = 1;
  int maxX, maxY;
  getmaxyx(stdscr, maxY, maxX);
  while (1) {
    clear();
    time_t now = clock();
    mvprintw(y, x, "A");
    x += wayX[direction];
    y += wayY[direction];
    refresh();
    while (1) {
      time_t end = clock();
      if ((double)(end - now) / CLOCKS_PER_SEC > 0.05) {
        break;
      }
    }
    if (CheckWall(x, y, maxX, maxY)) {
      direction = ChangeDirection(x, y, maxX, maxY, direction);
    }
  }
  endwin();
}

int ChangeDirection(int NowX, int NowY, int maxX, int maxY, int OldDirection) {
  if (OldDirection == 0) {
    if (NowX == maxX) {
      return 3;
    } else {
      return 1;
    }
  } else if (OldDirection == 1) {
    if (NowY == 0) {
      return 0;
    } else {
      return 2;
    }
  } else if (OldDirection == 2) {
    if (NowX == 0) {
      return 1;
    } else {
      return 3;
    }
  } else {
    if (NowY == maxY) {
      return 2;
    } else {
      return 0;
    }
  }
}

bool CheckWall(int NowX, int NowY, int maxX, int maxY) {
  if (NowX == 0 || NowY == 0 || NowX == maxX || NowY == maxY) {
    return true;
  } else {
    return false;
  }
}
