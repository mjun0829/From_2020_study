#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ALength 10
#define Mines 10
#define MineNum -100
#define Length 8
#define Edge 1000
int MakeMine(int (*A)[ALength]);
int PlayGame(int (*A)[ALength]);
int FindNum(int (*A)[ALength], int (*B)[Length], int c, int d);
int main(){
	printf("Size : %d * %d, Mines : %d\n", Length, Length, Mines);
	int Mine[ALength][ALength]={0,};//array with Edge space
	MakeMine(Mine);
	for(int y = Length; y >= 0; y--){
		printf(" %d", y);
		for(int x = 1; x <= Length; x++){//answer sheet
			if(y==0){// Last line
				for(int z = y + 1; z <= Length; z++){
					printf(" %d",z);
				}
				break;
			}
			if(Mine[y][x]==0){
				printf(" O");
			}
			if(Mine[y][x]<0){
				printf(" X");
			}
			if(Mine[y][x]>0){
				printf(" %d", Mine[y][x]);
			}
		}
		
		printf("\n");
	}
	PlayGame(Mine);	
}
int MakeMine(int (*A)[ALength]){
	srand(time(NULL));
	int temp = 0;
	int randnum = Length * Length;
	for(int x = 0; x < Mines ;){
		temp=rand()%randnum;
		if(A[(temp/Length)+1][(temp%Length)+1]<0){
			continue;
		}
		A[(temp/Length)+1][(temp%Length)+1]=MineNum;
		A[(temp/Length)+2][(temp%Length)]++;
		A[(temp/Length)+2][(temp%Length)+1]++;
		A[(temp/Length)+2][(temp%Length)+2]++;
		A[(temp/Length)+1][(temp%Length)]++;
		A[(temp/Length)+1][(temp%Length)+2]++;
		A[(temp/Length)][(temp%Length)]++;
		A[(temp/Length)][(temp%Length)+1]++;
		A[(temp/Length)][(temp%Length)+2]++;	
		x++;
		
	}
	for(int x = 0; x < ALength; x++){
		A[x][0] = Edge;
		A[x][ALength-1] = Edge;
		if( x==0 | x==(ALength-1) ){
			for(int y = 1; y < ALength-1; y++){
				A[x][y] = Edge;
			}
		}
	}
}
int PlayGame(int (*A)[ALength]){
	int input_x = 0;
	int input_y = 0;
	int round = (Length*Length)-Mines;
	int history[64]= {0,};
	int Choice[Length][Length] = {0, };// User's choice
	for(int r = 0; r < round; r++){
		for(int y = Length; y >= 0; y--){
                	printf(" %d", y);
                	for(int x = 0; x < Length; x++){
                        	if(y==0){// Last line
                                	for(int z = y + 1; z <= Length; z++){
                                        	printf(" %d",z);
                                	}
                                	break;
                        	}                        
                        	if(Choice[y-1][x]==0){
					printf(" ?");
				}                    
			    	if(Choice[y-1][x]!=0){
					printf(" %d", A[y][x+1]);
				}	
                	}
                	printf("\n");
        	}
		for(;;){
			for(;;){//x y range check and input
				printf("Enter X coordinate (range : 1~8) :");
				scanf("%d",&input_x);
				if(input_x<1 | input_x>8){
					printf("Out of range, Try again\n");
					continue;
				}
				break;
			}
			for(;;){
               			printf("Enter Y coordinate (range : 1~8) :");
                		scanf("%d",&input_y);
      	        		if(input_y<1 | input_y>8){
                       	 		printf("Out of range, Try again\n");
                       	 		continue;
                		}
                		break;
        		}
			if(Choice[input_y-1][input_x-1]>0){
				printf("You already picked, pick another one\n");
				continue;
			}
			break;
		}
		if(A[input_y][input_x]<0){
			printf("Booooooom! You pick Mine\n");
			break;
		}
		if(A[input_y][input_x]>0){
			Choice[input_y-1][input_x-1]++;
		}
		if(A[input_y][input_x]==0){
			Choice[input_y-1][input_x-1]++;
			for(int x = 0; x >=0;){//0 Search 
				if((A[input_y+1][input_x]==0 & A[input_y+1][input_x]<=100) & Choice[input_y][input_x-1]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
					FindNum(A, Choice, input_y, input_x);
					input_y++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				if((A[input_y][input_x+1]==0 & A[input_y][input_x+1]<=100) & Choice[input_y-1][input_x]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
					FindNum(A, Choice, input_y, input_x);
					input_x++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				if((A[input_y-1][input_x]==0 & A[input_y-1][input_x]<=100) & Choice[input_y-2][input_x-1]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
                                        FindNum(A, Choice, input_y, input_x);
					input_y--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				}
				if((A[input_y][input_x-1]==0 & A[input_y][input_x-1]<=100) & Choice[input_y-1][input_x-2]==0){
                                        history[x]=((input_y-1) * Length + (input_x-1));
                                        FindNum(A, Choice, input_y, input_x);
					input_x--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				}
				FindNum(A, Choice, input_y, input_x);
				input_y=((history[x-1]/Length)+1);
				input_x=((history[x-1]%Length)+1);
				x--;
			}
		}	
	}
}
int FindNum(int (*A)[ALength], int (*B)[Length], int y, int x){
	if(A[y+1][x-1]>0 & A[y+1][x-1]<Mines){B[y][x-2]++;}
	if(A[y+1][x]>0 & A[y+1][x]<Mines){ B[y][x-1]++;}
	if(A[y+1][x+1]>0 & A[y+1][x+1]<Mines){B[y][x]++;}
	if(A[y][x-1]>0 & A[y][x-1]<Mines){B[y-1][x-2]++;}
	if(A[y][x+1]>0 & A[y][x+1]<Mines){B[y-1][x]++;}
	if(A[y-1][x-1]>0 & A[y-1][x-1]<Mines){B[y-2][x-2]++;}
	if(A[y-1][x]>0 & A[y-1][x]<Mines){B[y-2][x-1]++;}
	if(A[y-1][x+1]>0 & A[y-1][x+1]<Mines){B[y-2][x]++;}
}
