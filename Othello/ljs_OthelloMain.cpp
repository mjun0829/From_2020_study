#include "ljs_Othello.cpp"

using namespace std;

int main() {
  Board();
  whichIsAccessible();
  display();
  
  while(1){
    insertBlock();
    whichIsAccessible();
    display();
    victory();
  }

  return 0;
}
