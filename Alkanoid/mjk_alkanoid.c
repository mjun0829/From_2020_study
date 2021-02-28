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
}Ball;

typedef struct _Screen{
	WINDOW *game_screen;
	WINDOW *game_score_screen;
	int max_y;
	int max_x;
	int game_y;
	int game_x;
	int game_score_y;
	int game_score_x;
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
Ball move_ball(Ball, Screen, Player, Block block[5][16]);
Ball ball_collision_check(Ball, Screen, Player, Block block[5][16]);
int y_axis_collision(Ball, Screen);
int x_axis_collision(Ball, Screen);
int player_left_right_collision(Ball, Player);
int player_upper_collision(Ball, Player);
int block_y_axis_collision(Ball, Screen, Block block[5][16]);
int block_x_axis_collision(Ball, Screen, Block block[5][16]);
Player move_player(Player, Screen, int input);

enum value{
	y_dir = 0,
	x_dir = 1,
	on = 1000,
	off = 2000
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
        return ball;
}

Block init_block(Block block[5][16]){
	for(int y = 0; y < 5; y++){
		for(int x = 0; x < 16; x++){
			block[y][x].y = 5 + y;
			block[y][x].x = 3 * x + 1;
			block[y][x].size = 3;
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

void print_game_screen_objects(Ball ball, Player player, Screen screen, Block block[5][16]){
	wborder(screen.game_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	for(int x = 0; x < player.size; x++){
                mvwprintw(screen.game_screen, player.y, player.x + x, "I");
        }
	for(int y = 0; y < 5; y++){
		for(int x = 0; x < 16; x++){
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
	mvwprintw(screen.game_score_screen, 15, 5, "SCORE : ");
	wrefresh(screen.game_score_screen);	
}

Ball move_ball(Ball ball, Screen screen, Player player, Block block[5][16]){
	screen = game_screen_init(screen);
	time_t start, now, limit;
	limit = clock();
	while(1){
		print_game_screen_objects(ball, player, screen, block);
		print_game_score_screen(ball, player, screen);
		int input = getch();
		if(input > 0){
			player = move_player(player, screen, input);
		}
        	start = clock();
		while(1){
			now = clock();
			if((double)(now - start)/(double)CLOCKS_PER_SEC >= 0.01){
				break;
			}
		}
		ball.back_y = ball.y;
		ball.back_x = ball.x;
		ball.y += ball.dir[0];
		ball.x += ball.dir[1];
		ball = ball_collision_check(ball, screen, player, block);
		if((double)(now - limit)/(double)CLOCKS_PER_SEC >= 20.0){
			break;
		}//테스트용 타이머 
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
	}
	if(block_x_axis_collision(ball, screen, block)){
		ball.dir[x_dir] = - ball.dir[x_dir];
	}
	return ball;
}

int y_axis_collision(Ball ball, Screen screen){
	return (ball.y == 1 || ball.y == screen.game_y - 2);
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
	int temp_y = ball.y + ball.dir[y_dir] - 5;
	int temp_x = ((ball.x - 1) / 3);
	if(temp_y <= 4 && temp_y >= 0){
		if(block[temp_y][temp_x].active == on){
			block[temp_y][temp_x].active = off;
			mvwprintw(screen.game_screen, block[temp_y][temp_x].y, block[temp_y][temp_x].x, "   ");
			return TRUE;
		}
	}
	return FALSE;	
}

int block_x_axis_collision(Ball ball, Screen screen, Block block[5][16]){
	int temp_y = ball.y - 5;
	int temp_x = ((ball.x - 1 + ball.dir[x_dir]) / 3);
	if(temp_y <= 4 && temp_y >= 0){
		if(block[temp_y][temp_x].active == on){
			block[temp_y][temp_x].active = off;
                        mvwprintw(screen.game_screen, block[temp_y][temp_x].y, block[temp_y][temp_x].x, "   ");
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
	move_ball(t_ball, t_screen, t_player, t_block);
	endwin();
}
