#include "ljs_Othello.hpp"
#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//오델로 판 사이즈
#define SIZE 8
//현재 둬져 있는 돌의 개수
int blocks = 4;
//현재 뒤집을 수 있는 돌의 개수
int accessibleBlocks = 4;
//플레이어가 선택한 돌의 위치
int selected = 0;

Block::Block(int newX, int newY) {
  is_empty = 1;
  is_white = 0;
  is_black = 0;
  is_accessible = 0;
  x = newX;
  y = newY;
}

Block::Block(bool white, bool black, int newX, int newY) {
  assert(white ^ black == 1);
  is_empty = 0;
  is_white = white;
  is_black = black;
  is_accessible = 0;
  x = newX;
  y = newY;
}
void Block::setwhite() {
  is_empty = 0;
  is_white = 1;
  is_black = 0;
  is_accessible = 0;
}

void Block::setblack() {
  is_empty = 0;
  is_white = 0;
  is_black = 1;
  is_accessible = 0;
}

void Block::access() { is_accessible = true; }

void Block::deny() { is_accessible = false; }

bool Block::getEmpty() { return is_empty; }

bool Block::getWhite() { return is_white && !is_empty; }

bool Block::getBlack() { return is_black && !is_empty; }

vector<Block> board;
void Board() {
  for (int i = 0; i < SIZE * SIZE; i++) {
    //첫 블록 4개 중에서 하얀색 만들 때
    if (i == 27 || i == 36)
      board.push_back(Block(1, 0, i % SIZE, i / SIZE));
    //첫 블록 4개 중에서 검정색 만들 때
    else if (i == 28 || i == 35)
      board.push_back(Block(0, 1, i % SIZE, i / SIZE));
    //나머지 블록은 비어있는 상태
    else
      board.push_back(Block(i % SIZE, i / SIZE));
    if (board[i].getBlack()) {
      cout << i << "번째는 black" << endl;
    }
  }
}

vector<Block>::iterator it;
void display() {
  int count = 0;
  for (it = board.begin(); it != board.end(); ++it) {
    if ((*it).getEmpty()) {
      if ((*it).getAccess())
        cout << "_  ";
      else
        cout << "E  ";
    } else {
      if ((*it).getWhite())
        cout << "W  ";
      if ((*it).getBlack())
        cout << "B  ";
    }
    if (count % SIZE == 7)
      cout << endl;
    count++;
  }
  cout << endl;
}

WhiteUser::WhiteUser() {
  whiteBlocks = 2;
  is_victory = false;
  turn = true;
}

void WhiteUser::victory() {
  cout << "하얀돌 승리!" << endl;
  display();
  exit(-1);
}

BlackUser::BlackUser() {
  blackBlocks = 2;
  is_victory = false;
  turn = false;
}

void BlackUser::victory() {
  cout << "검은돌 승리!" << endl;
  display();
  exit(-1);
}

int howManyWhiteBlocks() {
  int count = 0;
  for (it = board.begin(); it != board.end(); it++) {
    if ((*it).getWhite())
      count++;
  }
  return count;
}

WhiteUser whiteuser;
BlackUser blackuser;

void whichIsAccessible() {
  if (whiteuser.getTurn()) {
    for (int i = 0; i < SIZE * SIZE; i++) {
      if (!board[i].getEmpty()) {
        board[i].deny();
      } else {
        if (checkBlack(i, 9) || checkBlack(i, 8) || checkBlack(i, 7) ||
            checkBlack(i, 1) || checkBlack(i, -1) || checkBlack(i, -7) ||
            checkBlack(i, -8) || checkBlack(i, -9)) {
          board[i].access();
        } else
          board[i].deny();
      }
    }
  }

  if (blackuser.getTurn()) {
    for (int i = 0; i < SIZE * SIZE; i++) {
      if (!board[i].getEmpty()) {
        board[i].deny();
      } else {
        if (checkWhite(i, 9) || checkWhite(i, 8) || checkWhite(i, 7) ||
            checkWhite(i, 1) || checkWhite(i, -1) || checkWhite(i, -7) ||
            checkWhite(i, -8) || checkWhite(i, -9)) {
          board[i].access();
        } else
          board[i].deny();
      }
    }
  }
}

bool checkBlack(int now, int jump) {
  if (now + jump >= SIZE * SIZE || now + jump < 0)
    return false;
  else {
    for (int j = now;; j += jump) {
      if (board[j].getBlack())
        return true;

      if (isNextWall(j, jump))
        break;
    }
    return false;
  }
}

bool checkWhite(int now, int jump) {
  if (now + jump >= SIZE * SIZE || now + jump < 0)
    return false;
  else {
    for (int j = now;; j += jump) {
      if (board[j].getWhite())
        return true;
      if (isNextWall(j, jump))
        break;
    }
    return false;
  }
}

bool isNextWall(int now, int jump) {
  if (now + jump >= SIZE * SIZE || now + jump < 0)
    return true;

  if (board[now + jump].getEmpty())
    return true;

  int temp = now + jump;
  temp = temp % SIZE;
  int _now = now % SIZE;
  if (temp - _now == 7 || temp - _now == -7)
    return true;

  return false;
}

void insertBlock() {
  if (whiteuser.getTurn())
    cout << "흰색 돌 차례입니다." << endl;
  else
    cout << "검정색 돌 차례입니다." << endl;
  cout << "뒤집을 돌의 좌표 X Y를 입력해주세요." << endl;
  int _x, _y;
  cin >> _x >> _y;
  selected = 8 * _y + _x;
  whiteuser.changeTurn();
  blackuser.changeTurn();
}
