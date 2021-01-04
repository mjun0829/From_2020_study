#include "ljs_NewOthello.hpp"
#include <iostream>
#include <vector>

using namespace std;

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

void BoardManager::InitBoard(int NewSize) {
  vector<vector<Block>> NewBoard;
  Size = NewSize;
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      if (i == 3 && j == 3)
        NewBoard[i].push_back(Block(WHITE, 3, 3));
      else if (i == 4 && j == 4)
        NewBoard[i].push_back(Block(WHITE, 4, 4));
      else if (i == 3 && j == 4)
        NewBoard[i].push_back(Block(WHITE, 3, 4));
      else if (i == 4 && j == 3)
        NewBoard[i].push_back(Block(WHITE, 4, 3));
      else
        NewBoard[i].push_back(Block(EMPTY, i, j));
    }
  }
  Board = NewBoard;
}

void BoardManager::BlockDisplay(vector<vector<Block>> Board) {
  for (int i = Size - 1; i >= 0; i--) {
    cout << i << " ";
    for (int j = 0; j < Size; j++) {
      if (Board[i][j].GetStatus() == EMPTY) {
        cout << "   ";
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

  for (int i = 0; i < Size; i++) {
    cout << i << "  ";
  }
  cout << endl;
}

void BoardManager::WhichIsAccessible(vector<vector<Block>> Board) {
  if (UserManager::GetBlackUser().GetTurn()) {
    for (int i = 0; i < Size; i++) {
      for (int j = 0; j < Size; j++) {
        if (Board[i][j].GetStatus() == EMPTY) {
          Board[i][j].Deny();
        } else {
          if (CheckColor(j, i, WHITE)) {
            Board[i][j].Access();
          } else {
            Board[i][j].Deny();
          }
        }
      }
    }
  }

  if (UserManager::GetWhiteUser().GetTurn()) {
    for (int i = 0; i < Size; i++) {
      for (int j = 0; j < Size; j++) {
        if (Board[i][j].GetStatus() == EMPTY) {
          Board[i][j].Deny();
        } else {
          if (CheckColor(j, i, BLACK)) {
            Board[i][j].Access();
          } else {
            Board[i][j].Deny();
          }
        }
      }
    }
  }
}

int BoardManager::HowManyAccessibleBlocks(vector<vector<Block>> Board) {
  int Result = 0;
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      if (Board[i][j].GetAccess()) {
        Result++;
      }
    }
  }
  return Result;
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
    return FindMostCloseColor(NowX, NowY, JumpX, JumpY, Color);
  }
}

bool BoardManager::IsOverSize(int NowX, int NowY, int JumpX, int JumpY) {
  return NowX + JumpX >= Size || NowX + JumpX < 0 || NowY + JumpY >= Size ||
         NowY + JumpY < 0;
}

bool BoardManager::FindMostCloseColor(int NowX, int NowY, int JumpX, int JumpY,
                                      int Color) {
  int TempX = NowX + JumpX;
  int TempY = NowY + JumpY;
  while (true) {
    if (Board[TempY][TempX].GetStatus() == Color) {
      TempX += JumpX;
      TempY += JumpY;
      continue;
    } else if (Board[TempY][TempX].GetStatus() == EMPTY) {
      return false;
    } else {
      return true;
    }
  }
}

void BoardManager::ReverseBlocks(int X, int Y) {
  int WayX[8] = {1, 0, -1, 1, -1, 1, 0, -1};
  int WayY[8] = {1, 1, 1, 0, 0, -1, -1, -1};
  int Color;
  if (UserManager::GetWhiteUser().GetTurn()) {
    Color = WHITE;
  } else {
    Color = BLACK;
  }
  Board[Y][X].SetStatus(Color);
  for (int i = 0; i < 8; i++) {
    if (CheckColor(X, Y, Color)) {
      ReverseBlocks(X, Y, WayX[i], WayY[i], Color, Board);
    }
  }
}

void BoardManager::ReverseBlocks(int X, int Y, int WayX, int WayY, int Color,
                                 vector<vector<Block>> Board) {
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

User::User(int NewBlocks, bool NewTurn, int NewColor) {
  Blocks = NewBlocks;
  IsVictory = false;
  Turn = NewTurn;
  Color = NewColor;
}

int UserManager::HowManyBlocks(int Color) {
  int Count = 0;
  vector<vector<Block>> TempBoard = BoardManager::GetBoard();
  for (int i = 0; i < BoardManager::GetSize(); i++) {
    for (int j = 0; j < BoardManager::GetSize(); j++) {
      if (TempBoard[i][j].GetStatus() == WHITE) {
        Count++;
      }
    }
  }
  return Count;
}

void UserManager::InsertOneBlock() {
  while (true) {
    vector<vector<Block>> Board = BoardManager::GetBoard();
    BoardManager::SetAccessibleBlocks(
        BoardManager::HowManyAccessibleBlocks(Board));
    if (WhiteUser.GetTurn()) {
      cout << ">> 흰색 돌 차례입니다." << endl;
    } else {
      cout << ">> 검정색 돌 차례입니다." << endl;
    }
    if (BoardManager::GetAccessibleBlocks() == 0) {
      cout << ">> 둘 곳이 없어서 턴을 넘깁니다." << endl;
      break;
    }

    cout << ">> 뒤집을 돌의 좌표 X Y를 입력해주세요." << endl;
    cout << "<< ";
    int UserX, UserY;
    cin >> UserX >> UserY;

    if (!Board[UserY][UserX].GetAccess()) {
      cout << ">> 둘 수 없는 자리입니다. " << endl;
      continue;
    }

    int Blocks = BoardManager::GetBlocks();
    BoardManager::SetBlocks(Blocks++);
    BoardManager::ReverseBlocks(UserX, UserY);
    WhiteUser.SetBlocks(HowManyBlocks(WHITE));
    BlackUser.SetBlocks(HowManyBlocks(BLACK));
    WhiteUser.ChangeTurn();
    BlackUser.ChangeTurn();
    break;
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

void UserManager::EndGame(int Color) {
  if (Color == EMPTY) {
    cout << "무승부!" << endl;
  } else if (Color == WHITE) {
    cout << "흰 돌 승리!" << endl;
  } else {
    cout << "검정돌 승리!" << endl;
  }
  BoardManager::BlockDisplay(BoardManager::GetBoard());
}
