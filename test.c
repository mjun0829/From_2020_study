#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Length 8
#define Mines 10

void MakeMine(int (*A)[Length]);
int main(){
	printf("Size : %d * %d, Mines : %d\n", Length, Length, Mines);
	int Mine[Length][Length]={0,};
	MakeMine(Mine);
	for(int y = Length; y >= 0; y--){
		printf(" %d", y);
		for(int x = 0; x < Length; x++){
			if(y==0){
				for(int z = y + 1; z <= Length; z++){
					printf(" %d",z);
				}
				break;
			}
			if(Mine[x][y-1]==0){
				printf(" O");
			}
			if(Mine[x][y-1]==1){
				printf(" X");
			}
		}
		
		printf("\n");
	}	
}
void MakeMine(int (*A)[Length]){
	srand(time(NULL));
	int temp = 0;
	for(int x = 0; x < Mines ;){
		temp=rand()%64;
		if(A[temp/Length][temp%Length]==1){
			printf("A\n");
			continue;
		}
		A[temp/Length][temp%Length]=1;
		x++;
		
	}
}


