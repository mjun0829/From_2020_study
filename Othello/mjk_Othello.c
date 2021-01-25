#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int start_x = 4, start_y = 6;
float avail_count = 0, count = 0;
void window();				// 기본 윈도우 표시 
void initialize_color();
void game(int **, int, int);
int keypad_manager(int**, int, int, int, int*, int*, int);
void search(int **mat, int row, int col, int turn); 			// 현재플레이어가 둘 수 있는 위치 검색
void search_loop(int **mat, int y, int x, int, int, int);		// search에서 출력하는 위치 검색함수 
void update_color(int **mat, int y, int x, int turn);			//color_loop 8 times and update
void color_loop(int **mat, int y, int x, int, int, int turn);		// 플레이어 위치 선정이후 돌색깔 변환 함수 
void information();
void make_origin_othello(int **mat, int row, int col);
void update_turn(int turn);
void update_othello(int **mat, int row, int col);
void update_cursor(int **mat, int y, int x);
int change_turn(int **mat, int row, int col, int turn);
int mat_range_check(int y, int x, int range);
int mat_color_check(int **mat, int y, int x, int y_t, int x_t, int y_t2, int x_t2, int turn);
int opposite_turn(int);
int direction_check(int, int, int, int, int);
int forward_search(int**, int*, int*, int, int, int);
int backward_coloring(int**, int*, int*, int, int, int, int, int);
void ai_test(int**, int, int, int);
void init_greedy(int A[8][8]);
void ai_search(int a[8][8], int**, int, int, int, int, int);
int ai_index_search(int a[8][8]);
int ai_max_search(int a[8][8]);
int calculate_standard_dev(int A[8][8]);
void plus_standard_dev(int A[8][8]);
void ai_next_check(int **mat, int greedy[8][8], int row, int col, int turn);
void ai_next_search(int**, int A[8][8], int, int, int, int, int);
void matrix_copy(int**, int**, int, int);
void print_greedy(int A[8][8], int row, int col);
int **make_2d_array_pointer(int**, int, int);
void free_2d_array_pointer(int**, int);
enum value
{
	Empty = 0,
	Player_1 = 1,
	Player_2 = 2,
	Available = 10
};

enum direction
{
	Forward = 20,
	Backward = 30
};

enum value_greedy{
        greedy_empty = -1000
};

void window(){
	int win_x = 0, win_y = 0; 	// 윈도우 최대치받기용 
	noecho();
	getmaxyx(stdscr, win_y, win_x);
	WINDOW *win = newwin(0, 0, win_x, win_y);
}

void game(int **mat, int row, int col){
	keypad(stdscr, TRUE);
	int y = start_y, x = start_x;
	int turn = Player_1;
	while(1){
		update_turn(turn);
		search(mat, row, col, turn);
		update_othello(mat, row, col);
		if(turn == Player_2){
			refresh();
			sleep(2);
			ai_test(mat, row, col, turn);
			turn = change_turn(mat, row, col, turn);
			continue;
		}
		attron(COLOR_PAIR(3)); // 커서 색깔 
        	mvprintw(y, x, "O");
	        refresh();
		int input = getch();
		if(input == 'q'){
			break;
		}
		update_cursor(mat, y, x);
		turn = keypad_manager(mat, row, col, input, &y, &x, turn);
	}	
}

int keypad_manager(int **mat, int row, int col, int input, int *y, int *x, int turn){
	int y_origin = *y, x_origin = *x;
	switch(input){ // 키보드 입력시 커서  위치변동 
                case KEY_UP:
                        if((*y - 1) < start_y){break;}
                        *y = *y - 1;
			break;
                case KEY_DOWN:
                        if((*y + 1) > (start_y + (row-1))){break;}
                        *y = *y + 1;
			break;
                case KEY_RIGHT:
                        if((*x + 2) > (start_x + (col-1) * 2)){break;}
                        *x = *x + 2;
			break;
                case KEY_LEFT:
                        if((*x - 2) < start_x){break;}
                        *x = *x - 2;
			break;
                case ' ':// 플레이어가 선택 시 색깔바꿔주기 및 커서위치 초기화, 플레이어 전환 
                        if(mat[*y-start_y][(*x-start_x)/2] == Available){
                                update_color(mat, y_origin, x_origin, turn);
                                turn = change_turn(mat, row, col, turn);
                                *y = start_y;
                                *x = start_x;
                        }
			break;
	}
	return turn;
}

void initialize_color(){
	curs_set(FALSE);
        start_color();
        init_pair(Player_1, COLOR_RED, COLOR_BLACK);    // 플레이어 1 색 
        init_pair(Player_2, COLOR_BLUE, COLOR_BLACK);   // 플레이어 2 색 
        init_pair(3, COLOR_BLACK, COLOR_WHITE); // 커서 색 
        init_pair(4, COLOR_WHITE, COLOR_BLACK); // 기본 색
        init_pair(5, COLOR_GREEN, COLOR_BLACK); // 유효칸 색
}

void search(int **mat, int row, int col, int turn){
	int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			for(int z = 0; z < 8; z++){
			search_loop(mat, y, x, dir[z][0], dir[z][1], turn);
    			}			
		}               
	}
}

// ydir, xdir 은 8방향 중에 나아갈 방향 
void search_loop(int **mat, int y, int x, int y_dir, int x_dir, int turn){
	int y_t = y;
	int x_t = x;
	while(1){// 배열에 들어간 값 범위체크, 다음 칸 빈칸 또는 이미 유효칸이면 패스, 같은 색깔 칸이면 패스 
                if(!mat_range_check(y_t + y_dir, x_t + x_dir, 8)){break;}
                if(mat[y_t + y_dir][x_t + x_dir] == Empty || mat[y_t + y_dir][x_t + x_dir] == Available){break;}
                if(mat_color_check(mat, y, x, y_t, x_t, y_t + y_dir, x_t + x_dir, turn)){
			if(!(x == x_t && y == y_t)){
                        	mat[y][x] = Available;
                        	break;
                	}
		}
               	if(mat[y_t + y_dir][x_t + x_dir] == turn ){break;}
               	y_t = y_t + y_dir;
		x_t = x_t + x_dir;
	}
}

void update_color(int **mat, int y, int x, int turn){
	int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	mat[y-start_y][(x-start_x)/2]=turn;
	for(int z = 0; z < 8; z++){
        	color_loop(mat, (y-start_y), (x-start_x)/2, dir[z][0], dir[z][1], turn);
        }
 	attron(COLOR_PAIR(turn));
 	mvprintw(y, x, "O");
 	refresh();
}


void color_loop(int **mat, int y, int x, int y_dir, int x_dir, int turn){
	int y_now = y, x_now = x;
	int dir_check = Forward;
	if(mat_range_check(y + y_dir, x + x_dir, 8) && (mat[y][x] != mat[y + y_dir][x + x_dir]) && (mat[y + y_dir][x + x_dir] != Empty)){
		if(dir_check == Forward){
		       	if(forward_search(mat, &y_now, &x_now, y_dir, x_dir, turn)){
				dir_check = Backward;
			}
		}
		if(dir_check == Backward){ 
			backward_coloring(mat, &y_now, &x_now, y_dir, x_dir, y, x, turn);
		}
		
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
				attron(COLOR_PAIR(Player_1));
				mat[a][b] = Player_1;
			}
			if((a == 3 && b == 4) || (a == 4 && b == 3)){
				attron(COLOR_PAIR(Player_2));
				mat[a][b] = Player_2;
			}
			mvprintw(a + start_y, 2 * b + start_x, "O");
                }
        }
}

void update_turn(int turn){
	attron(COLOR_PAIR(turn)); 
        mvprintw(5, 8, "%dP Turn", turn);
}

void update_othello(int **mat, int row, int col){
	for(int a = 0; a < row; a++){ // 오셀로 판 업데이트 
                for(int b = 0; b < col; b++){
                        if(mat[a][b] == Empty){attron(COLOR_PAIR(4)); }// 빈칸 
                        if(mat[a][b] == Player_1 || mat[a][b] == Player_2){attron(COLOR_PAIR(mat[a][b])); }// 플레이어고른칸 
                        if(mat[a][b] == Available){attron(COLOR_PAIR(5)); } // 플레이어가 고를수 있는 칸
                	mvprintw(a + start_y, 2 * b + start_x, "O");
		}
	}
	refresh();
}

void update_cursor(int **mat, int y, int x){
        if(mat[y-start_y][(x-start_x)/2] == Empty){
                attron(COLOR_PAIR(4));
        }
        if(mat[y-start_y][(x-start_x)/2] != Empty){
                attron(COLOR_PAIR(mat[y-start_y][(x-start_x)/2]));
        }
        mvprintw(y, x, "O");//커서이동후 원래 색깔로 돌려줌
}

int change_turn(int **mat, int row, int col, int turn){
	for(int q = 0; q < row; q++){// 현재 플레이어 유효칸 초기화(중복표시 방지)
		for(int w = 0; w < col; w++){
			if(mat[q][w] == Available){mat[q][w] = Empty; }
		}
	}
	return opposite_turn(turn);
	
}

int mat_range_check(int y, int x, int range){
	return (y >= 0 && y < range && x >= 0 && x < range);
}

int mat_color_check(int **mat, int y_origin, int x_origin, int y_now, int x_now, int y_next, int x_next, int turn){
	return (mat[y_origin][x_origin] == Empty && mat[y_next][x_next] == turn && mat[y_now][x_now] == opposite_turn(turn));
}

int opposite_turn(int turn){
	return (3-turn);
}

int forward_search(int **mat, int *y_now, int *x_now, int y_dir, int x_dir, int turn){
	while(1){
		*y_now = *y_now + y_dir;
		*x_now = *x_now + x_dir;
		if(!mat_range_check(*y_now, *x_now, 8)){return false;}
		if(mat[*y_now][*x_now] == turn){return true;}
		if(mat[*y_now][*x_now] == Empty){return false;}
	}
}

int backward_coloring(int **mat, int *y_now, int *x_now, int y_dir, int x_dir, int y, int x, int turn){
	while(1){
		*y_now = *y_now - y_dir;
                *x_now = *x_now - x_dir;
		if(y == *y_now && x == *x_now){break;}
		if(mat[*y_now][*x_now] == opposite_turn(turn)){mat[*y_now][*x_now] = turn;	}
	}
}

void ai_test(int **mat, int row, int col, int turn){
	int index = 0, index_y = 0, index_x = 0;
	int greedy_array[8][8];
	int standard_dev = 0;
	int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	init_greedy(greedy_array);
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			if(mat[y][x] == Available){
				greedy_array[y][x] = 0;
				for(int z = 0; z < 8; z++){
					ai_search(greedy_array, mat, y, x, dir[z][0], dir[z][1], turn);
				}
			}
		}
	}
	plus_standard_dev(greedy_array);
	ai_next_check(mat, greedy_array, row, col, turn);
	print_greedy(greedy_array, row, col);
	index = ai_index_search(greedy_array);
	index_y = (index / 8) + start_y;
	index_x = (index % 8) * 2 + start_x;
	mvprintw(0,0,"y : %d x : %d",index/8,index%8);
	update_color(mat, index_y, index_x, Player_2);
}

void init_greedy(int greedy[8][8]){
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			greedy[y][x] = greedy_empty;
		}
	}
}
void ai_search(int greedy[8][8], int **mat, int y, int x, int y_dir, int x_dir, int turn){
	int greed = 0;
	int y_now = y, x_now = x;
	while(1){
		if(!mat_range_check(y_now + y_dir, x_now + x_dir, 8) || mat[y_now + y_dir][x_now + x_dir] == Empty){
			greed = 0;
			break;
		}
		if(mat[y_now + y_dir][x_now + x_dir] == opposite_turn(turn)){greed++;}
		if(mat[y_now + y_dir][x_now + x_dir] == turn){break;}
		y_now = y_now + y_dir;
		x_now = x_now + x_dir;
		
	}
	greedy[y][x] = greedy[y][x] + greed;
}

int ai_index_search(int greedy[8][8]){
	int max = greedy_empty;
	int max_index = 0;
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			if(greedy[y][x] > max){
				max = greedy[y][x];
				max_index = (y * 8) + x;
			}
		}
	}
	return max_index;
}

int ai_max_search(int greedy[8][8]){
	int max = greedy_empty;
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			if(greedy[y][x] > max){
				max = greedy[y][x];
			}
		}
	}
	return max;
}

int calculate_standard_dev(int greedy[8][8]){
	int standard_dev = 0;
	double average = 0;
	double total = 0;
	double total_2 = 0;
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			if(greedy[y][x] > greedy_empty){
				total++;
				average = average + (double)greedy[y][x];
				total_2 = total_2 + pow((double)greedy[y][x], 2.0);
			}
		}
	}
	average = pow(average / total, 2.0);
	standard_dev = sqrt(total_2 - average);
	return standard_dev;
}

void plus_standard_dev(int greedy[8][8]){
	int standard_dev = calculate_standard_dev(greedy);
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			if(greedy[y][x] > greedy_empty){
				greedy[y][x] = greedy[y][x] + standard_dev;
			}
		}
	}
}

void ai_next_check(int **mat, int greedy[8][8], int row, int col, int turn){
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			if(greedy[y][x] > 0){
				ai_next_search(mat, greedy, row, col, y, x, turn);
			}
		}
	}
}

void ai_next_search(int **mat, int greedy[8][8], int row, int col, int a, int b, int turn){
	int temp_greedy[8][8] = {0,};
	int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	int **mat_copy;
       	mat_copy = make_2d_array_pointer(mat, row, col);
	matrix_copy(mat, mat_copy, row, col);
	turn = change_turn(mat_copy, row, col, turn);
	search(mat_copy, row, col, turn);
	for(int y = 0; y < row; y++){
                for(int x = 0; x < col; x++){
                        if(mat_copy[y][x] == Available){
                                for(int z = 0; z < 8; z++){
                                        ai_search(temp_greedy, mat_copy, y, x, dir[z][0], dir[z][1], turn);
                                }
                        }
                }
        }
	plus_standard_dev(temp_greedy);
	free_2d_array_pointer(mat_copy, row);
	greedy[a][b] = greedy[a][b] - ai_max_search(temp_greedy);
}

void matrix_copy(int **mat, int **mat_copy, int row, int col){
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			mat_copy[y][x] = mat[y][x];
		}	
	}
}

void print_greedy(int greedy[8][8], int row, int col){
	for(int y = 0; y < row; y++){
                for(int x = 0; x < col; x++){
			if(greedy[y][x] > greedy_empty){
                        	mvprintw(y + start_y, 2 * x + 24, "%d", greedy[y][x]);
			}
			else {
				mvprintw(y + start_y, 2 * x + 24, "X");
			}
                }
        }
}

void make_priority(int **mat){
	int priority_array[8][8] = {{7, 1, 6, 2, 2, 6, 1, 7}, {1, 1, 3, 3, 3, 3, 1, 1}, {6, 3, 5, 4, 4, 5, 3, 6}, {2, 3, 4, 0, 0, 4, 3, 2}, {2, 3, 4, 0, 0, 4, 3, 2}, {6, 3, 5, 4, 4, 5, 3, 6}, {1, 1, 3, 3, 3, 3, 1, 1}, {7, 1, 6, 2, 2, 6, 1, 7}};
	int priority_max = 0, priority_index = 0;
}

int available_check(int **mat){
	int count = 0;
	for(int y = 0 ; y < 8 ; y++){
		for(int x = 0; x < 8; x++){
			if(mat[y][x] == Available){
			count++;	
			}
		}
	}
	return count;
}

int **make_2d_array_pointer(int **mat, int row, int col){
	mat = malloc(sizeof(int *) * row);
	for(int x = 0; x < row; x++){
		mat[x] = malloc(sizeof(int) * col);
		for(int y = 0; y < col; y++){
			mat[x][y] = 0;
		}
	}
	return mat;
}

void free_2d_array_pointer(int **mat, int row){
	for(int x = 0; x < row; x++){
                free(mat[x]);
        }
        free(mat);
}

int main(){
	int row_size = 8;
	int col_size = 8;
	int **o_matrix;
        o_matrix = make_2d_array_pointer(o_matrix, row_size, col_size); 
	initscr();
	window();
	initialize_color();
        information();
        make_origin_othello(o_matrix, row_size, col_size);
	game(o_matrix, row_size, col_size);
	endwin();
	free_2d_array_pointer(o_matrix, row_size);	
}

