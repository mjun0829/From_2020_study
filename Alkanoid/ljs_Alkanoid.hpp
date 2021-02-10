#ifndef __LJS_ALKANOID_HPP__
#define __LJS_ALKANOID_HPP__

#include <ncurses.h>

namespace Alkanoid {
// Ball의 변수 jumpx,jumpy에 들어가는 변수
enum DirectionX { RIGHT = 1, LEFT = -1 };
enum DirectionY { UP = -1, DOWN = 1 };
enum WallSize { LEFTMAX = 0, RIGHTMAX = 50, UPMAX = 0, DOWNMAX = 35 };

// 공을 위치정보/운동방향을 저장하고 있는 클래스.
// 공을 움직이는 과정을 X=X+JumpX, Y=Y+JumpY로 구현한다.
class Ball {
private:
  // 공의 위치
  int X;
  int Y;

  // 공의 이동방향
  int JumpX;
  int JumpY;

public:
  //공은 25,33에서 시작하고, LEFT/UP방향으로 움직인다.
  Ball();
  Ball &operator=(const Ball NewBall);

  int GetX() const { return X; }
  int GetY() const { return Y; }
  int GetJumpX() const { return JumpX; }
  int GetJumpY() const { return JumpY; }

  void SetX(int NewX) { X = NewX; }
  void SetY(int NewY) { Y = NewY; }
  void SetJumpX(int NewDirection) { JumpX = NewDirection; }
  void SetJumpY(int NewDirection) { JumpY = NewDirection; }

  void Move() {
    X += JumpX;
    Y += JumpY;
  }

  // 다음 이동이 y축에 평행한 벽을 넘는지를 반환
  // true라면 ReflectionX 호출을 통해서 JumpX 수정
  bool IsNextWallX();

  // 다음 이동이 x축에 평행한 벽을 넘는지를 반환
  // true라면 ReflectionY 호출을 통해서 JumpY 수정
  bool IsNextWallY();

  // JumpX를 수정하는 함수
  // RIGHT <-> LEFT
  void ReflectionX();

  // JumpY를 수정하는 함수
  // UP <-> DOWN
  void ReflectionY();
};

// 유저가 움직일 수 있는 바
// 바의 위치정보를 Private변수로 가지고있다.
// 바의 길이만 정할 수 있고, 바의 y축 위치는 고정이다.
// 추후 난이도에 따른 바의 길이를 수정할 것
class Bar {
private:
  int Y = 33;
  int Length;
  // Bar를 그릴 때 시작하는 위치
  int StartLocation;
  // Bar를 그릴 때 화면에 지워야 하는 위치
  int ErarsePoint;

public:
  Bar();
  Bar(int NewLength);

  int GetLength() const { return Length; }
  int GetY() const { return Y; }
  int GetStartLocation() const { return StartLocation; }
  int GetErasePoint() const { return ErarsePoint; }

  // Way에는 RIGHT/LEFT 만 가능
  void Move(int Way);

  // 시작위치가 벽에 닿았는지 여부를 반환하는 함수
  bool CheckLocate(int Way);
};

// ncurses.h 를 이용해서 스크린에 표시하는 함수들을 모아둔 클래스
// 스크린의 크기는 75*35
class Board {
private:
  Ball SourceBall;
  Bar SourceBar;
  WINDOW *GameScreen;
  WINDOW *ScoreScreen;

public:
  Board() { InitSetting(); }

  Ball GetBall() const { return SourceBall; }
  Bar GetBar() const { return SourceBar; }
  WINDOW *GetGameScrPtr() const { return GameScreen; }
  WINDOW *GetScoreScrPtr() const { return ScoreScreen; }

  // 프로그램 실행시 WINDOW 포인터 및 공을 생성하는 함수를 실행시키는 함수
  void InitSetting();

  // WINDOW 포인터 생성하는 함수
  WINDOW *MakeGameScrPtr();
  WINDOW *MakeScoreScrPtr();

  // Mutator
  void SetGameScrPtr(WINDOW *NewPtr) { GameScreen = NewPtr; }
  void SetScoreScrPtr(WINDOW *NewPtr) { ScoreScreen = NewPtr; }

  // 공을 생성하는 함수
  Ball MakeBall();

  void SetBall(Ball NewBall) { SourceBall = NewBall; }
  void SetBar(Bar NewBar) { SourceBar = NewBar; }

  // 현재 공의 위치상태 및 플레이어의 막대 상태 등을 고려해서 게임의 스크린에
  // 한 프레임을 출력하는 함수
  void PlayBoard();

  // 주변 경계선, 공, 막대를 그리는 함수를 호출하는 함수
  void DrawPlayBoard() const;

  // 경계선을 그리는 함수
  void DrawPlayBoardEdge() const;

  // 공을 그리는 함수
  void DrawBall() const;

  // SourceBall의 Reflection여부를 파악하는 함수
  void CheckReflection();

  // SourceBall의 Move함수 호출
  void MoveBall() { SourceBall.Move(); }

  // Ball과 Bar에서 반사되는지 여부를 반환하는 함수
  bool BallBarReflection();

  // SourceBall의 ReflectionX,Y 함수 호출
  void ReflectionX() { SourceBall.ReflectionX(); }
  void ReflectionY() { SourceBall.ReflectionY(); }

  // 유저가 움직이는 바를 만드는 함수
  Bar MakeBar();

  // 유저가 움직이는 바를 그리는 함수
  void DrawUserBar() const;

  // 유저로부터 키를 받는 함수
  void InsertKey();

  // InsertKey를 바탕으로 바의 Move함수 호출하는 함수
  void MoveBar(int Way) { SourceBar.Move(Way); }

  // 공이 벽에 닿았는지 확인하는 함수
  // true라면 PlayBoard()내의 무한루프를 빠져나온다.
  bool CheckGameEnd();

  // 게임 종료시 호출하는 함수
  // 추후 스코어를 보여주는 함수를 출력할 것
  void EndGame();
};
} // namespace Alkanoid

#endif
