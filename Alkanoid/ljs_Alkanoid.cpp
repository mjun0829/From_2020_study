#include "ljs_Alkanoid.hpp"
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

using namespace Alkanoid;

Ball::Ball() {
  X = 25;
  Y = 33;
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
  return X + JumpX == LEFTMAX + 1 || X + JumpX == RIGHTMAX - 2;
}
bool Ball::IsNextWallY() { return Y + JumpY == UPMAX + 1; }

void Ball::ReflectionX() {
  if (JumpX == RIGHT) {
    JumpX = LEFT;
  } else {
    JumpX = RIGHT;
  }
}

void Ball::ReflectionY() {
  if (JumpY == DOWN) {
    JumpY = UP;
  } else {
    JumpY = DOWN;
  }
}

Bar::Bar() : Length(0), StartLocation(0), ErarsePoint(0) {}

Bar::Bar(int NewLength) {
  Length = NewLength;
  StartLocation = 25 - Length / 2;
}

void Bar::Move(int Way) {
  if (!CheckLocate(Way)) {
    if (Way == RIGHT) {
      ErarsePoint = StartLocation;
    } else {
      ErarsePoint = StartLocation + Length - 1;
    }
    StartLocation += Way;
  }
}

bool Bar::CheckLocate(int Way) {
  if (Way == RIGHT) {
    return StartLocation + Length == RIGHTMAX - 1;
  } else if (Way == LEFT) {
    return StartLocation == LEFTMAX + 1;
  } else {
    return true;
  }
}

void Board::InitSetting() {
  initscr();
  noecho();
  keypad(stdscr, true);
  nodelay(stdscr, true);
  curs_set(false);
  SetGameScrPtr(MakeGameScrPtr());
  SetScoreScrPtr(MakeScoreScrPtr());
  SetBall(MakeBall());
  SetBar(MakeBar());
}

WINDOW *Board::MakeGameScrPtr() { return newwin(35, 50, 3, 3); }
WINDOW *Board::MakeScoreScrPtr() { return newwin(35, 20, 3, 53); }

Ball Board::MakeBall() { return Ball(); }

bool Board::BallBarReflection() {
  if (GetBall().GetJumpY() == DOWN && GetBall().GetY() == DOWNMAX - 3) {
    return GetBall().GetX() >= GetBar().GetStartLocation() - 1 &&
           GetBall().GetX() <=
               GetBar().GetStartLocation() + GetBar().GetLength();
  } else {
    return false;
  }
}

void Board::PlayBoard() {
  time_t BallSpeedStart = clock();
  while (!CheckGameEnd()) {
    time_t Now = clock();
    InsertKey();
    DrawPlayBoard();
    CheckReflection();

    time_t BallSpeedEnd = clock();
    if ((double)(BallSpeedEnd - BallSpeedStart) / CLOCKS_PER_SEC > 0.1) {
      MoveBall();
      BallSpeedStart = clock();
    }

    while (1) {
      time_t End = clock();
      if ((double)(End - Now) / CLOCKS_PER_SEC > 0.0167) {
        break;
      }
    }
  }
  EndGame();
}

void Board::DrawPlayBoard() const {
  DrawPlayBoardEdge();
  DrawBall();
  DrawUserBar();
  wrefresh(GetGameScrPtr());
  wrefresh(GetScoreScrPtr());
}

void Board::DrawPlayBoardEdge() const {
  wborder(GetGameScrPtr(), '|', '|', '-', '-', '+', '+', '+', '+');
  wborder(GetScoreScrPtr(), '|', '|', '-', '-', '+', '+', '+', '+');
}

void Board::DrawBall() const {
  mvwprintw(GetGameScrPtr(), GetBall().GetY() - GetBall().GetJumpY(),
            GetBall().GetX() - GetBall().GetJumpX(), " ");
  mvwprintw(GetGameScrPtr(), GetBall().GetY(), GetBall().GetX(), "O");
}
void Board::CheckReflection() {
  if (GetBall().IsNextWallX()) {
    ReflectionX();
  }
  if (GetBall().IsNextWallY() || BallBarReflection()) {
    ReflectionY();
  }
}

Bar Board::MakeBar() { return Bar(5); }

void Board::DrawUserBar() const {
  mvwprintw(GetGameScrPtr(), GetBar().GetY(), GetBar().GetErasePoint(), " ");
  for (int i = 0; i < GetBar().GetLength(); i++) {
    mvwprintw(GetGameScrPtr(), GetBar().GetY(), i + GetBar().GetStartLocation(),
              "^");
  }
}

void Board::InsertKey() {
  int Insert = getch();
  switch (Insert) {
  case KEY_RIGHT:
    MoveBar(RIGHT);
    break;
  case KEY_LEFT:
    MoveBar(LEFT);
    break;
  }
}

bool Board::CheckGameEnd() { return GetBall().GetY() == (DOWNMAX - 1); }

void Board::EndGame() { endwin(); }
