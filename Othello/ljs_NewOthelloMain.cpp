#include "ljs_NewOthello.hpp"

using namespace Othello;

int main() {
  BoardManager BoardManager(8);
  AIBoardManager AIBoardManager(BLACK, BoardManager);

  UserManager UserManager;
  AIBoardManager.WhichIsAccessible(AIBoardManager.GetTurnColor());

  while (1) {
    UserManager.DisplayScore();
    AIBoardManager.BlockDisplay();
    if (AIBoardManager.IsBlocks64()) {
      int WinColor = UserManager.CheckVictory();
      UserManager.EndGame(AIBoardManager, WinColor);
      break;
    }

    if (UserManager.CheckIsBlocks0(BoardManager.GetTurnColor())){
      if(BoardManager.GetTurnColor()==WHITE){
        UserManager.EndGame(AIBoardManager,BLACK);
        break;
      } else {
        UserManager.EndGame(AIBoardManager,WHITE);
        break;
      }
    }

    UserManager.DisplayTurn(AIBoardManager.GetTurnColor());

    if (AIBoardManager.GetAccessibleBlocks() > 0) {
      if (AIBoardManager.IsAITurn()) {
        AIBoardManager.Algorithm();
      } else {
        AIBoardManager.InsertOneBlock();
      }
      AIBoardManager.ReverseBlocks(AIBoardManager.GetTurnColor());
    } else {
      UserManager.DisplayWarning();
    }
    UserManager.ChangeTurn();
    AIBoardManager.ChangeTurn();
    UserManager.RefreshBlocks(AIBoardManager);
    AIBoardManager.WhichIsAccessible(AIBoardManager.GetTurnColor());
  }

  return 0;
}
