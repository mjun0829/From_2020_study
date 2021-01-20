#ifndef __LJS_NEWOTHELLO_HPP__
#define __LJS_NEWOTHELLO_HPP__

#include <vector>
using std::vector;

namespace Othello {
//한 칸에 상태를 나타내는 열거형
// Block 클래스의 status 에 들어감.
enum Color { WHITE, BLACK, EMPTY };

//한 칸에 대해서 비어있는지, 하얀 돌인지, 검은 돌인지 정보를 포함하고 있는
//클래스.
class Block {
private:
  //현재 돌의 상태를 나타내는 변수
  //여기에는 enum Color의 멤버만이 올 수 있다.
  int Status;
  // Status=EMPTY 일 때, 이 자리에 놓을 수 있는지
  bool IsAccessible;
  //칸의 위치
  int x;
  int y;

public:
  //처음 판을 그릴 때 중앙 4칸을 제외하고 나머지 칸을 만들 때 사용.
  // Status=EMPTY
  Block(int NewX, int NewY);
  //중앙 4칸을 만들 때 사용
  Block(int NewStatus, int NewX, int NewY);
  void SetStatus(int NewStatus) { Status = NewStatus; }

  //칸의 IsAccessible을 바꾸는 함수
  void Access() { IsAccessible = true; }
  void Deny() { IsAccessible = false; }

  // Accessors
  int GetStatus() const { return Status; }
  bool GetAccess() const { return IsAccessible; }
  int GetX() const { return x; }
  int GetY() const { return y; }
};

// 64개의 Block을 가지고있는 벡터를 저장하고 있는 클래스
// 이 벡터를 조작하는 함수들은 이 클래스에 들어있음.
class BoardManager {
private:
  // 64개의 칸을 저장하는 벡터
  vector<vector<Block>> Board;

  // 현재 판에 놓여져있는 돌의 수
  int Blocks;

  // 유저가 선택한 돌의 위치
  int SelectedX;
  int SelectedY;

  // 유저가 놓을 수 있는 돌의 개수
  // 돌의 개수가 0개면 턴을 바로 넘김
  int AccessibleBlocks;

  // 판의 가로/세로 줄 크기
  int Size;

  // 현재 턴의 색
  int TurnColor;

public:
  BoardManager();
  BoardManager(BoardManager &NewBoardManager);
  BoardManager(int NewSize);
  // 게임 시작 전 판을 그리는 함수
  void InitBoard(int NewSize);

  void SetBlocks(int NewBlocks) { Blocks = NewBlocks; }
  void SetSelected(int NewX, int NewY) {
    SelectedX = NewX;
    SelectedY = NewY;
  }
  void SetAccessibleBlocks(int NewBlocks) { AccessibleBlocks = NewBlocks; }
  void SetTurnColor(int Color) { TurnColor = Color; }
  // Accessors
  vector<vector<Block>> GetBoard() { return Board; }
  void SetBoard(vector<vector<Block>> NewBoard) {
    Board.clear();
    Board = NewBoard;
  }
  void SetSize(int NewSize) { Size = NewSize; }
  int GetX() { return SelectedX; }
  int GetY() { return SelectedY; }
  int GetBlocks() { return Blocks; }
  int GetSize() { return Size; }
  int GetAccessibleBlocks() { return AccessibleBlocks; }
  int GetTurnColor() { return TurnColor; }

  // TargetColor Block이 얼마나 있는지 반환하는 함수
  int HowManyColorBlocks(int TargetColor);

  // 판이 어떤 상황인지 보여주는 함수
  void BlockDisplay();

  // 어떤 블록에 color 돌을 놓을 수 있는지 각 블록의 IsAccessible을 바꾸는 함수
  // 바꿔주면서 AccessibleBlocks도 갱신
  void WhichIsAccessible(int Color);

  // Now에서 JumpX, JumpY 방향으로 해당 Color Block이 있는지 판별하는 함수
  // 8방향중에서 하나도 없다면 그 블록의 IsAccessible=false
  bool CheckColor(int NowX, int NowY, int JumpX, int JumpY, int Color);

  // 8방향 중 한 방향이라도 흰/검정색이 있으면 true 반환
  bool CheckColor(int NowX, int NowY, int Color);

  // (NowX, NowY) 위치의 돌을 제외하고 jump방향으로 해당 색이 나올 때까지 찾음.
  // 발견되면 true, 없으면 false
  bool FindMostCloseColor(int NowX, int NowY, int JumpX, int JumpY, int Color);

  // Now+Jump >= Size 인지 확인한다.
  // 넘어가면 Segmentation fault 발생!
  bool IsOverSize(int NowX, int NowY, int JumpX, int JumpY);

  // 해당 위치가 EMPTY인지 확인한다.
  bool CheckEmpty(int NowX, int NowY);

  // (X,Y)에 돌을 두고 해당하는 돌을 뒤집어주고, 아래 8방향을 뒤집는 함수를
  // 호출하는 함수
  void ReverseBlocks(int Color);

  // 해당 방향에 있는 돌들을 전부 뒤집는 함수
  void ReverseBlocks(int X, int Y, int WayX, int WayY, int Color);

  // 현재 판위의 블록의 수가 64개인지 반환하는 함수
  bool IsBlocks64() { return Blocks == 64; }

  // 유저로부터 위치를 입력받는 함수
  void InsertOneBlock();

  // 유저로부터 입력받은 위치가 0에서 Size 사이의 수 인지 확인하는 함수
  bool IsValidBlock(int X, int Y);

  // 턴을 바꾸는 함수
  void ChangeTurn() {
    if (TurnColor == WHITE) {
      TurnColor = BLACK;
    } else {
      TurnColor = WHITE;
    }
  }
};

// 유저 정보를 담고 있는 클래스
class User {
private:
  //판 위에 유저의 색에 해당하는 돌의 개수
  int Blocks;

  //승리 여부
  bool IsVictory;

  //현재 둘 차례인지
  bool Turn;

  //유저의 색
  int Color;

public:
  User();
  //흰 돌의 Turn=true, 검은 돌의 Turn=false
  User(int NewBlocks, bool NewTurn, int NewColor);
  int GetBlocks() const { return Blocks; }
  bool GetVictory() const { return IsVictory; }
  bool GetTurn() const { return Turn; }
  int GetColor() const { return Color; }

  void SetBlocks(int NewBlocks) { Blocks = NewBlocks; }
  void SetVictory(bool NewVictory) { IsVictory = NewVictory; }

  //유저의 턴을 바꾸는 함수
  void ChangeTurn() { Turn = !Turn; }
};

//유저 간 상호작용을 할 수 있게 하는 함수를 가지고 있는 클래스.
//유저 두 명을 클래스의 private 멤버로 가지고 있음.
class UserManager {
private:
  //유저의 color=BLACK
  User BlackUser;

  //유저의 color=WHITE
  User WhiteUser;

public:
  UserManager() {
    BlackUser = User(2, false, BLACK);
    WhiteUser = User(2, true, WHITE);
  }

  User GetBlackUser() { return BlackUser; }
  User GetWhiteUser() { return WhiteUser; }

  // 현재 누구의 턴인지 출력하는 함수
  void DisplayTurn(int TurnColor);

  // Blocks=64 일 때, User의 Blocks가 더 많은 사람이 승리
  // 무승부라면 EMPTY를, 아니라면 승리한 사람의 색을 반환
  int CheckVictory();

  // CheckVictory의 반환값을 통해서 승리 문구를 표시하고 게임 종료.
  void EndGame(BoardManager BoardManager, int Color);

  // 턴을 바꾸는 함수
  // Black -> White, White -> Black
  void ChangeTurn() {
    BlackUser.ChangeTurn();
    WhiteUser.ChangeTurn();
  }

  // 현재 턴의 주인이 놓을 돌이 없어서 턴을 자동으로 넘길 때, 경고문구를
  // 출력하는 함수
  void DisplayWarning();

  // 현재 점수를 출력하는 함수
  void DisplayScore();

  // 유저들의 Blocks를 갱신하는 함수
  void RefreshBlocks(BoardManager BoardManager);
};

// User의 색과 AIUserManager의 AIColor 와 일치하는 유저가 AI
// 이 유저는 따로 InsertOneBlock 함수를 거치지 않고
// 현재 선택된 칸의 x,y 좌표인 SelectedX, SelectedY를 내부 알고리즘을 통하여
// 선택한 뒤 턴을 넘김.
// 그러나 AccessibleBlocks==0 이라면 User와 동일하게 턴을 바로 넘김.
// 칸 마다 우선 순위를 둬서 현재 AccessibleBlocks 들 중 가장 우선 순위가 높은
// 블록을 선택하는 방식으로 진행한다. 우선 순위가 제일 높은 블록이 2개
// 이상이라면 가장 돌을 많이 뒤집는 블록을 선택한다. 이 개수 또한 동일하다면 그
// 중 랜덤으로 선택.
class AIBoardManager : public BoardManager {
private:
  int AIColor;
  vector<vector<int>> PriorityBoard;

public:
  AIBoardManager(int NewAIColor) {
    AIColor = NewAIColor;
    InitPriorityBoard();
  }
  AIBoardManager(int NewAIColor, BoardManager NewBoardManager);
  AIBoardManager(AIBoardManager &NewAIBoardManager);

  // PriorityBoard를 만드는 함수
  void InitPriorityBoard();
  vector<vector<int>> GetPriorityBoard() { return PriorityBoard; }
  void SetPriorityBoard(vector<vector<int>> NewBoard) {
    PriorityBoard = NewBoard;
  }
  int GetAIColor() { return AIColor; }

  // 현재 턴의 색이 AI의 턴의 색인지 확인
  // True라면 Algorithm 호출
  // False라면 InsertOneBlock 호출
  bool IsAITurn() { return GetTurnColor() == AIColor; }

  // Accessible 한 Block들 중 가장 Priority 가 높은 블록을 골라냄.
  // 그 블록의 X,Y를 SelectedX, SelectedY로 갱신해놓음
  void Algorithm();

  // AI가 어디에 뒀는지 말해주는 함수
  void DisplayAISelected(int X, int Y);

  // 어떤 돌이 더 나은 선택인지 알려주는 함수
  // True를 리턴하면 MaxPriority를 바꾸고, 아니면 바꾸지 않음
  bool IsBetterXY(int LeftX, int LeftY, int RightX, int RightY);

  // 가장 낮은 priority를 반환
  int FindMinPriority();

  // 가장 높은 priority를 반환
  int FindMaxPriority();
};
} // namespace Othello

#endif
