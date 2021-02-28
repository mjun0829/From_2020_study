#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct _Ball{
        int y;
	int x;
	int back_y;
	int back_x;
	int dir[2];
	int score;
	int status;
	double hardness;
}Ball;

typedef struct _Screen{
	WINDOW *game_screen;
	WINDOW *game_score_screen;
	WINDOW *start_screen;
	WINDOW *option_screen;
	int max_y;
	int max_x;
	int game_y;
	int game_x;
	int game_score_y;
	int game_score_x;
	double frame;
}Screen;

typedef struct _Player{
	int y;
	int x;
	int size;
}Player;

typedef struct _Block{
	int y;
	int x;
	int size;
	int active;
}Block;

void initialize();
Screen init_screen(Screen);
Player init_player(Player);
Ball init_ball(Ball);
Block init_block(Block block[5][16]);
void print_game_screen_objects(Ball, Player, Screen, Block block[5][16]);
void print_game_score_screen(Ball, Player, Screen);
Screen game_screen_init(Screen);
Screen start_screen_init(Screen);
Screen option_screen_init(Screen);
Ball move_ball(Ball, Screen, Player, Block block[5][16]);
Ball ball_collision_check(Ball, Screen, Player, Block block[5][16]);
int boom_check(Ball, Screen);
int y_axis_collision(Ball, Screen);
int x_axis_collision(Ball, Screen);
int player_left_right_collision(Ball, Player);
int player_upper_collision(Ball, Player);
int block_y_axis_collision(Ball, Screen, Block block[5][16]);
int block_x_axis_collision(Ball, Screen, Block block[5][16]);
Player move_player(Player, Screen, int input);
void print_start_screen(Screen);
void print_cursor(Screen, int cursor);
void erase_cursor(Screen, int cursor);
void print_option_screen(Screen);
void print_option_cursor(Screen, int cursor);
void erase_option_cursor(Screen, int cursor);
void game(Ball, Screen, Player, Block block[5][16]);
Ball option_screen(Ball, Screen);

enum value{
	y_dir = 0,
	x_dir = 1,
	block_start_y = 5,
	block_start_x = 1,
	block_size = 3,
	block_max_in_y = 5,
	block_max_in_x = 16,
	cursor_game = 1,
	cursor_option = 2,
	on = 1000,
	off = 2000,
	victory = 99,
	boom = 999,
	in_game = 9
};

void initialize(){
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
}

Screen init_screen(Screen screen){
        screen.max_y = 35;
        screen.max_x = 70;
        screen.game_y = 35;
        screen.game_x = 50;
        screen.game_score_y = 35;
        screen.game_score_x = 20;
	screen.frame = 0.0166;
        return screen;
}

Player init_player(Player player){
        player.y = 33;
        player.x = 18;
        player.size = 16;
        return player;
}

Ball init_ball(Ball ball){
        ball.y = 32;
        ball.x = 25;
        ball.dir[y_dir] = -1;
        ball.dir[x_dir] = 1;
	ball.score = 0;
	ball.status = in_game;
	ball.hardness = 10;
        return ball;
}

Block init_block(Block block[5][16]){
	for(int y = 0; y < block_max_in_y; y++){
		for(int x = 0; x < block_max_in_x; x++){
			block[y][x].y = block_start_y + y;
			block[y][x].x = block_start_x + block_size * x ;
			block[y][x].size = block_size;
			block[y][x].active = on;
		}
	}
}

Screen game_screen_init(Screen screen){
	screen.game_screen = newwin(screen.game_y, screen.game_x, 0, 0);
        screen.game_score_screen = newwin(screen.game_score_y, screen.game_score_x, 0, screen.game_x);
	wtimeout(screen.game_screen, 0);
        wtimeout(screen.game_score_screen, 0);
	wrefresh(screen.game_screen);
	wrefresh(screen.game_score_screen);
	return screen;
}

Screen start_screen_init(Screen screen){
	screen.start_screen = newwin(screen.max_y, screen.max_x, 0, 0);
	return screen;
}

Screen option_screen_init(Screen screen){
	screen.option_screen = newwin(screen.max_y, screen.max_x, 0, 0);
	return screen;
}

void print_game_screen_objects(Ball ball, Player player, Screen screen, Block block[5][16]){
	wborder(screen.game_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	for(int x = 0; x < player.size; x++){
                mvwprintw(screen.game_screen, player.y, player.x + x, "I");
        }
	for(int y = 0; y < block_max_in_y; y++){
		for(int x = 0; x < block_max_in_x; x++){
			if(block[y][x].active == on){
				mvwprintw(screen.game_screen, block[y][x].y, block[y][x].x, "+-+");
			}
		}
	}
	mvwprintw(screen.game_screen, ball.back_y, ball.back_x, " ");
        mvwprintw(screen.game_screen, ball.y, ball.x, "O");
        wrefresh(screen.game_screen);
}

void print_game_score_screen(Ball ball, Player player, Screen screen){
	wborder(screen.game_score_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	mvwprintw(screen.game_score_screen, 15, 5, "SCORE : %d", ball.score);
	mvwprintw(screen.game_score_screen, 20, 3, "HARDNESS : %.0f", ball.hardness);
	wrefresh(screen.game_score_screen);	
}

Ball move_ball(Ball ball, Screen screen, Player player, Block block[5][16]){
	block[5][16] = init_block(block);
	screen = game_screen_init(screen);
	time_t start, now, limit, ball_start = clock(), ball_end;
	limit = clock();
	while(ball.status != victory){
		print_game_screen_objects(ball, player, screen, block);
		print_game_score_screen(ball, player, screen);
		int input = getch();
		if(input > 0){
			player = move_player(player, screen, input);
		}
        	start = clock();
		while(1){
			now = clock();
			if((double)(now - start)/(double)CLOCKS_PER_SEC >= screen.frame){
				break;
			}
		}
		ball_end = clock();
		if((double)(ball_end - ball_start)/(double)CLOCKS_PER_SEC >= (0.07 - 0.003 * ball.hardness) - screen.frame){
			ball.back_y = ball.y;
			ball.back_x = ball.x;
			ball.y += ball.dir[0];
			ball.x += ball.dir[1];
			ball = ball_collision_check(ball, screen, player, block);
			ball_start = clock();
		}
		if(ball.status == boom){
			break;
		}
	}
}

Ball ball_collision_check(Ball ball, Screen screen, Player player, Block block[5][16]){
	if(y_axis_collision(ball, screen)){
		ball.dir[y_dir] = - ball.dir[y_dir];
	}
	if(x_axis_collision(ball, screen)){
		ball.dir[x_dir] = - ball.dir[x_dir];
	}
	if(player_left_right_collision(ball, player)){
		ball.dir[x_dir] = - ball.dir[x_dir];
	}
	if(player_upper_collision(ball, player)){
		ball.dir[y_dir] = - ball.dir[y_dir];
	}
	if(block_y_axis_collision(ball, screen, block)){
		ball.dir[y_dir] = - ball.dir[y_dir];
		ball.score++;
	}
	if(block_x_axis_collision(ball, screen, block)){
		ball.dir[x_dir] = - ball.dir[x_dir];
		ball.score++;
	}
	if(boom_check(ball, screen)){
		ball.status = boom;
	}
	return ball;
}

int boom_check(Ball ball, Screen screen){
	return(ball.y == screen.game_y - 2);
}

int y_axis_collision(Ball ball, Screen screen){
	return (ball.y == 1);
}

int x_axis_collision(Ball ball, Screen screen){
	return (ball.x == 1 || ball.x == screen.game_x - 2);
}

int player_left_right_collision(Ball ball, Player player){
	return (ball.y == player.y && (ball.x == player.x - 1 || ball.x == player.x + player.size));
}

int player_upper_collision(Ball ball, Player player){
	return (ball.y == player.y - 1 && (ball.x >= player.x - 1 && ball.x <= player.x + player.size));
}

int block_y_axis_collision(Ball ball, Screen screen, Block block[5][16]){
	int t_y = ball.y + ball.dir[y_dir] - block_start_y;
	int t_x = ((ball.x - block_start_x) / block_size);
	if(t_y <= 4 && t_y >= 0){
		if(block[t_y][t_x].active == on){
			block[t_y][t_x].active = off;
			mvwprintw(screen.game_screen, block[t_y][t_x].y, block[t_y][t_x].x, "   ");
			return TRUE;
		}
	}
	return FALSE;	
}

int block_x_axis_collision(Ball ball, Screen screen, Block block[5][16]){
	int t_y = ball.y - block_start_y;
	int t_x = ((ball.x - block_start_x + ball.dir[x_dir]) / block_size);
	if(t_y <= 4 && t_y >= 0){
		if(block[t_y][t_x].active == on){
			block[t_y][t_x].active = off;
                        mvwprintw(screen.game_screen, block[t_y][t_x].y, block[t_y][t_x].x, "   ");
                        return TRUE;
		}
	}
	return FALSE;
}

Player move_player(Player player, Screen screen, int input){
	switch(input){
		case KEY_LEFT:
			if(player.x > 1){
				player.x = player.x - 1;
				mvwprintw(screen.game_screen, player.y, player.x + player.size , " ");
			}
			break;
		case KEY_RIGHT:
			if(player.x < screen.game_x - player.size - 1){
				mvwprintw(screen.game_screen, player.y, player.x , " ");
				player.x = player.x + 1;
			}
			break;
	}
	return player;
}

void print_start_screen(Screen screen){
	wborder(screen.start_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	mvwprintw(screen.start_screen, 5, 3, "     /\\      |\\/\\  |  /      /\\      |\\     |  //\\\\  ||| |\\\\  ");
	mvwprintw(screen.start_screen, 6, 3, "    /  \\     |/  \\ | /      /  \\     | \\    | /    \\  |  |  \\ ");
	mvwprintw(screen.start_screen, 7, 3, "   /    \\    |\\  / |/      /    \\    |  \\   | |    |  |  |   |");
	mvwprintw(screen.start_screen, 8, 3, "  /------\\   |/\\/  |\\     /------\\   |   \\  | |    |  |  |   |");
	mvwprintw(screen.start_screen, 9, 3, " /        \\  |  \\  | \\   /        \\  |    \\ | \\    /  |  |  / ");
	mvwprintw(screen.start_screen, 10, 3, "/          \\ |   \\ |  \\ /          \\ |     \\|  \\\\//  ||| |//  ");
	mvwprintw(screen.start_screen, 18, 28, "+------------+");
	mvwprintw(screen.start_screen, 19, 28, "| GAME START |");
	mvwprintw(screen.start_screen, 20, 28, "+------------+");
	mvwprintw(screen.start_screen, 28, 28, "+----------+");
	mvwprintw(screen.start_screen, 29, 28, "|  OPTION  |");
	mvwprintw(screen.start_screen, 30, 28, "+----------+");
	mvwprintw(screen.start_screen, 33, 2, "Press 'Q' to quit game");
	wrefresh(screen.start_screen);
}

void print_cursor(Screen screen, int cursor){
	mvwprintw(screen.start_screen, 10 * cursor + 8, 23, "\\\\ ");
	mvwprintw(screen.start_screen, 10 * cursor + 9, 23, " ||");
	mvwprintw(screen.start_screen, 10 * cursor + 10, 23, "// ");
	wrefresh(screen.start_screen);
}

void erase_cursor(Screen screen, int cursor){
	mvwprintw(screen.start_screen, 10 * cursor + 8, 23, "   ");
        mvwprintw(screen.start_screen, 10 * cursor + 9, 23, "   ");
        mvwprintw(screen.start_screen, 10 * cursor + 10, 23, "   ");
	wrefresh(screen.start_screen);
}

void print_option_screen(Screen screen){
	wborder(screen.option_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	mvwprintw(screen.option_screen, 2, 6, "Press space bar to choose hardness and quit option screen");
	mvwprintw(screen.option_screen, 4, 12, "1    2    3    4    5    6    7    8    9    10");
	wrefresh(screen.option_screen);
}

void print_option_cursor(Screen screen, int cursor){
	mvwprintw(screen.option_screen, 5, 7 + cursor * 5, "^");
	wrefresh(screen.option_screen);
}

void erase_option_cursor(Screen screen, int cursor){
	mvwprintw(screen.option_screen, 5, 7 + cursor * 5, " ");
	wrefresh(screen.option_screen);
}

void game(Ball ball, Screen screen, Player player, Block block[5][16]){
	screen = start_screen_init(screen);
	int cursor = 1;
	int end = on;
	while(end == on){
		print_start_screen(screen);
		print_cursor(screen, cursor);
		int input = getch();
		switch(input){
			case KEY_UP:
				if(cursor == cursor_option){
					erase_cursor(screen, cursor);
					cursor = cursor_game;
				}
				continue;
			case KEY_DOWN:
				if(cursor == cursor_game){
					erase_cursor(screen, cursor);
					cursor = cursor_option;
				}
				continue;
			case ' ':
				if(cursor == cursor_game){
					clear();
					move_ball(ball, screen, player, block);
				}
				if(cursor == cursor_option){
					clear();
					ball = option_screen(ball, screen);
				}
				continue;
			case 'q':
				end = off;
				break;		
		}
	}
}

Ball option_screen(Ball ball, Screen screen){
	int cursor = 1, end = on;
	screen = option_screen_init(screen);
	while(end == on){
		print_option_screen(screen);
		print_option_cursor(screen, cursor);
		int input = getch();
		switch(input){
			case KEY_RIGHT:
				if(cursor < 10){
					erase_option_cursor(screen, cursor);
					cursor++;
				}
				continue;
			case KEY_LEFT:
				if(cursor > 1){
					erase_option_cursor(screen, cursor);
					cursor--;
				}
				continue;
			case ' ':
				ball.hardness = (double)cursor;
				end = off;
				break;
		}
	}
	return ball;
}

int main(){
	Ball t_ball;
	Screen t_screen;
	Player t_player;
	Block t_block[5][16];
	t_ball = init_ball(t_ball);
	t_screen = init_screen(t_screen);
	t_player = init_player(t_player);
	t_block[5][16] = init_block(t_block);
	initialize();
	game(t_ball, t_screen, t_player, t_block);
	endwin();
}
