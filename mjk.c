#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ALength 10  //NULL값이 포함된 array길이(Mine[][])
#define Mines 10	//지뢰 갯수 
#define MineNum -100	//지뢰 값 +1등의 작업이이루어 지므로 충분히 낮은값
#define Length 8	//NULL값없는 기본 array길이(Choice[][])
#define Edge 1000	//NULL값 대신 사용한것 
int MakeMine(int (*A)[ALength]);
//랜덤한 지뢰 생성 및 지뢰값에 대해 주변 8칸의 숫자지정 

int PlayGame(int (*A)[ALength]);
//프로그램 실행 후 게임플레이 부분 

int FindNum(int (*A)[ALength], int (*B)[Length], int c, int d);
//c, d 좌표값의 주변 8칸에 0이아닌 숫자값이 있는지 확인 및 Choice에 추가 
//0 선택시의 알고리즘에 사용 
int main(){
	printf("Size : %d * %d, Mines : %d\n", Length, Length, Mines);
	int Mine[ALength][ALength]={0,};
	MakeMine(Mine);
//for문에서 y축 맨위부터 x값 0부터 차례로 정답지 만드는 루프 
	for(int y = Length; y >= 0; y--){
		printf(" %d", y);
		for(int x = 1; x <= Length; x++){//answer sheet
			if(y==0){// 마지막줄 숫자표시 
				for(int z = y + 1; z <= Length; z++){
					printf(" %d",z);
				}
				break;
			}
			if(Mine[y][x]==0){ // 지뢰없는 0칸
				printf(" O");
			}
			if(Mine[y][x]<0){ // 지뢰칸 
				printf(" X");
			}
			if(Mine[y][x]>0){ // 주변에 지뢰있는 숫자칸 
				printf(" %d", Mine[y][x]);
			}
			//부등호로되있는 이유는 맨위 변수값설정참조 
		}
		
		printf("\n");
	}
	PlayGame(Mine);	
}
int MakeMine(int (*A)[ALength]){
	srand(time(NULL));//랜덤값 지정 
	int temp = 0;
	int randnum = Length * Length;
	for(int x = 0; x < Mines ;){
		temp=rand()%randnum;
		if(A[(temp/Length)+1][(temp%Length)+1]<0){
			continue;
		}// 랜덤값 지정 및 이미 있는 수일 경우 다시 루프 
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
		// 성공적으로 지뢰 생성시 주변 8칸에 +1씩
	}
	//가장자리의 NULL값 생성 
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
	int input_x = 0;//입력받을 x y 값들
	int input_y = 0;
	int round = (Length*Length)-Mines;
	int history[64]= {0,};// 0선택시 탐색과정에서의 발자취 저장 
	int Choice[Length][Length] = {0, };//유저에게 보여지는 마인판 저장소 
	for(int r = 0; r < round; r++){//선택시 마다 유저에게 보여지는 마인판 
		for(int y = Length; y >= 0; y--){
                	printf(" %d", y);
                	for(int x = 0; x < Length; x++){
                        	if(y==0){// Last line
                                	for(int z = y + 1; z <= Length; z++){
                                        	printf(" %d",z);
                                	}
                                	break;
                        	}                        
                        	if(Choice[y-1][x]==0){//아직안보여진 부분 
					printf(" ?");
				}                    
			    	if(Choice[y-1][x]!=0){// 선택된부분 
					printf(" %d", A[y][x+1]);
				}	
                	}
                	printf("\n");
        	}
		for(;;){
			for(;;){//x y 값 범위 체크 불만족시 무한루프 
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
			// 이미 골랐던 부분을 다시골랐을때 
			if(Choice[input_y-1][input_x-1]>0){
				printf("You already picked, pick another one\n");
				continue;
			}
			break;
		}
		// 지뢰를 골랐을때 
		if(A[input_y][input_x]<0){
			printf("Booooooom! You pick Mine\n");
			break;
		}
		// 숫자칸을 골랐을때 
		if(A[input_y][input_x]>0){
			Choice[input_y-1][input_x-1]++;
		}
		//0 칸을 골랐을때 
		if(A[input_y][input_x]==0){
			Choice[input_y-1][input_x-1]++;
			for(int x = 0; x >=0;){//0 Search 
				//인풋값기준 위쪽칸 확인 및 조건을 만족했을시 인풋값 변경, 히스토리 추가, FindNum실행 
				if((A[input_y+1][input_x]==0 & A[input_y+1][input_x]<=100) & Choice[input_y][input_x-1]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
					FindNum(A, Choice, input_y, input_x);
					input_y++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				//인풋값 기준 우측칸 확인 나머지 위와같음 
				if((A[input_y][input_x+1]==0 & A[input_y][input_x+1]<=100) & Choice[input_y-1][input_x]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
					FindNum(A, Choice, input_y, input_x);
					input_x++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				//인풋값 기준 아래 칸 확인 나머지 위와같음 
				if((A[input_y-1][input_x]==0 & A[input_y-1][input_x]<=100) & Choice[input_y-2][input_x-1]==0){
					history[x]=((input_y-1) * Length + (input_x-1));
                                        FindNum(A, Choice, input_y, input_x);
					input_y--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				}
				// 인풋기준 좌측칸 확인 나머지 위와같음 
				if((A[input_y][input_x-1]==0 & A[input_y][input_x-1]<=100) & Choice[input_y-1][input_x-2]==0){
                                        history[x]=((input_y-1) * Length + (input_x-1));
                                        FindNum(A, Choice, input_y, input_x);
					input_x--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				}
				//인풋기준 4방향모두 탐색 불가할때 지금위치 기준으로 FindNum실행 및 히스토리 기준으로 한단계 이전으로 
				FindNum(A, Choice, input_y, input_x);
				input_y=((history[x-1]/Length)+1);
				input_x=((history[x-1]%Length)+1);
				x--;
			}
		}	
	}
}
//입력값 y x 기준으로 실행됨
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
