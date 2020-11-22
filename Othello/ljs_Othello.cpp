#include "ljs_Othello.hpp"
#include <assert.h>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

//오델로 판 사이즈
#define SIZE 8
//현재 둬져 있는 돌의 개수
int blocks=4;
//현재 뒤집을 수 있는 돌의 개수
int accessibleBlocks=4;

Block::Block(int newX,int newY)
{
    is_empty=1;
    is_white=0;
    is_black=0;
    is_accessible=0;
    x=newX;
    y=newY;
}

//<assert.h>
Block::Block(bool white,bool black,int newX,int newY){
    //Confirm white xor black == 1
    assert(white^black>0);
    is_empty=0;
    is_white=white;
    is_black=black;
    is_accessible=0;
    x=newX;
    y=newY;
}
void Block::setwhite(){
    is_empty=0;
    is_white=1;
    is_black=0;
    is_accessible=0;
}

void Block::setblack(){
    is_empty=0;
    is_white=0;
    is_black=1;
    is_accessible=0;
}

void Block::access(){
    is_accessible=1;
}

void Block::deny(){
    is_accessible=0;
}

bool Block::getEmpty(){return is_empty;}

bool Block::getWhite(){return is_white&&!is_empty;}

bool Block::getBlack(){return is_black&&!is_empty;}

//<vector>
vector<Block> board;
void Board(){
    for(int i=0; i<64;i++){
        //첫 블록 4개 중에서 하얀색 만들 때
        if(i==27||i==36)
            board.push_back(Block(1,0,i%SIZE,i/SIZE));
        //첫 블록 4개 중에서 검정색 만들 때
        else if(i==28||i==35)
            board.push_back(Block(0,1,i%SIZE,i/SIZE));
        //나머지 블록은 비어있는 상태
        else
            board.push_back(Block(i%SIZE,i/SIZE));
    }
}

//<iostream>,<vector>
vector<Block>::iterator it;
void display(){
    int count=1;
    for(it=board.begin();it!=board.end();++it){
        if(count%SIZE==1)
            cout << endl;
        if((*it).getEmpty()){
            if((*it).getAccess())
            cout << "_  ";
            else
                cout << "E  ";
        }
        if((*it).getWhite())
            cout << "W  ";
        if((*it).getBlack())
            cout << "B  ";
        count++;
    }
    cout << endl;
}

WhiteUser::WhiteUser(){
    whiteBlocks=2;
    is_victory=0;
    turn=1;
}

//<cstdlib>
void WhiteUser::victory(){
    cout << "하얀돌 승리!" << endl;
    display();
    exit(-1);
}

BlackUser::BlackUser(){
    blackBlocks=2;
    is_victory=0;
    turn=0;
}

//<cstdlib>
void BlackUser::victory(){
    cout << "검은돌 승리!" << endl;
    display();
    exit(-1);
}

//<vector>
int howManyWhiteBlocks(){
    int count=0;
    for(it=board.begin();it!=board.end();it++){
        if((*it).getWhite())
            count++
    }
    return count;
}

WhiteUser whiteuser();
BlackUser blackuser();

void whichIsAccessible(){
    //흰색돌 차례일 때
    if(whiteuser.getTurn()){
        for(it=board.begin();it!=board.end();it++){
            if(!(*it).getEmpty()){
                (*it).deny();
                continue;
            }
            //주변에 돌이 있는지 확인
            else if{
                //1. top/left
                if((*it).getX()>=1 && (*it).getY()<=6){
                    if((*it).getBlack()){

                        break;
                    }
                }
                //2. top/middle
                //3. top/right
                //4. middle/left
                //5. middle/right
                //6. bottom/left
                //7. bottom/middle
                //8. bottom/right
            }

        }
    }
    //검정돌 차례일 때
    else{}
}

void insertBlock(){
    if(whiteuser.getTurn())
        cout << "흰색 돌 차례입니다." << endl;
    else
        cout << "검정색 돌 차례입니다." << endl;
    cout << "뒤집을 돌의 좌표 X Y를 입력해주세요."<<endl;
    int _x, _y;
    cin >> _x >> _y;

}
