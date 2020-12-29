#ifndef __LJS_NEWOTHELLO_HPP__
#define __LJS_NEWOTHELLO_HPP__

#include <vector>
using namespace std;

enum Color { WHITE, BLACK, EMPTY };

class Block {
  private:
    int Status;
    bool IsAccessible;
    int x;
    int y;

  public:
    Block(int NewX, int NewY);
    Block(int Status, int NewX, int NewY);
    void SetStatus(int NewStatus);
    void Access();
    void Deny();

    bool GetEmpty() const;
    bool GetStatus() const;
    bool GetAccess() const;
    int GetX() const;
    int GetY() const;
};

class BoardManager {
    private:
      vector<vector<Block>> Board;
      static int Blocks;
      static int Selected;
      static int AccessibleBlocks;
      const static int SIZE;

    public:
      vector<vector<Block>> InitBoard;
     void SetBlocks(int);
        void SetSelected(int);
    void BlockDisplay(vector<vector<Block>> Board) const;
            void WhichIsAccessible(vector<vector<Block>> Board);
            bool CheckBlack(int Now, int Jump, vector<vector<Block>> Board);
            bool CheckWhite(int Now, int Jump, vector<vector<Block>> Board);
            bool IsNextWall(int Now, int Jump, vector<vector<Block>> Board);

            void ReverseBlocks();
};

class User {
    private:
      int Blocks;
      bool IsVictory;
      bool Turn;
      int Color;

    public:
      User(int NewBlocks,bool NewTurn,int NewColor);
      int GetBlocks() const;
      bool GetVictory() const;
      bool GetTurn() const;
      int GetColor() const;

      void SetBlocks(int NewBlocks);
      void SetVictory(bool NewVictory);
      void ChangeTurn() { Turn = !Turn; }
};

class UserManager {
    private:
      static User BlackUser;
      static User WhiteUser;

    public:
      int HowManyWhiteBlocks();
      void InsertOneBlock();
      void CheckVictory();
};

#endif
