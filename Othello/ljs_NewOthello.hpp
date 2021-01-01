#ifndef __LJS_NEWOTHELLO_HPP__
#define __LJS_NEWOTHELLO_HPP__

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
  bool GetStatus() const { return Status; }
  bool GetAccess() const { return IsAccessible; }
  int GetX() const { return x; }
  int GetY() const { return y; }
};

class BoardManager {
private:
  // 64개의 칸을 저장하는 벡터
  static vector<vector<Block>> Board;

  //현재 판에 놓여져있는 돌의 수
  static int Blocks;

  //유저가 선택한 돌의 위치
  static int SelectedX;
  static int SelectedY;

  //유저가 놓을 수 있는 돌의 개수
  //돌의 개수가 0개면 턴을 바로 넘김
  static int AccessibleBlocks;

  //판의 가로/세로 줄 크기
  static int Size;

public:
  //게임 시작 전 판을 그리는 함수
  vector<vector<Block>> InitBoard(int NewSize);
  void SetBlocks(int NewBlocks) { Blocks = NewBlocks; }
  void SetSelected(int NewX, int NewY) {
    SelectedX = NewX;
    SelectedY = NewY;
  }

  // Accessors
  vector<vector<Block>> GetBoard();
  int GetX() const;
  int GetY() const;
  int GetSize() const;
  int GetAccessibleBlocks() const;

  //판이 어떤 상황인지 보여주는 함수
  void BlockDisplay(vector<vector<Block>> Board) const;

  //어떤 블록에 돌을 놓을 수 있는지 각 블록의 IsAccessible을 바꾸는 함수
  void WhichIsAccessible(vector<vector<Block>> Board);

  // Now에서 JumpX, JumpY 방향으로 검정돌 혹은 흰돌이 있는지 판별하는 함수
  // 8방향중에서 하나도 없다면 그 블록의 IsAccessible=false
  bool CheckBlack(int Now, int JumpX, int JumpY);
  bool CheckWhite(int Now, int JumpX, int JumpY);

  // Now에서 JumpX, JumpY 만큼 거리에 있는 돌이 접근할 수 있는지 판별하는 함수
  bool IsNextWall(int Now, int JumpX, int JumpY, vector<vector<Block>> Board);

  //(X,Y)에 돌을 두고 해당하는 돌을 뒤집어주는 함수
  void ReverseBlocks(int X, int Y, vector<vector<Block>> Board);
};

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
  //흰 돌의 Turn=true, 검은 돌의 Turn=false
  User(int NewBlocks, bool NewTurn, int NewColor);
  int GetBlocks() const;
  bool GetVictory() const;
  bool GetTurn() const;
  int GetColor() const;

  void SetBlocks(int NewBlocks);
  void SetVictory(bool NewVictory);

  //턴을 바꾸는 함수
  void ChangeTurn() { Turn = !Turn; }
};

class UserManager {
private:
  //유저의 color=BLACK
  static User BlackUser;

  //유저의 color=WHITE
  static User WhiteUser;

public:
  // Color Block이 얼마나 많이 있는지 반환하는 함수
  int HowManyWhiteBlocks(int Color);

  //유저에게서 블록 위치 하나를 입력받는 함수
  void InsertOneBlock();

  // Blocks=64 일 때, User의 Blocks가 더 많은 사람이 승리
  void CheckVictory();
};

#endif
