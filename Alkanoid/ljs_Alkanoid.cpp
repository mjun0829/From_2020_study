#include "ljs_Alkanoid.hpp"
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using namespace Alkanoid;

#define BAR_LENGTH 10
#define NUMBER_OF_BRICKS 10
#define SECONDS_PER_FRAME 0.001
#define BRICK_WIDTH 3
#define BRICK_HEIGHT 4
#define BLANK " "
#define BRICK_SHAPE "@"
#define BALL_SHAPE "O"
#define BAR_SHAPE "^"
#define BALL_SPEED 0.1

Ball::Ball() {
  X = RIGHTMAX/2;
  Y = DOWNMAX-3;
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

Bar::Bar() : Y(DOWNMAX-2), Length(0), StartLocation(0), ErarsePoint(0) {}

Bar::Bar(int NewLength) {
    Y = DOWNMAX-2;
    Length = NewLength;
    StartLocation = RIGHTMAX/2 - Length / 2;
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
        NowX += BRICK_WIDTH;
        if(NowX>(RIGHTMAX-BRICK_WIDTH)){
            NowX = 1;
            NowY += BRICK_HEIGHT;
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
  Score = 100;
  std::vector<std::vector<int>> NewBlockStatus = InitEmptyBlockStatus();
  SetGameScrPtr(MakeGameScrPtr(NewBlockStatus));
  SetScoreScrPtr(MakeScoreScrPtr());
  SetBall(MakeBall(NewBlockStatus));
  SetBar(MakeBar(NewBlockStatus));
  SetBricks(MakeBricks(NewBlockStatus));
  SetBlockStatus(NewBlockStatus);
}

void Board::CountDown() const {
  int Count = 3;
  DrawPlayBoardEdge();
  while (Count >= 0) {
      time_t Start = clock();
      mvwprintw(GetGameScrPtr(), DOWNMAX / 2, (RIGHTMAX - 2) / 2, "%d", Count);
      wrefresh(GetGameScrPtr());
      while (1) {
          time_t End = clock();
          if ((double)(End - Start) / CLOCKS_PER_SEC >= 1) {
              Start = clock();
              break;
          }
      }
      Count--;
  }
  mvwprintw(GetGameScrPtr(), DOWNMAX / 2, (RIGHTMAX - 2) / 2, " ");
}

std::vector<std::vector<int>> Board::InitEmptyBlockStatus(){
  std::vector<std::vector<int>> Result;
    for (int i = 0; i < DOWNMAX; i++) {
        std::vector<int> TempArray;
        for (int j = 0; j < RIGHTMAX;j++){
            TempArray.push_back(EMPTY);
        }
      Result.push_back(TempArray);
    }
  return Result;
}

WINDOW *Board::MakeGameScrPtr(std::vector<std::vector<int>>& NewBlockStatus) {
  for (int i = 0; i < DOWNMAX;i++){
    for (int j = 0; j < RIGHTMAX;j++){
      if((i==UPMAX)||(i==DOWNMAX-1)||(j==LEFTMAX)||(j==RIGHTMAX-1)){            NewBlockStatus[i][j] = WALL;
      }
    }
  }
  return newwin(DOWNMAX, RIGHTMAX, 3, 3);
}
WINDOW *Board::MakeScoreScrPtr() { return newwin(DOWNMAX, 20, 3, 3+RIGHTMAX); }

Ball Board::MakeBall(std::vector<std::vector<int>> &BlockStatus) {
    Ball NewBall = Ball();
    ChangeBlockStatus(BlockStatus, NewBall.GetY(), NewBall.GetX(), BALL);
    return NewBall;
}

void Board::PlayBoard() {
    CountDown();
    time_t BallSpeedStart = clock();
    while (1) {
        time_t Now = clock();
        InsertKey();
        DrawPlayBoard();
        CheckReflection();

        time_t BallSpeedEnd = clock();
        if ((double)(BallSpeedEnd - BallSpeedStart) / CLOCKS_PER_SEC > BALL_SPEED) {
            MoveBall();
            BallSpeedStart = clock();
        }

        while (1) {
            time_t End = clock();
            if ((double)(End - Now) / CLOCKS_PER_SEC > SECONDS_PER_FRAME) {
                break;
            }
        }

        if (CheckVictory()) {
            DrawPlayBoard();
            DeclareVictory();
            break;
        }
        if (CheckGameEnd()){
            DrawPlayBoard();
            DeclareDefeat();
            break;
        }
  }
  EndGame();
}


void Board::DrawPlayBoard() const {
  DrawBricks();
  DrawBall();
  DrawUserBar();
  DrawPlayBoardEdge();
  DrawScore();
  wrefresh(GetGameScrPtr());
  wrefresh(GetScoreScrPtr());
}

void Board::DrawPlayBoardEdge() const {
  wborder(GetGameScrPtr(), '|', '|', '-', '-', '+', '+', '+', '+');
  wborder(GetScoreScrPtr(), '|', '|', '-', '-', '+', '+', '+', '+');
}

void Board::DrawBall() const {
  mvwprintw(GetGameScrPtr(), GetBall().GetY() - GetBall().GetJumpY(),
            GetBall().GetX() - GetBall().GetJumpX(), BLANK);
  mvwprintw(GetGameScrPtr(), GetBall().GetY(), GetBall().GetX(),BALL_SHAPE);
}

void Board::CheckReflection() {
  int StatusPos1 = GetBlockStatus()[GetBall().GetY() + GetBall().GetJumpY()][GetBall().GetX()];
  int StatusPos2 = GetBlockStatus()[GetBall().GetY() + GetBall().GetJumpY()][GetBall().GetX() + GetBall().GetJumpX()];
  int StatusPos3 = GetBlockStatus()[GetBall().GetY()][GetBall().GetX() + GetBall().GetJumpX()];

  if(StatusPos1==EMPTY && StatusPos3==EMPTY){
    if(StatusPos2!=EMPTY){
        RemoveBricks();
        ReflectionX();
        ReflectionY();
        return;
    } else {
        return;
    }
  } else {
    if(StatusPos1==EMPTY){
        RemoveBricks();
        ReflectionX();
        return;
    } else if (StatusPos3 == EMPTY) {
        RemoveBricks();
        ReflectionY();
        return;
    } else {
        RemoveBricks();
        ReflectionX();
        ReflectionY();
        return;
    }
  }
}

void Board::RemoveBricks() {
  Bricks TempBricks = GetBricks();
  std::vector<Brick> TempBrickBundle = TempBricks.GetBrickBundle();

  int PosX = GetBall().GetX();
  int PosY = GetBall().GetY();
  int JumpX = GetBall().GetJumpX();
  int JumpY = GetBall().GetJumpY();

  int PosNumX[3] = {PosX, PosX + JumpX, PosX + JumpX};
  int PosNumY[3] = {PosY + JumpY, PosY + JumpY, PosY};

  std::vector<int> BrickNumbers;

  for (int i = 0; i < 3;i++){
    if(IsBrick(PosNumY[i],PosNumX[i])){
        BrickNumbers.push_back(GetBrickNum(ConvertX(PosNumX[i]), ConvertY(PosNumY[i])));
    }
  }

  for (int BrickNumber : BrickNumbers) {
      if (BrickNumber < NUMBER_OF_BRICKS) {
          TempBrickBundle[BrickNumber].Remove();
          MakeEmpty(BrickNumber);
          AddScore(BRICK_HEIGHT * BRICK_WIDTH * 100);
      }
  }

  TempBricks.SetBrickBundle(TempBrickBundle);
  SetBricks(TempBricks);
}

void Board::AddScore(int Add) {
  int NewScore = GetScore();
  SetScore(NewScore+Add);
}

bool Board::IsBrick(int Y, int X) { return GetBlockStatus()[Y][X] == BRICK; }


void Board::MakeEmpty(int BrickNumber) {
    int BlocksInLine = (RIGHTMAX - 2) / BRICK_WIDTH;
    int PosX = BrickNumber % BlocksInLine;
    PosX = PosX * BRICK_WIDTH + 1;
    int PosY = BrickNumber / BlocksInLine;
    PosY = PosY * BRICK_HEIGHT + 1;
    for (int i = 0; i < BRICK_HEIGHT; i++) {
        for (int j = 0; j < BRICK_WIDTH; j++) {
            BlockStatus[PosY + i][PosX + j] = EMPTY;
        }
    }
}

int Board::ConvertX(int X) { return (X - 1) / BRICK_WIDTH; }
int Board::ConvertY(int Y) { return (Y - 1) / BRICK_HEIGHT; }

int Board::GetBrickNum(int ConvertedX, int ConvertedY){
    return ((RIGHTMAX - 2) / BRICK_WIDTH) * ConvertedY + ConvertedX;
}

void Board::MoveBall() {
  std::vector<std::vector<int>> TempBlockStatus = GetBlockStatus();
  ChangeBlockStatus(TempBlockStatus, GetBall().GetY(), GetBall().GetX(),EMPTY);
  SourceBall.Move();
  AddScore(-1);
  ChangeBlockStatus(TempBlockStatus, GetBall().GetY(), GetBall().GetX(), BALL);
  SetBlockStatus(TempBlockStatus);
}

Bar Board::MakeBar(std::vector<std::vector<int>> &BlockStatus) {
  Bar NewBar = Bar(BAR_LENGTH);
  for (int i = 0; i < BAR_LENGTH;i++){
    ChangeBlockStatus(BlockStatus, NewBar.GetY(), NewBar.GetStartLocation() + i, BAR);
    }
  return NewBar;
}

void Board::DrawUserBar() const {
  mvwprintw(GetGameScrPtr(), GetBar().GetY(), GetBar().GetErasePoint(), " ");
  for (int i = 0; i < GetBar().GetLength(); i++) {
    mvwprintw(GetGameScrPtr(), GetBar().GetY(), i + GetBar().GetStartLocation(),
              BAR_SHAPE);
  }
}

Bricks Board::MakeBricks(std::vector<std::vector<int>> &BlockStatus) {
  Bricks NewBricks = Bricks(NUMBER_OF_BRICKS);
  for(Brick OneBrick : NewBricks.GetBrickBundle()){
    for (int j = 0; j < BRICK_HEIGHT;j++){
      for (int k = 0; k < BRICK_WIDTH;k++){
        ChangeBlockStatus(BlockStatus, OneBrick.GetY() + j,OneBrick.GetX() + k, BRICK);
        }
    }
  }
  return NewBricks;
}

void Board::DrawBricks() const {
  std::vector<Brick> TempBrick = GetBricks().GetBrickBundle();
  for (int i = 0; i < NUMBER_OF_BRICKS; i++) {
    if (TempBrick[i].GetEmpty()) {
        DrawBrick(TempBrick[i],BLANK);
    } else {
      DrawBrick(TempBrick[i],BRICK_SHAPE);
    }
  }
}

void Board::DrawBrick(Brick OneBrick,const char *Shape) const {
  for (int i = 0; i < BRICK_HEIGHT;i++){
    for (int j = 0; j < BRICK_WIDTH;j++){
      mvwprintw(GetGameScrPtr(),OneBrick.GetY()+i,OneBrick.GetX()+j,Shape);
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

void Board::MoveBar(int Way) {
  std::vector<std::vector<int>> NewBlockstatus = GetBlockStatus();
  for (int i = 0; i < BAR_LENGTH;i++){
    NewBlockstatus[GetBar().GetY()][GetBar().GetStartLocation() + i] = EMPTY;
  }
  SourceBar.Move(Way);
  for (int i = 0; i < BAR_LENGTH;i++){
    NewBlockstatus[GetBar().GetY()][GetBar().GetStartLocation() + i] = BAR;
  }
  SetBlockStatus(NewBlockstatus);
}

void Board::DrawScore() const {
  mvwprintw(GetScoreScrPtr(), 1, 1, "SCORE");
  mvwprintw(GetScoreScrPtr(), 2, 1, "        ");
  mvwprintw(GetScoreScrPtr(), 2, 1, "%d", GetScore());
}

bool Board::CheckGameEnd() const { return GetBall().GetY() == (DOWNMAX - 2); }

bool Board::CheckVictory() const {
  for(Brick OneBrick : GetBricks().GetBrickBundle()){
    if(!OneBrick.GetEmpty()){
        return false;
    }
  }
  return true;
}

void Board::DeclareVictory() const {
    mvwprintw(GetGameScrPtr(),DOWNMAX/2,(RIGHTMAX-4)/2,"WIN");
    wrefresh(GetGameScrPtr());
}

void Board::DeclareDefeat() const {
    mvwprintw(GetGameScrPtr(),DOWNMAX/2,(RIGHTMAX-4)/2,"LOSE");
    wrefresh(GetGameScrPtr());
}

void Board::EndGame() {
    sleep(2);
    endwin();
}
