#ifndef __LJS_ALKANOID_HPP__
#define __LJS_ALKANOID_HPP__

#include <ncurses.h>

namespace Alkanoid {
// Ball의 변수 jumpx,jumpy에 들어가는 변수
enum DirectionX { RIGHT=1, LEFT=-1 };
enum DirectionY { UP=-1, DOWN=1 };
enum WallSize { LEFTMAX = 0, RIGHTMAX = 70, UPMAX=0, DOWNMAX=35 };

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
  //공은 17,68에서 시작하고, LEFT/UP방향으로 움직인다.
  Ball();
  Ball &operator=(const Ball NewBall);
  int GetX() const { return X; }
  int GetY() const { return Y; }
  int GetJumpX() const { return JumpX; }
  int GetJumpY() const { return JumpY; }
  void SetX(int NewX) { X = NewX; }
  void SetY(int NewY) { Y = NewY; }

  void Move() {
    X += JumpX;
    Y += JumpY;
  }

  // 다음 이동이 x축에 평행한 벽을 넘는지를 반환
  // true라면 ReflectionY 호출을 통해서 JumpY 수정
  bool IsNextWallX();

  // 다음 이동이 y축에 평행한 벽을 넘는지를 반환
  // true라면 ReflectionX 호출을 통해서 JumpX 수정
  bool IsNextWallY();

  // JumpX를 수정하는 함수
  // RIGHT <-> LEFT
  void ReflectionX();

  // JumpY를 수정하는 함수
  // UP <-> DOWN
  void ReflectionY();
};

class Bar{
  private:
    int X;

  public:
    Bar();

    int GetX() const { return X; }
};

// ncurses.h 를 이용해서 스크린에 표시하는 함수들을 모아둔 클래스
// 스크린의 크기는 75*35
class Board {
  private:
    Ball SourceBall;
    WINDOW *Screen;

  public:
    Ball GetBall() const { return SourceBall; }
    WINDOW *GetScrPtr() const { return Screen; }

    // 프로그램 실행시 WINDOW 포인터 및 공을 생성하는 함수를 실행시키는 함수
    void InitSetting();

    // WINDOW 포인터 생성하는 함수
    WINDOW *MakeWinPtr();

    // Mutator
    void SetWinPtr(WINDOW *NewPtr) { Screen = NewPtr; }

    // 공을 생성하는 함수
    Ball MakeBall();

    void SetBall(Ball NewBall) { SourceBall = NewBall; }

    // 현재 공의 위치상태 및 플레이어의 막대 상태 등을 고려해서 게임의 스크린에
    // 한 프레임을 출력하는 함수
    void DrawPlayBoard();
};
} // namespace Alkanoid

#endif
