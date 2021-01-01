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

vector<vector<Block>> BoardManager::InitBoard(int NewSize) {
  vector<vector<Block>> NewBoard;
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      if (i == 3 && j == 3)
        NewBoard[i].push_back(Block(WHITE, 3, 3));
      if (i == 4 && j == 4)
        NewBoard[i].push_back(Block(WHITE, 4, 4));
      if (i == 3 && j == 4)
        NewBoard[i].push_back(Block(WHITE, 3, 4));
      if (i == 4 && j == 3)
        NewBoard[i].push_back(Block(WHITE, 4, 3));
    }
  }
  return NewBoard;
}
