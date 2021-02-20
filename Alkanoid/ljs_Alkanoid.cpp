#include "ljs_Alkanoid.hpp"
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using namespace Alkanoid;

#define BAR_LENGTH 5
#define NUMBER_OF_BRICKS 100
#define SECONDS_PER_FRAME 0.0167

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

Bar::Bar() : Y(33), Length(0), StartLocation(0), ErarsePoint(0) {}

Bar::Bar(int NewLength) {
    Y = 33;
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

Brick::Brick() : X(0), Y(0), IsEmpty(true) {}

Brick::Brick(int NewX, int NewY) : X(NewX), Y(NewY), IsEmpty(false) {}

Bricks::Bricks(int BrickNumber) {
    std::vector<Brick> Result;
    int NowX = 1;
    int NowY = 1;
    for (int i = 0; i < BrickNumber; i++) {
        Result.push_back(Brick(NowX,NowY));
        NowX += 2;
        if(NowX>=RIGHTMAX-1){
            NowX = 1;
            NowY++;
        }
    }
    BrickBundle = Result;
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
  SetBricks(MakeBricks());
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
    if ((double)(BallSpeedEnd - BallSpeedStart) / CLOCKS_PER_SEC >0.1) {
      MoveBall();
      BallSpeedStart = clock();
    }

    while (1) {
      time_t End = clock();
      if ((double)(End - Now) / CLOCKS_PER_SEC > SECONDS_PER_FRAME) {
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
  DrawBricks();
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

Bar Board::MakeBar() { return Bar(BAR_LENGTH); }

void Board::DrawUserBar() const {
  mvwprintw(GetGameScrPtr(), GetBar().GetY(), GetBar().GetErasePoint(), " ");
  for (int i = 0; i < GetBar().GetLength(); i++) {
    mvwprintw(GetGameScrPtr(), GetBar().GetY(), i + GetBar().GetStartLocation(),
              "^");
  }
}

Bricks Board::MakeBricks() { return Bricks(NUMBER_OF_BRICKS); }

void Board::DrawBricks() const {
    std::vector<Brick> TempBrick = GetBricks().GetBrickBundle();
    for (int i = 0; i < NUMBER_OF_BRICKS; i++) {
        if (TempBrick[i].GetEmpty()) {
            mvwprintw(GetGameScrPtr(), TempBrick[i].GetY(), TempBrick[i].GetX(),
                      " ");
            mvwprintw(GetGameScrPtr(), TempBrick[i].GetY(), TempBrick[i].GetX()+1,
                      " ");
        } else {
            mvwprintw(GetGameScrPtr(), TempBrick[i].GetY(), TempBrick[i].GetX(),
                      "@");
            mvwprintw(GetGameScrPtr(), TempBrick[i].GetY(), TempBrick[i].GetX()+1,
                      "@");
        }
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
