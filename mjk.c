#include <stdio.h>	
#include <stdlib.h>	//난수생성용 srand(), rand()
#include <time.h>	//난수생성용 srand용 time()
#include <string.h>	//난이도 검사용 변수검사 strcmp()
#include <ctype.h>	//PlayGame 입력값 검사용으로 isdigit()사용 
#define ALength 10	//NULL값이 포함된 array길이(Mine[][])
#define Length 8        //NULL값없는 기본 array길이(Choice[][])
#define M_ALength 18	//NULL포함  M 사이즈 array 길이 
#define M_Length 16	//M 사이즈 기본 array
#define B_ALength 27	// NULL 포함 B 사이즈 array
#define B_Length 25	// B 사이즈 array
#define Mines 10	//지뢰 갯수 
#define M_Mines 40	//M 지뢰 
#define B_Mines 120	// B 지뢰 
#define MineNum -100	//지뢰 값 +1등의 작업이이루어 지므로 충분히 낮은값
#define Edge 1000	//NULL값 대신 사용한것 
int MakeMine(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int a, int b);
//랜덤한 지뢰 생성 및 지뢰값에 대해 주변 8칸의 숫자지정 

int AnswerSheet(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int a, int b);
//답지 표시 
//
int PlayGame(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int a, int b, int c);
//프로그램 실행 후 게임플레이 부분 

//원래있던 Findnum제거하고 PlayGame에 합침 
//c, d 좌표값의 주변 8칸에 0이아닌 숫자값이 있는지 확인 및 Choice에 추가 
//0 선택시의 알고리즘에 사용 
int main(){
	char D_check[1];	// 난이도 판별용 변수
       	int D_num = 0; 		// 난이도 저장용 
	int D_length = 0;	// 난이도별 LENGTH저장용 
	int D_Mnum = 0;		// 난이도별 마인갯수 저장용 
//난이도 설정 
	printf("Choose the difficulty of Mine sweeper\n");
	printf("S : Size : %d * %d, Mines : %d\n", Length, Length, Mines);
	printf("M : Size : %d * %d, Mines : %d\n", M_Length, M_Length, M_Mines);
	printf("B : Size : %d * %d, Mines : %d\n", B_Length, B_Length, B_Mines);
	printf("Which one?(S, M, B) : ");
	scanf("%c", D_check);
//고른 사이즈 체크 및 이후 사용될 Dnum 지정 char 값이면 불편함(개인적의견 )
	if(strcmp(D_check,"S")==0){
		printf("you pick S\n");
		D_num = 1;
		D_length = Length;
		D_Mnum = Mines;
	}
	if(strcmp(D_check,"M")==0){
		printf("you pick M\n");
		D_num = 2;
		D_length = M_Length;
		D_Mnum = M_Mines;
	}
	if(strcmp(D_check,"B")==0){
		printf("you pick B\n");
		D_num = 3;
		D_length = B_Length;
		D_Mnum = B_Mines;
	}
//사이즈별 지뢰판 생성 및 0 초기화 
	int Mine[ALength][ALength] = {0, };
	int M_Mine[M_ALength][M_ALength] = {0, };
	int B_Mine[B_ALength][B_ALength] = {0, };
	MakeMine(Mine, M_Mine, B_Mine, D_num, D_length);
	AnswerSheet(Mine, M_Mine, B_Mine, D_num, D_length);
	PlayGame(Mine, M_Mine, B_Mine, D_num, D_length, D_Mnum);	
}

int MakeMine(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int D, int L){
	srand(time(NULL));//랜덤값 지정 
	int temp = 0;
	int randnum = 0;//난수범위 
	int randloop = 0;//난수 반복문 횟수 
	int edgenum = 0;//가장자리 생성 반복문 변수
       		
//Dnum값에 따라 랜덤값 지정 및 받을 array 지정  
	if(D==1){
		edgenum = ALength;
		randnum = Length*Length;
		randloop = Mines;
	}
	if(D==2){
		edgenum = M_ALength;
		randnum = M_Length*M_Length;
		randloop = M_Mines;
	}
	if(D==3){
		edgenum = B_ALength;
		randnum = B_Length*B_Length;
		randloop = B_Mines;
	}
	// 3가지 array중에사 사용할거만 포인터로 받음
	int (*randPtr)[edgenum];
	if(D==1){randPtr = A;}
	if(D==2){randPtr = B;}
	if(D==3){randPtr = C;}
	for(int x = 0; x < randloop ;){
		temp=rand()%randnum;
		if(randPtr[(temp/L)+1][(temp%L)+1]<0){
			continue;
		}// 랜덤값 지정 및 이미 있는 수일 경우 다시 루프 
		randPtr[(temp/L)+1][(temp%L)+1]=MineNum;
		randPtr[(temp/L)+2][(temp%L)]++;
		randPtr[(temp/L)+2][(temp%L)+1]++;
		randPtr[(temp/L)+2][(temp%L)+2]++;
		randPtr[(temp/L)+1][(temp%L)]++;
		randPtr[(temp/L)+1][(temp%L)+2]++;
		randPtr[(temp/L)][(temp%L)]++;
		randPtr[(temp/L)][(temp%L)+1]++;
		randPtr[(temp/L)][(temp%L)+2]++;	
		x++;
		// 성공적으로 지뢰 생성시 주변 8칸에 +1씩
	}
	//가장자리의 NULL값 생성 
	for(int x = 0; x < edgenum; x++){
		randPtr[x][0] = Edge;
		randPtr[x][edgenum-1] = Edge;
		if( x==0 | x==(edgenum-1) ){
			for(int y = 1; y < edgenum-1; y++){
				randPtr[x][y] = Edge;
			}
		}
	}
}
// 정답지 생성 루프
int AnswerSheet(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int D, int L){
	//Sheet만들 포인터 할당 
	int (*Mptr)[L+2];
	if(D==1){Mptr = A;}
	if(D==2){Mptr = B;}
	if(D==3){Mptr = C;}
	for(int y = L; y >= 0; y--){
                printf(" %d", y%10);
                for(int x = 1; x <= L; x++){//answer sheet
                        if(y==0){// 마지막줄 숫자표시 
                                for(int z = y + 1; z <= L; z++){
                                        printf(" %d",z%10);
                                }
                                break;
                        }
                        if(Mptr[y][x]==0){ // 지뢰없는 0칸
                                printf(" O");
                        }
                        if(Mptr[y][x]<0){ // 지뢰칸 
                                printf(" X");
                        }
                        if(Mptr[y][x]>0){ // 주변에 지뢰있는 숫자칸 
                                printf(" %d", Mptr[y][x]);
                        }
                        //부등호로되있는 이유는 맨위 변수값설정참조 
                }

                printf("\n");
        }

}
int PlayGame(int (*A)[ALength], int (*B)[M_ALength], int (*C)[B_ALength], int D, int L, int M){
	int input_x = 0;	//입력받을 x y 값들
	int input_y = 0;
	int temp = 0;		//계산용 임시변수 
	int flag_counter = 0;	//승리조건용 플래그변수 
	int round = (L*L);	//하나하나 다누를시에 최대치 
	int history[round];	// 0선택시 탐색과정에서의 발자취 저장 
	int Choice[L][L];	//유저에게 보여지는 마인판 저장소
        int (*Gptr)[(L+2)];	//게임 진행시 사용될 배열을 받는 포인터 
	char flag_check[1];	//플래그 인풋용 

	if(D==1){Gptr = A;}	//사용될 마인판 Dnum으로 정함 
	if(D==2){Gptr = B;}
	if(D==3){Gptr = C;}	
	for(int x = 0; x < L; x++){
		for(int y = 0; y < L; y++){Choice[x][y] = 0;}
	}
	for(int r = 0; r < round; r++){//선택시 마다 유저에게 보여지는 마인판 
		for(int y = L; y >= 0; y--){
                	printf(" %d", y%10);
                	for(int x = 0; x < L; x++){
                        	if(y==0){// Last line
                                	for(int z = y + 1; z <= L; z++){
						printf(" %d",z%10);
					}
                                	break;
                        	}                        
                        	if(Choice[y-1][x] == 0){//아직안보여진 부분 
					printf(" ?");
				}                    
			    	if(Choice[y-1][x] > 0 && Choice[y-1][x] < 100){// 선택된부분 
					printf(" %d", Gptr[y][x+1]);
				}
				if(Choice[y-1][x] >= 100){// 플래그일경우 
					printf(" X");
				}
                	}
                	printf("\n");
        	}
		for(;;){
			for(;;){//x y 값 숫자 체크 및 범위 체크 불만족시 무한루프 
				printf("Enter X coordinate (range : 1 ~ %d) :", L);
				scanf("%d",&input_x);
				if(isdigit((input_x%10)+48)==0){
					printf("This is not number, Try again\n");
					input_x = 0;
					continue;
				}
				if(input_x<1 || input_x>L){
					printf("Out of range, Try again\n");
					input_x = 0;
					continue;
				}
				break;
			}
			for(;;){
               			printf("Enter Y coordinate (range : 1 ~ %d) :", L);
                		scanf("%d",&input_y);
				if(isdigit((input_y%10)+48)==0){
                                        printf("This is not number, Try again\n");
					input_y = 0;
                                        continue;
                                }
      	        		if(input_y<1 || input_y>L){
                       	 		printf("Out of range, Try again\n");
					input_y = 0;
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
		//지금의 인풋에 플래그(지뢰)세울때, 플래그 해제(지뢰가 아닌경우)는 그냥 플래그칸 재선택하면됨 
		printf("open or Make Flag (Press F if you want Flag, other input ignored) : \n");
		scanf(" %c",flag_check);
		if(strcmp(flag_check, "F")==0){
			Choice[input_y-1][input_x-1]+=100;
			flag_counter++;
			continue;
		}
		// 지뢰를 골랐을때 
		if(Gptr[input_y][input_x]<0){
			printf("Booooooom! You pick Mine\n");
			break;
		}
		// 숫자칸을 골랐을때 
		if(Gptr[input_y][input_x]>0){
			if(Choice[input_y-1][input_x-1]>=100){
				Choice[input_y-1][input_x-1]-=100;
			}
			Choice[input_y-1][input_x-1]++;
		}
		//0 칸을 골랐을때 
		if(Gptr[input_y][input_x]==0){
			Choice[input_y-1][input_x-1]++;
			for(int x = 0; x >=0;){//0 Search 
				//인풋값기준 위쪽칸 확인 및 조건을 만족했을시 인풋값 변경, 히스토리 추가, FindNum실행 
				if((Gptr[input_y+1][input_x]==0 && Gptr[input_y+1][input_x]<=100) && Choice[input_y][input_x-1]==0){
					history[x]=((input_y-1) * L + (input_x-1));
					input_y++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				//인풋값 기준 우측칸 확인 나머지 위와같음 
				if((Gptr[input_y][input_x+1]==0 && Gptr[input_y][input_x+1]<=100) && Choice[input_y-1][input_x]==0){
					history[x]=((input_y-1) * L + (input_x-1));
					input_x++;
					Choice[input_y-1][input_x-1]++;
					x++;
					continue;
				}
				//인풋값 기준 아래 칸 확인 나머지 위와같음 
				if((Gptr[input_y-1][input_x]==0 && Gptr[input_y-1][input_x]<=100) && Choice[input_y-2][input_x-1]==0){
					history[x]=((input_y-1) * L + (input_x-1));
                                        input_y--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				}
				// 인풋기준 좌측칸 확인 나머지 위와같음 
				if((Gptr[input_y][input_x-1]==0 && Gptr[input_y][input_x-1]<=100) && Choice[input_y-1][input_x-2]==0){
                                        history[x]=((input_y-1) * L + (input_x-1));
                                        input_x--;
                                        Choice[input_y-1][input_x-1]++;
                                        x++;
                                        continue;
				} 
				//원래있던 Findnun : 지금위치 기준으로 주위 8칸에 0이아닌 수가있으면 열어줌 
				if(Gptr[input_y+1][input_x-1]>0 && Gptr[input_y+1][input_x-1]<M){Choice[input_y][input_x-2]++;}
			        if(Gptr[input_y+1][input_x]>0 && Gptr[input_y+1][input_x]<M){ Choice[input_y][input_x-1]++;}
			        if(Gptr[input_y+1][input_x+1]>0 && Gptr[input_y+1][input_x+1]<M){Choice[input_y][input_x]++;}
			        if(Gptr[input_y][input_x-1]>0 && Gptr[input_y][input_x-1]<M){Choice[input_y-1][input_x-2]++;}
			        if(Gptr[input_y][input_x+1]>0 && Gptr[input_y][input_x+1]<M){Choice[input_y-1][input_x]++;}
			        if(Gptr[input_y-1][input_x-1]>0 && Gptr[input_y-1][input_x-1]<M){Choice[input_y-2][input_x-2]++;}
			        if(Gptr[input_y-1][input_x]>0 && Gptr[input_y-1][input_x]<M){Choice[input_y-2][input_x-1]++;}
			        if(Gptr[input_y-1][input_x+1]>0 && Gptr[input_y-1][input_x+1]<M){Choice[input_y-2][input_x]++;}
				//인풋기준 4방향모두 탐색 불가할때 지금위치 기준으로 FindNum실행 및 히스토리 기준으로 한단계 이전으로
				input_y=((history[x-1]/L)+1);
				input_x=((history[x-1]%L)+1);
				x--;
			}
		}
		//플래그 갯수=지뢰갯수일때 동작, Choice의 플래그위치의 값들이 전부 마인일경우 승리문구뜨고 종료 
		//두번테스트해서 당장은 문구안뜨는데 진빠져서 나중에할예정 
		if(flag_counter == M){
			for(int a = 0; a < L; a++){
				for(int b = 0; b < L; b++){
					if(Choice[a][b]>=100 && Gptr[a+1][b+1]<0){temp++;}
				}
			}
			if(temp == flag_counter){
				printf("Congratulations! You find all Mine\n");
				break;
			}
		}
	}
}

