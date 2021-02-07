#ifndef __LJS_ALKANOID_HPP__
#define __LJS_ALKANOID_HPP__

namespace Alkanoid {
// Ball의 변수 jumpx,jumpy에 들어가는 변수
enum DirectionX { Right, Left };
enum DirectionY { Up, Down };

// 공을 위치정보/운동방향을 저장하고 있는 클래스.
// 공을 움직이는 과정을 X=X+JumpX, Y=Y+JumpY로 구현한다.
class Ball {
private:
  int X;
  int Y;
  int JumpX;
  int JumpY;

public:
  int GetX() { return X; }
  int GetY() { return Y; }
  int GetJumpX() { return JumpX; }
  int GetJumpY() { return JumpY; }
  void SetX(int NewX) { X = NewX; }
  void SetY(int NewY) { Y = NewY; }

  void Move() {
    X += JumpX;
    Y += JumpY;
  }

  void Reflection();
};

// 다음 움직이는 위치가 벽에 닿는지 반환
// false를 반환할 시 Reflection을 호출
class BallManager {
public:
  bool IsNextMoveWall(Ball Ball);
};

class Board {
public:
  void InitBoard();
  void DrawPlayBoard();
};
} // namespace Alkanoid

#endif
