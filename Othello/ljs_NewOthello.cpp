#include "ljs_NewOthello.hpp"
#include <cmath>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using namespace Othello;

Block::Block(int NewX, int NewY) {
  Status = EMPTY;
  IsAccessible = false;
  x = NewX;
  y = NewY;
}

Block::Block(int NewStatus, int NewX, int NewY) {
  Status = NewStatus;
  IsAccessible = false;
  x = NewX;
  y = NewY;
}

BoardManager::BoardManager() {
  InitBoard(1);
  Size = 1;
  Blocks = 4;
  SelectedX = 0;
  SelectedY = 0;
  AccessibleBlocks = 0;
  TurnColor = EMPTY;
}

BoardManager::BoardManager(BoardManager &NewBoardManager) {
  Board = NewBoardManager.Board;
  Blocks = NewBoardManager.Blocks;
  SelectedX = NewBoardManager.SelectedX;
  SelectedY = NewBoardManager.SelectedY;
  AccessibleBlocks = NewBoardManager.AccessibleBlocks;
  Size = NewBoardManager.Size;
  TurnColor = NewBoardManager.TurnColor;
}

BoardManager::BoardManager(int NewSize) {
  Size = NewSize;
  InitBoard(NewSize);
  Blocks = 4;
  SelectedX = 0;
  SelectedY = 0;
  AccessibleBlocks = 4;
  TurnColor = WHITE;
}

void BoardManager::InitBoard(int NewSize) {
  vector<vector<Block>> NewBoard;
  for (int i = 0; i < Size; i++) {
    vector<Block> TempBoard;
    for (int j = 0; j < Size; j++) {
      if (i == 3 && j == 3)
        TempBoard.push_back(Block(WHITE, 3, 3));
      else if (i == 4 && j == 4)
        TempBoard.push_back(Block(WHITE, 4, 4));
      else if (i == 3 && j == 4)
        TempBoard.push_back(Block(BLACK, 3, 4));
      else if (i == 4 && j == 3)
        TempBoard.push_back(Block(BLACK, 4, 3));
      else
        TempBoard.push_back(Block(EMPTY, i, j));
    }
    NewBoard.push_back(TempBoard);
  }
  Board = NewBoard;
}

void BoardManager::BlockDisplay() {
  cout << "Blocks : " << Blocks << endl;
  for (int i = Size - 1; i >= 0; i--) {
    cout << i << "  ";
    for (int j = 0; j < Size; j++) {
      if (Board[i][j].GetStatus() == EMPTY) {
        if (Board[i][j].GetAccess()) {
          cout << "□  ";
        } else {
          cout << "   ";
        }
      } else if (Board[i][j].GetStatus() == WHITE) {
        cout << "●  ";
      } else if (Board[i][j].GetStatus() == BLACK) {
        cout << "○  ";
      } else {
        perror("Status error!");
        exit(-1);
      }

      if (j == Size - 1) {
        cout << endl;
      }
    }
  }
  cout << " ";
  for (int i = 0; i < Size; i++) {
    cout << "  " << i;
  }
  cout << endl;
}

void BoardManager::WhichIsAccessible(int Color) {
  int HowManyAccessibleBlocks = 0;
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      if (Board[i][j].GetStatus() != EMPTY) {
        Board[i][j].Deny();
      } else {
        if (CheckColor(j, i, Color)) {
          Board[i][j].Access();
          HowManyAccessibleBlocks++;
        } else {
          Board[i][j].Deny();
        }
      }
    }
  }
  AccessibleBlocks = HowManyAccessibleBlocks;
}

bool BoardManager::CheckColor(int NowX, int NowY, int Color) {
  int WayX[8] = {1, 0, -1, 1, -1, 1, 0, -1};
  int WayY[8] = {1, 1, 1, 0, 0, -1, -1, -1};
  for (int i = 0; i < 8; i++) {
    if (!CheckColor(NowX, NowY, WayX[i], WayY[i], Color)) {
      continue;
    } else {
      return true;
    }
  }
  return false;
}

bool BoardManager::CheckColor(int NowX, int NowY, int JumpX, int JumpY,
                              int Color) {
  if (IsOverSize(NowX, NowY, JumpX, JumpY)) {
    return false;
  } else if (CheckEmpty(NowX + JumpX, NowY + JumpY)) {
    return false;
  } else if (Board[NowY + JumpY][NowX + JumpX].GetStatus() != Color) {
    return FindMostCloseColor(NowX + JumpX, NowY + JumpY, JumpX, JumpY, Color);
  } else {
    return false;
  }
}

bool BoardManager::IsOverSize(int NowX, int NowY, int JumpX, int JumpY) {
  return NowX + JumpX >= Size || NowX + JumpX < 0 || NowY + JumpY >= Size ||
         NowY + JumpY < 0;
}

bool BoardManager::CheckEmpty(int NowX, int NowY) {
  if (Board[NowY][NowX].GetStatus() == EMPTY) {
    return true;
  }
  return false;
}

bool BoardManager::FindMostCloseColor(int NowX, int NowY, int JumpX, int JumpY,
                                      int Color) {
  int TempX = NowX + JumpX;
  int TempY = NowY + JumpY;
  while (true) {
    if (!IsValidBlock(TempX, TempY)) {
      return false;
    } else if (CheckEmpty(TempX, TempY)) {
      return false;
    } else if (Board[TempY][TempX].GetStatus() != Color) {
      TempX += JumpX;
      TempY += JumpY;
      continue;
    } else {
      return true;
    }
  }
}

void BoardManager::ReverseBlocks(int Color) {
  int WayX[8] = {1, 0, -1, 1, -1, 1, 0, -1};
  int WayY[8] = {1, 1, 1, 0, 0, -1, -1, -1};
  Board[SelectedY][SelectedX].SetStatus(Color);
  for (int i = 0; i < 8; i++) {
    if (CheckColor(SelectedX, SelectedY, WayX[i], WayY[i], Color)) {
      ReverseBlocks(SelectedX, SelectedY, WayX[i], WayY[i], Color);
    }
  }
}

void BoardManager::ReverseBlocks(int X, int Y, int WayX, int WayY, int Color) {
  int CursorX = X + WayX;
  int CursorY = Y + WayY;
  while (true) {
    if (Board[CursorY][CursorX].GetStatus() != Color) {
      Board[CursorY][CursorX].SetStatus(Color);
      CursorX += WayX;
      CursorY += WayY;
    } else {
      break;
    }
  }
}

bool BoardManager::IsValidBlock(int X, int Y) {
  if (X >= 0 && X < Size && Y >= 0 && Y < Size) {
    return true;
  }
  return false;
}

void BoardManager::InsertOneBlock() {
  while (true) {
    cout << ">> 돌의 x,y 좌표를 입력해주세요." << endl;
    int UserX, UserY;
    cin >> UserX >> UserY;
    if ((IsValidBlock(UserX, UserY)) && Board[UserY][UserX].GetAccess()) {
      SetSelected(UserX, UserY);
      Blocks++;
      break;
    } else {
      cout << ">> 유효하지않은 좌표입니다." << endl;
    }
  }
}

User::User() {
  Blocks = 0;
  IsVictory = false;
  Turn = false;
  Color = EMPTY;
}

User::User(int NewBlocks, bool NewTurn, int NewColor) {
  Blocks = NewBlocks;
  IsVictory = false;
  Turn = NewTurn;
  Color = NewColor;
}

int BoardManager::HowManyColorBlocks(int TargerColor) {
  int Count = 0;
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      if (Board[i][j].GetStatus() == TargerColor) {
        Count++;
      }
    }
  }
  return Count;
}

void UserManager::DisplayTurn(int TurnColor) {
  if (TurnColor == BLACK) {
    cout << ">> 검정색 돌 차례입니다." << endl;
  } else {
    cout << ">> 흰색 돌 차례 입니다." << endl;
  }
}

int UserManager::CheckVictory() {
  if (WhiteUser.GetBlocks() == BlackUser.GetBlocks()) {
    return EMPTY;
  } else if (WhiteUser.GetBlocks() > BlackUser.GetBlocks()) {
    WhiteUser.SetVictory(true);
    return WHITE;
  } else {
    BlackUser.SetVictory(true);
    return BLACK;
  }
}

void UserManager::EndGame(BoardManager BoardManager, int Color) {
  if (Color == EMPTY) {
    cout << "무승부!" << endl;
  } else if (Color == WHITE) {
    cout << "흰 돌 승리!" << endl;
  } else {
    cout << "검정돌 승리!" << endl;
  }
  BoardManager.BlockDisplay();
}

void UserManager::DisplayWarning() {
  cout << "둘 곳이 없어서 턴을 넘깁니다." << endl;
}

void UserManager::DisplayScore() {
  cout << "● : " << WhiteUser.GetBlocks() << "  ○ : " << BlackUser.GetBlocks()
       << endl;
}

void UserManager::RefreshBlocks(BoardManager BoardManager) {
  int WhiteBlocks = BoardManager.HowManyColorBlocks(WHITE);
  int BlackBlocks = BoardManager.HowManyColorBlocks(BLACK);
  WhiteUser.SetBlocks(WhiteBlocks);
  BlackUser.SetBlocks(BlackBlocks);
}

AIBoardManager::AIBoardManager(int NewAIColor, BoardManager NewBoardManager)
    : BoardManager(NewBoardManager) {
  AIColor = NewAIColor;
  InitPriorityBoard();
}

AIBoardManager::AIBoardManager(AIBoardManager &NewAIBoardManager) {
  AIColor = NewAIBoardManager.GetAIColor();
  PriorityBoard = NewAIBoardManager.GetPriorityBoard();
  SetBoard(NewAIBoardManager.GetBoard());
  SetSelected(-1, -1);
  SetSize(NewAIBoardManager.GetSize());
  SetBlocks(NewAIBoardManager.GetBlocks());
  SetAccessibleBlocks(NewAIBoardManager.GetAccessibleBlocks());
  SetTurnColor(NewAIBoardManager.GetTurnColor());
}

void AIBoardManager::InitPriorityBoard() {
  vector<vector<int>> Result;
  Result = {{7, 1, 6, 2, 2, 6, 1, 7}, {1, 1, 3, 3, 3, 3, 1, 1},
            {6, 3, 5, 4, 4, 5, 3, 6}, {2, 3, 4, 0, 0, 4, 3, 2},
            {2, 3, 4, 0, 0, 4, 3, 2}, {6, 3, 5, 4, 4, 5, 3, 6},
            {1, 1, 3, 3, 3, 3, 1, 1}, {7, 1, 6, 2, 2, 6, 1, 7}};
  PriorityBoard = Result;
}

void AIBoardManager::Algorithm() {
  vector<vector<double>> RankBoard;
  RankBoard = InitRankBoard();
  int MaxPriorityX = 0;
  int MaxPriorityY = 0;
  FindMaxXY(MaxPriorityX, MaxPriorityY, RankBoard);
  SetSelected(MaxPriorityX, MaxPriorityY);
  DisplayAISelected(MaxPriorityX, MaxPriorityY);
}

vector<vector<double>> AIBoardManager::InitRankBoard() {
  vector<vector<double>> Result;
  vector<vector<Block>> SourceBoard = GetBoard();
  for (int i = 0; i < GetSize(); i++) {
    vector<double> TempBoard;
    for (int j = 0; j < GetSize(); j++) {
      if (!SourceBoard[i][j].GetAccess()) {
        TempBoard.push_back(INF);
      } else {
        TempBoard.push_back(0);
      }
    }
    Result.push_back(TempBoard);
  }
  return Result;
}

void AIBoardManager::PriorityAlgorithm(vector<vector<double>> &RankBoard) {
  vector<int> TempVector;
  for (int i; i < GetSize(); i++) {
    for (int j; j < GetSize(); j++) {
      if (GetBoard()[i][j].GetAccess()) {
        TempVector.push_back(GetPriorityBoard()[i][j]);
      }
    }
  }
  double Average = GetAverage(TempVector);

  for (int i; i < GetSize(); i++) {
    for (int j; j < GetSize(); j++) {
      if (GetBoard()[i][j].GetAccess()) {
        double Value = GetPriorityBoard()[i][j];
        Value -= Average;
        Value /= Average;
        Value *= CosineSquare(GetBlocks() - 4);
        RankBoard[i][j] += Value;
      }
    }
  }
}

double AIBoardManager::GetAverage(vector<int> Vec) {
  double Result = 0;
  for (int Value : Vec) {
    Result += Value;
  }
  return Result / Vec.size();
}

double AIBoardManager::CosineSquare(int Turn) {
  double Value = cos(M_PI * Turn / 60);
  return Value * Value;
}

void AIBoardManager::GreedyAlgorithm(vector<vector<double>> &RankBoard) {}

bool AIBoardManager::IsBetterXY(int LeftX, int LeftY, int RightX, int RightY) {
  AIBoardManager LeftBM(this->GetAIColor());
  AIBoardManager RightBM(this->GetAIColor());
  LeftBM.SetBoard(this->GetBoard());
  RightBM.SetBoard(this->GetBoard());
  LeftBM.SetSelected(LeftX, LeftY);
  RightBM.SetSelected(RightX, RightY);
  LeftBM.ReverseBlocks(this->GetAIColor());
  RightBM.ReverseBlocks(this->GetAIColor());
  LeftBM.ChangeTurn();
  RightBM.ChangeTurn();

  if (this->GetAIColor() == BLACK) {
    LeftBM.WhichIsAccessible(WHITE);
    RightBM.WhichIsAccessible(BLACK);
  } else {
    LeftBM.WhichIsAccessible(BLACK);
    RightBM.WhichIsAccessible(WHITE);
  }

  if (!LeftBM.FindMinPriority() < RightBM.FindMinPriority()) {
    return false;
  } else {
    if (!LeftBM.FindMaxPriority() < RightBM.FindMaxPriority()) {
      return false;
    } else {
      return true;
    }
  }
}

int AIBoardManager::FindMinPriority() {
  vector<vector<Block>> TempBoard = GetBoard();
  int Result = 8;
  for (int i = 0; i < GetSize(); i++) {
    for (int j = 0; j < GetSize(); j++) {
      if (TempBoard[i][j].GetAccess()) {
        if (Result > PriorityBoard[i][j]) {
          Result = PriorityBoard[i][j];
        }
      }
    }
  }
  return Result;
}

int AIBoardManager::FindMaxPriority() {
  vector<vector<Block>> TempBoard = GetBoard();
  int Result = 0;
  for (int i = 0; i < GetSize(); i++) {
    for (int j = 0; j < GetSize(); j++) {
      if (TempBoard[i][j].GetAccess()) {
        if (Result < PriorityBoard[i][j]) {
          Result = PriorityBoard[i][j];
        }
      }
    }
  }
  return Result;
}

void AIBoardManager::DisplayAISelected(int X, int Y) {
  cout << endl;
  cout << "AI 가 " << X << ", " << Y << " 에 두고 턴을 넘깁니다." << endl
       << endl;
}
