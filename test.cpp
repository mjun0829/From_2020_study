#include <iostream>
#include <stdlib.h>
#include <time.h>

#define LENGTH 8
#define MINES 10

int makeMine(int (*A)[LENGTH]);
int main() {
    int Mine[LENGTH][LENGTH]={0};
    makeMine(Mine);

    for(int y = LENGTH; y>=0; y--) {
        std::cout << " " << y;
        for(int x = 0; x < LENGTH; x++) {
            if(y==0) {
                for(int z = y + 1; z <= LENGTH; z++) {
                    std::cout << " " << z;
                }
                break;
            }
            if(Mine[x][y-1]==0) {std::cout << " O";}
            if(Mine[x][y-1]==1) {std::cout << " X";}
        }
        std::cout << std::endl;
    }
}

int makeMine(int (*A)[LENGTH])
{
    srand(time(NULL));
    int temp=0;
    for(int x=0; x< MINES ;x++) {
        while(1) {
            temp=rand()%64;
            if(A[temp/LENGTH][temp%LENGTH]==0) {
                A[temp/LENGTH][temp%LENGTH]=1;
                break;
            }
        }
    }
}
