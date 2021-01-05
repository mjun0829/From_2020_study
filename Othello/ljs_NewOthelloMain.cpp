#include "ljs_NewOthello.hpp"

using namespace Othello;

int main() {
  BoardManager BoardManager(8);
  UserManager UserManager;
  BoardManager.WhichIsAccessible(BoardManager.GetTurnColor());
  while (1) {
    UserManager.DisplayScore();
    BoardManager.BlockDisplay();
    if (BoardManager.IsBlocks64()) {
      int WinColor = UserManager.CheckVictory();
      UserManager.EndGame(BoardManager, WinColor);
      break;
    }
    UserManager.DisplayTurn(BoardManager.GetTurnColor());
    if (BoardManager.GetAccessibleBlocks() > 0) {
      BoardManager.InsertOneBlock();
      BoardManager.ReverseBlocks(BoardManager.GetTurnColor());
    } else {
      UserManager.DisplayWarning();
    }
    UserManager.ChangeTurn();
    BoardManager.ChangeTurn();
    UserManager.RefreshBlocks(BoardManager);
    BoardManager.WhichIsAccessible(BoardManager.GetTurnColor());
  }

  return 0;
}
