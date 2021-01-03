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
  static void InitBoard(int NewSize);

  static void SetBlocks(int NewBlocks) { Blocks = NewBlocks; }
  static void SetSelected(int NewX, int NewY) {
    SelectedX = NewX;
    SelectedY = NewY;
  }
  static void SetAccessibleBlocks(int NewBlocks){
      AccessibleBlocks = NewBlocks;
  }

  // Accessors
  static vector<vector<Block>> GetBoard() { return Board; }
  static int GetX() { return SelectedX; }
  static int GetY() { return SelectedY; }
  static int GetBlocks() { return Blocks; }
  static int GetSize() { return Size; }
  static int GetAccessibleBlocks() { return AccessibleBlocks; }

  //판이 어떤 상황인지 보여주는 함수
  static void BlockDisplay(vector<vector<Block>> Board);

  //어떤 블록에 돌을 놓을 수 있는지 각 블록의 IsAccessible을 바꾸는 함수
  static void WhichIsAccessible(vector<vector<Block>> Board);

  // 얼마나 많은 블록이 Accessible 한지 찾아내는 함수
  static int HowManyAccessibleBlocks(vector<vector<Block>> Board);

  // Now에서 JumpX, JumpY 방향으로 해당 Color Block이 있는지 판별하는 함수
  // 8방향중에서 하나도 없다면 그 블록의 IsAccessible=false
  static bool CheckColor(int NowX, int NowY, int JumpX, int JumpY,int Color);

  // 8방향 중 한 방향이라도 흰/검정색이 있으면 true 반환
  static bool CheckColor(int NowX, int NowY,int Color);

  // (NowX, NowY) 위치의 돌을 제외하고 jump방향으로 해당 색이 나올 때까지 찾음.
  // 발견되면 true, 없으면 false
  static bool FindMostCloseColor(int NowX, int NowY, int JumpX, int JumpY, int Color);

  // Now+Jump >= Size 인지 확인한다.
  // 넘어가면 Segmentation fault 발생!
  static bool IsOverSize(int NowX, int NowY, int JumpX, int JumpY);

  // 해당 위치가 EMPTY인지 확인한다.
  static bool CheckEmpty(int NowX, int NowY);

  // (X,Y)에 돌을 두고 해당하는 돌을 뒤집어주고, 아래 8방향을 뒤집는 함수를 호출하는 함수
  static void ReverseBlocks(int X, int Y);

  // 해당 방향에 있는 돌들을 전부 뒤집는 함수
  static void ReverseBlocks(int X, int Y, int WayX, int WayY,int Color,vector<vector<Block>> Board);
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
  int GetBlocks() const { return Blocks; }
  bool GetVictory() const { return IsVictory; }
  bool GetTurn() const { return Turn; }
  int GetColor() const { return Color; }

  void SetBlocks(int NewBlocks) { Blocks = NewBlocks; }
  void SetVictory(bool NewVictory) { IsVictory = NewVictory; }

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
  static User GetBlackUser() { return BlackUser; }
  static User GetWhiteUser() { return WhiteUser; }

  static void InitUser() {
    BlackUser = User(2, false, BLACK);
    WhiteUser = User(2, true, WHITE);
  }
  // Color Block이 얼마나 많이 있는지 반환하는 함수
  static int HowManyBlocks(int Color);

  //유저에게서 블록 위치 하나를 입력받는 함수
  static void InsertOneBlock();

  // Blocks=64 일 때, User의 Blocks가 더 많은 사람이 승리
  // 무승부라면 EMPTY를, 아니라면 승리한 사람의 색을 반환
  static int CheckVictory();

  // CheckVictory의 반환값을 통해서 승리 문구를 표시하고 게임 종료.
  static void EndGame(int Color);
};

#endif
