#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void window(int**, int, int);
void search(int**, int, int, int);
void search_loop(int**, int, int, int, int, int, int);
void color_loop(int**, int, int, int, int, int);

void window(int **mat, int row, int col){
	int win_x = 0, win_y = 0; //윈도우 최대치받기용 
	int x = 0, y = 0;
        int end = 0;
	int start_x = 4, start_y = 6;
	int turn = 1;
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
	mvprintw(3, 2, "Press Q button to exit");
	mvprintw(1, 2, "White space is your cursor");
	mvprintw(2, 2, "You can move cursor with Keypad");
	mvprintw(4, 2, "1P is RED, 2P is BLUE, GREEN SPACE is active spot");
	// 기본 판 
	for(int a = 0; a < row; a++){
		for(int b = 0; b < col; b++){
			mvprintw(a + start_y, 2 * b + start_x, "O");
		}
	}
	y = start_y;
	x = start_x;
	refresh();
	while(end == 0){
		attron(COLOR_PAIR(turn));
		mvprintw(5, 8, "%dP Turn", turn);
		search(mat, row, col, turn);
		for(int a = 0; a < row; a++){
			for(int b = 0; b < col; b++){
				if(mat[a][b] == 0){attron(COLOR_PAIR(4)); }
				if(mat[a][b] == 1 || mat[a][b] == 2){attron(COLOR_PAIR(mat[a][b])); }
				if(mat[a][b] == turn * 10){attron(COLOR_PAIR(5)); }
				if(mat[a][b] == (3-turn) * 10){attron(COLOR_PAIR(4)); }
				mvprintw(a + start_y, 2 * b + start_x, "O");
			}
		}
		attron(COLOR_PAIR(3));// 커서 색깔 
		mvprintw(y, x, "O");
		int input = getch();
		if(input == 'q'){
			end++;
			break;
		}
		refresh();
		if(mat[y-start_y][(x-start_x)/2]==0){
			attron(COLOR_PAIR(4));
		}
		if(mat[y-start_y][(x-start_x)/2]!=0){
			attron(COLOR_PAIR(mat[y-start_y][(x-start_x)/2]));	
		}
		mvprintw(y, x, "O");//커서이동후 원래 색깔 
		switch(input){
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
		case ' ':
			if(mat[y-start_y][(x-start_x)/2]==0 || mat[y-start_y][(x-start_x)/2] >=10){
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
				if(turn==1){
					for(int q = 0; q < row; q++){
						for(int w = 0; w < col; w++){
							if(mat[q][w] == turn * 10){mat[q][w] = 0; }
						}
					}
					turn=2;
					y = start_y;
					x = start_x;
					continue;
				}
				if(turn==2){
					for(int q = 0; q < row; q++){
                                                for(int w = 0; w < col; w++){
                                                        if(mat[q][w] == turn * 10){mat[q][w] = 0; }
                                                }
                                        }
					turn=1;
					y = start_y;
					x = start_x;
					continue;	
				}
			}
			continue;

		}
		
	}	
	endwin();
}

void search(int **mat, int col, int row, int t){
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
void search_loop(int **mat, int y, int x, int y_dir, int x_dir, int p, int p_t){
	int y_t = y;
	int x_t = x;
	int dir_check = 0;
	if(y_dir != 0){dir_check = dir_check + 1; }
	if(x_dir != 0){dir_check = dir_check + 2; }
	while(1){
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
void color_loop(int **mat, int y, int x, int y_dir, int x_dir, int t){
	int y_t = y;
	int x_t = x;
	int color_search = 0;
	if((y + y_dir) >= 0 && (y + y_dir) < 8 && (x + x_dir) >= 0 && (x + x_dir) < 8 && mat[y][x] != mat[y + y_dir][x + x_dir]){
		while(color_search == 0){
			y_t = y_t + y_dir;
			x_t = x_t + x_dir;
			if(y_t < 0 || y_t >7 || x_t < 0 || x_t >7){
				color_search = color_search + 1;
				break;
		       	}
			if(mat[y_t][x_t] == t){break; }
		}
		while(color_search == 0){
			y_t = y_t - y_dir;
			x_t = x_t - x_dir;
			if(y_t == y && x_t == x){break; }
			if(mat[y_t][x_t] == 3-t){mat[y_t][x_t] = t; }
		}

	}
}
int main(){
	int **o_matrix;
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
