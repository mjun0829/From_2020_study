#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int start_x = 4, start_y = 6;
void window(int **mat, int row, int col); 				// 기본 윈도우 표시 
void search(int **mat, int row, int col, int turn); 			// 현재플레이어가 둘 수 있는 위치 검색
void search_loop(int **mat, int y, int x, int, int, int, int);		// search에서 출력하는 위치 검색함수 
void update_color(int **mat, int y, int x, int turn);			//color_loop 8 times and update
void color_loop(int **mat, int y, int x, int, int, int turn);		// 플레이어 위치 선정이후 돌색깔 변환 함수 
void information();
void make_origin_othello(int **mat, int row, int col);
void update_turn(int turn);
void update_othello(int **mat, int row, int col, int turn);
void update_cursor(int **mat, int y, int x);
void change_turn(int **mat, int row, int col, int turn);

void window(int **mat, int row, int col){
	int win_x = 0, win_y = 0; 	// 윈도우 최대치받기용 
	int x = 0, y = 0;		// 커서 위치 받을 좌표값 
        int end = 0;			// 함수 종료용 변수 
	int turn = 1;			// 현재의 플레이어 표시 변수 
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK); 	// 플레이어 1 색 
	init_pair(2, COLOR_BLUE, COLOR_BLACK);	// 플레이어 2 색 
	init_pair(3, COLOR_BLACK, COLOR_WHITE); // 커서 색 
	init_pair(4, COLOR_WHITE, COLOR_BLACK);	// 기본 색
       	init_pair(5, COLOR_GREEN, COLOR_BLACK); // 유효칸 색 	
	getmaxyx(stdscr, win_y, win_x);
	WINDOW *win = newwin(0, 0, win_x, win_y);
	information();
	make_origin_othello(mat, row, col);
	y = start_y;	// 시작좌표로 초기화 
	x = start_x;	// 시작좌표로 초기화 
	refresh();
	while(end == 0){
		update_turn(turn);
		search(mat, row, col, turn); 
		update_othello(mat, row, col, turn);
		attron(COLOR_PAIR(3)); // 커서 색깔 
        	mvprintw(y, x, "O");
	        refresh();
		int input = getch();
		if(input == 'q'){
			end++;
			break;
		}
		update_cursor(mat, y, x);
		switch(input){ // 키보드 입력시 커서  위치변동 
		case KEY_UP:
			if((y - 1) < start_y){continue;}
			y = y - 1;
			continue;
		case KEY_DOWN:
			if((y + 1) > (start_y+(row-1))){continue;}
			y = y + 1;
			continue;
		case KEY_RIGHT:
			if((x + 2) > (start_x + (col-1) * 2)){continue;}
			x = x + 2;
			continue;
		case KEY_LEFT:
			if((x - 2) < start_x){continue;}
			x = x - 2;
			continue;
		case ' ':// 플레이어가 선택 시 색깔바꿔주기 및 커서위치 초기화, 플레이어 전환 
			if(mat[y-start_y][(x-start_x)/2]==0 || mat[y-start_y][(x-start_x)/2] >=10){
				update_color(mat, y, x, turn);
				change_turn(mat, row, col, turn);
				y = start_y;
				x = start_x;
				continue;
			}
			continue;
		}	
	}	
	endwin();
}

void search(int **mat, int row, int col, int t){
	int end_check = 0;
	int y_t = 0;
	int x_t = 0;
	int t_t = 0;
	if(t == 1){t_t = 2;}
	if(t == 2){t_t = 1;}
	for(int y = 0; y < row; y++){
		for(int x = 0; x< col; x++){
			search_loop(mat, y, x, -1, 0, t, t_t);
			search_loop(mat, y, x, -1, 1, t, t_t);
			search_loop(mat, y, x, -1, -1, t, t_t);
			search_loop(mat, y, x, 0, 1, t, t_t);
			search_loop(mat, y, x, 0, -1, t, t_t);
			search_loop(mat, y, x, 1, 0, t, t_t);
			search_loop(mat, y, x, 1, 1, t, t_t);
			search_loop(mat, y, x, 1, -1, t, t_t);                    
		}               
	}
}
// ydir, xdir 은 8방향 중에 나아갈 방향 
void search_loop(int **mat, int y, int x, int y_dir, int x_dir, int p, int p_t){
	int y_t = y;
	int x_t = x;
	int dir_check = 0;
	if(y_dir != 0){dir_check = dir_check + 1; }
	if(x_dir != 0){dir_check = dir_check + 2; }
	while(1){// 배열에 들어간 값 범위체크, 다음 칸 빈칸 또는 이미 유효칸이면 패스, 같은 색깔 칸이면 패스 
                if((y_t + y_dir) < 0 || (y_t + y_dir) > 7 || (x_t + x_dir) < 0 || (x_t + x_dir) > 7){break;}
                if(mat[y_t + y_dir][x_t + x_dir] == 0 || mat[y_t + y_dir][x_t + x_dir] >= 10){break;}
                if(dir_check == 1){
			if(y_t != y && mat[y][x] == 0 && mat[y_t + y_dir][x_t + x_dir] == p && mat[y_t][x_t] == p_t){
                        	if(p == 1){mat[y][x] = 10; }
				if(p == 2){mat[y][x] = 20; }
                        	break;
                	}
		}
		if(dir_check == 2){
                        if(x_t != x && mat[y][x] == 0 && mat[y_t + y_dir][x_t + x_dir] == p && mat[y_t][x_t] == p_t){
                                if(p == 1){mat[y][x] = 10; }
                                if(p == 2){mat[y][x] = 20; }
                                break;
                        }
                }
		if(dir_check == 3){
                        if(y_t != y && x_t != x && mat[y][x] == 0 && mat[y_t + y_dir][x_t + x_dir] == p && mat[y_t][x_t] == p_t){
                                if(p == 1){mat[y][x] = 10; }
                                if(p == 2){mat[y][x] = 20; }
                                break;
                        }
                }
                if(mat[y_t + y_dir][x_t + x_dir] == p ){break;}
                y_t = y_t + y_dir;
		x_t = x_t + x_dir;
                }
}

void update_color(int **mat, int y, int x, int turn){
	mat[y-start_y][(x-start_x)/2]=turn;
	color_loop(mat, (y-start_y), (x-start_x)/2, -1, 1, turn);
	color_loop(mat, (y-start_y), (x-start_x)/2, -1, 0, turn);
	color_loop(mat, (y-start_y), (x-start_x)/2, -1, -1, turn);
 	color_loop(mat, (y-start_y), (x-start_x)/2, 0, 1, turn);
 	color_loop(mat, (y-start_y), (x-start_x)/2, 0, -1, turn);
 	color_loop(mat, (y-start_y), (x-start_x)/2, 1, 1, turn);
 	color_loop(mat, (y-start_y), (x-start_x)/2, 1, 0, turn);
 	color_loop(mat, (y-start_y), (x-start_x)/2, 1, -1, turn);
 	attron(COLOR_PAIR(turn));
 	mvprintw(y, x, "O");
 	refresh();
}
void color_loop(int **mat, int y, int x, int y_dir, int x_dir, int t){
	int y_t = y;
	int x_t = x;
	int color_search = 0;	//반복문 제어용 
	// 배열에 들어갈 값 범위 체크 및 현재 값과 다음칸의 값이 다를 경우 진행
	while(1){ 
	if((y + y_dir) >= 0 && (y + y_dir) < 8 && (x + x_dir) >= 0 && (x + x_dir) < 8 && mat[y][x] != mat[y + y_dir][x + x_dir]){
		y_t = y;
		x_t = x;
		while(color_search == 0){
			y_t = y_t + y_dir;
			x_t = x_t + x_dir;
			if(y_t < 0 || y_t >7 || x_t < 0 || x_t >7){ // 범위넘어가면 아래쪽 반복문이랑 같이 종료 
				color_search = color_search + 1;
				break;
		       	}
			if(mat[y_t][x_t] == t){break; }
		}
		while(color_search == 0){// 탐색된 값부터 뒤로 돌아가면서 진행 
			y_t = y_t - y_dir;
			x_t = x_t - x_dir;
			if(y_t == y && x_t == x){break; }
			if(mat[y_t][x_t] == 3-t){mat[y_t][x_t] = t; }
		}
	}
	y = y + y_dir;//만약 넘어서도 다른색 돌이있으면 찾아서 진행 
	x = x + x_dir;
	if(y > 7 || y < 0 || x < 0 || x > 7){break;	}
	}
}

void information(){
	mvprintw(3, 2, "Press Q button to exit");
        mvprintw(1, 2, "White space is your cursor");
        mvprintw(2, 2, "You can move cursor with Keypad");
        mvprintw(4, 2, "1P is RED, 2P is BLUE, GREEN SPACE is active spot");
}

void make_origin_othello(int **mat,int row, int col){
	for(int a = 0; a < row; a++){
                for(int b = 0; b < col; b++){
			attron(COLOR_PAIR(4));
			if((a == 3 && b == 3) || (a == 4 && b == 4)){
				attron(COLOR_PAIR(1));
				mat[a][b] = 1;
			}
			if((a == 3 && b == 4) || (a == 4 && b == 3)){
				attron(COLOR_PAIR(2));
				mat[a][b] = 2;
			}
			mvprintw(a + start_y, 2 * b + start_x, "O");
                }
        }
}

void update_turn(int turn){
	attron(COLOR_PAIR(turn)); 
        mvprintw(5, 8, "%dP Turn", turn);
}

void update_othello(int **mat, int row, int col, int turn){
	for(int a = 0; a < row; a++){ // 오셀로 판 업데이트 
                for(int b = 0; b < col; b++){
                        if(mat[a][b] == 0){attron(COLOR_PAIR(4)); }// 빈칸 
                        if(mat[a][b] == 1 || mat[a][b] == 2){attron(COLOR_PAIR(mat[a][b])); }// 플레이어고른칸 
                        if(mat[a][b] == turn * 10){attron(COLOR_PAIR(5)); } // 플레이어가 고를수 있는 칸
                        if(mat[a][b] == (3-turn) * 10){attron(COLOR_PAIR(4)); } // 반대 플레이어가 고를수 있는칸
                	mvprintw(a + start_y, 2 * b + start_x, "O");
		}
	}
}

void update_cursor(int **mat, int y, int x){
        if(mat[y-start_y][(x-start_x)/2]==0){
                attron(COLOR_PAIR(4));
        }
        if(mat[y-start_y][(x-start_x)/2]!=0){
                attron(COLOR_PAIR(mat[y-start_y][(x-start_x)/2]));
        }
        mvprintw(y, x, "O");//커서이동후 원래 색깔로 돌려줌
}

void change_turn(int **mat, int row, int col, int turn){
	if(turn==1){
		for(int q = 0; q < row; q++){// 현재 플레이어 유효칸 초기화(중복표시 방지)
			for(int w = 0; w < col; w++){
				if(mat[q][w] == turn * 10){mat[q][w] = 0; }
			}
		}
		turn=2;
		return;
	}
	if(turn==2){
		for(int q = 0; q < row; q++){
			for(int w = 0; w < col; w++){
				if(mat[q][w] == turn * 10){mat[q][w] = 0; }
			}
		}
		turn=1;
		return;
	}
}

int main(){
	int **o_matrix;		//2차원 배열 포인터 선언 
	int row_size = 8;
	int col_size = 8;
	o_matrix = malloc(sizeof(int *) * row_size);
	for(int x = 0; x < row_size; x++){
		o_matrix[x] = malloc(sizeof(int) * col_size);
		for(int y = 0; y < col_size; y++){
			o_matrix[x][y] = 0;
		}
	}
	window(o_matrix, row_size, col_size);
	for(int x = 0; x < row_size; x++){
		free(o_matrix[x]);
	}
	free(o_matrix);
	
}
