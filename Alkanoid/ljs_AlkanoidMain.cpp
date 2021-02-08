#include "ljs_Alkanoid.hpp"
#include <iostream>

using namespace Alkanoid;


int main() {
    Board BoardManager;
    BoardManager.InitSetting();
    //std::cout << BoardManager.GetBall().GetX() << " "
    //          << BoardManager.GetBall().GetY() << std::endl;
    BoardManager.DrawPlayBoard();
    return 0;
}
