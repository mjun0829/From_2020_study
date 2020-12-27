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

void Block::access() { is_accessible = 1; }

void Block::deny() { is_accessible = 0; }

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
  }
}

WhiteUser whiteuser;
BlackUser blackuser;

void display() {
  cout << "────────────────────────" << endl;
  cout << "● : " << whiteuser.getWhiteBlocks() << " ────────── ○ : " << blackuser.getBlackBlocks() << endl<<endl;
  for (int i=SIZE-1;i>=0;i--) {
    cout << i << " ";
    for(int j=0;j<SIZE;j++){
    if (board[8*i+j].getEmpty()) {
      if (board[8*i+j].getAccess())
        cout << "□  ";
      else
        cout << "   ";
    } else {
      if (board[8*i+j].getWhite())
        cout << "●  ";
      if (board[8*i+j].getBlack())
        cout << "○  ";
    }
    if (j == 7)
      cout << endl;
    }
  }
  cout << "  ";

  for(int i=0;i<8;i++)
    cout << i << "  ";
  
  cout << endl;
  cout << "────────────────────────" << endl<<endl;
}

WhiteUser::WhiteUser() {
  whiteBlocks = 2;
  is_victory = false;
  turn = true;
}

void WhiteUser::victory() {
  cout << ">> 하얀돌 승리!" << endl;
  display();
  exit(-1);
}

BlackUser::BlackUser() {
  blackBlocks = 2;
  is_victory = false;
  turn = false;
}

void BlackUser::victory() {
  cout << ">> 검은돌 승리!" << endl;
  display();
  exit(-1);
}

vector<Block>::iterator it;
int howManyWhiteBlocks() {
  int count = 0;
  for (it = board.begin(); it != board.end(); it++) {
    if ((*it).getWhite())
      count++;
  }
  return count;
}


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
    if (isNextWall(now, jump))
      return false;

    if (board[now + jump].getBlack()) {
      for (int k = now + jump;; k += jump) {
        if (board[k].getWhite())
          return true;
        if (isNextWall(k, jump))
          return false;
      }
    }
    return false;
  }
}

bool checkWhite(int now, int jump) {
  if (now + jump >= SIZE * SIZE || now + jump < 0)
    return false;
  else {
    if (isNextWall(now, jump))
      return false;

    if (board[now + jump].getWhite()) {
      for (int k = now + jump;; k += jump) {
        if (board[k].getBlack())
          return true;
        if (isNextWall(k, jump))
          return false;
      }
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

void howManyAccessibleBlocks() {
  accessibleBlocks = 0;
  for (int i = 0; i < SIZE * SIZE; i++) {
    if (board[i].getAccess())
      accessibleBlocks++;
  }
}

void insertBlock() {
  while (1) {
    howManyAccessibleBlocks();
    if (whiteuser.getTurn())
      cout << ">> 흰색 돌 차례입니다." << endl;
    else
      cout << ">> 검정색 돌 차례입니다." << endl;
    if (accessibleBlocks == 0) {
      cout << ">> 둘 곳이 없어서 턴을 넘깁니다." << endl;
      break;
    }

    cout << ">> 뒤집을 돌의 좌표 X Y를 입력해주세요." << endl;
    cout << "<< ";
    int _x, _y;
    cin >> _x >> _y;
    selected = 8 * _y + _x;

    if (!board[selected].getAccess()){
      cout << ">> 둘 수 없는 자리입니다. " << endl;
      continue;
    }
    blocks++;
    reverseBlock();
    whiteuser.setWhiteBlocks(howManyWhiteBlocks());
    blackuser.setBlackBlocks(blocks-howManyWhiteBlocks());
    break;
  }
  whiteuser.changeTurn();
  blackuser.changeTurn();
}

void reverseBlock() {
  if (whiteuser.getTurn()) {
    board[selected].setwhite();

    if(checkBlack(selected, 9))
      reverseOneBlock(selected,9);

    if(checkBlack(selected, 8))
      reverseOneBlock(selected,8);

    if(checkBlack(selected, 7))
      reverseOneBlock(selected,7);

    if(checkBlack(selected, 1))
      reverseOneBlock(selected,1);

    if(checkBlack(selected, -1))
      reverseOneBlock(selected,-1);

    if(checkBlack(selected, -7))
      reverseOneBlock(selected,-7);

    if(checkBlack(selected, -8))
      reverseOneBlock(selected,-8);

    if(checkBlack(selected, -9))
      reverseOneBlock(selected,-9);
  }

  if (blackuser.getTurn()) {
    board[selected].setblack();

    if(checkWhite(selected, 9))
      reverseOneBlock(selected,9);

    if(checkWhite(selected, 8))
      reverseOneBlock(selected,8);

    if(checkWhite(selected, 7))
      reverseOneBlock(selected,7);

    if(checkWhite(selected, 1))
      reverseOneBlock(selected,1);

    if(checkWhite(selected, -1))
      reverseOneBlock(selected,-1);

    if(checkWhite(selected, -7))
      reverseOneBlock(selected,-7);

    if(checkWhite(selected, -8))
      reverseOneBlock(selected,-8);

    if(checkWhite(selected, -9))
      reverseOneBlock(selected,-9);
  }
}

void reverseOneBlock(int now,int jump){
  if (whiteuser.getTurn()){
    while(1) {
      if(board[now+jump].getBlack())
        board[now+jump].setwhite();
      else
        break;
      now=now+jump;
    }
  }

  if (blackuser.getTurn()){
    while(1){
      if(board[now+jump].getWhite())
        board[now+jump].setblack();
      else
        break;
      now=now+jump;
    }
  }
}

void victory(){
  if(blocks==SIZE*SIZE){
    if(whiteuser.getWhiteBlocks()>blackuser.getBlackBlocks())
      whiteuser.victory();
    else if(whiteuser.getWhiteBlocks()<blackuser.getBlackBlocks())
      blackuser.victory();
    else {
      cout << ">> 무승부!" << endl;
      display();
      exit(-1);
    }
  }
}