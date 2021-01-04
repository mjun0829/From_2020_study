#include "ljs_NewOthello.cpp"

using namespace std;

int main() {
  BoardManager::InitBoard(8);
  BoardManager::BlockDisplay(BoardManager::GetBoard());
  return 0;
}
