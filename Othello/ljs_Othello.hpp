#ifndef __LJS_OTHELLO_H__
#define __LJS_OTHELLO_H__

//한 칸에 대해서 비어있는지, 하얀 돌인지, 검은 돌인지 정보를 포함하고 있는 클래스.
//is_empty, is_white, is_black 중 에서 하나만 true.
class Block{
    private:
        bool is_empty;
        bool is_accessible; // 여기에 돌을 둘 수 있는가?
        bool is_white;
        bool is_black;
        int x;
        int y;

    public:
        //Default constructor
        //중앙 4칸을 제외하고는 초기에 empty=1
        Block(int newX,int newY);

        //중앙 4칸을 만들 때 쓰는 constructor
        Block(bool white,bool black,int newX,int newY);

        //Mutator
        void setwhite();
        void setblack();
        void access();
        void deny();

        //Accessor
        bool getEmpty();
        bool getWhite();
        bool getBlack();
        bool getAccess(){return is_accessible;};
        int getX(){return x;}
        int getY(){return y;}

};

//64개의 칸을 생성자를 통해서 만들고 이를 vector로 관리.
void Board();

//반복자를 통해서 현재 판 상황을 보여주기
void display();

// 흰 돌을 먼저 둠.

//흰 돌 두는 사람
class WhiteUser{
    private:
    //현재 하얀 돌의 개수
        int whiteBlocks;
    //승리 여부
        bool is_victory;
    //차례 여부
        bool turn;
    public:
    //Default constructor
        WhiteUser();
    //승리시 나올 문구 및 게임 종료
        void victory();
    //Accessor
        int getWhiteBlocks(){return whiteBlocks;}
        bool getTurn(){return turn;}
    //Mutator
        void setWhiteBlocks(int newBlocks){whiteBlocks=newBlocks;}
    //차례 넘기기
        void changeTurn(){turn=turn^1;}
};

//검은 돌 두는 사람
class BlackUser{
    private:
    //현재 검정 돌의 개수
        int blackBlocks;
    //승리 여부
        bool is_victory;
    //차례 여부
        bool turn;
    public:
    //Default constructor
        BlackUser();
    //승리시 나올 문구 및 게임 종료
        void victory();
    //Accessor
        int getBlackBlocks(){return blackBlocks;}
        bool getTurn(){return turn;}
    //Mutator
        void setBlackBlocks(int newBlocks){blackBlocks=newBlocks;}
    //차례 넘기기
        void changeTurn(){turn=turn^1;}
};

//돌의 개수 찾는 함수
//검정돌의 개수는 현재 둔 돌의 개수(blocks)에서 빼는 거로.
int howManyWhiteBlocks();

//어떤 돌들이 둘 수 있는 자리인지 알아내는 함수
void whichIsAccessible();

//둘 돌의 입력을 받고, 돌을 뒤집는 함수
void insertBlock();


#endif