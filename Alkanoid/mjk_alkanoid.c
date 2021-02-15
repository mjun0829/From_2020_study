#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


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

void initialize();
Screen init_screen(Screen);
Player init_player(Player);
Ball init_ball(Ball);
void print_game_screen_objects(Ball, Player, Screen);
void print_game_score_screen(Ball, Player, Screen);
Screen game_screen_init(Screen);
Ball move_ball(Ball, Screen, Player);
Ball ball_collision_check(Ball, Screen, Player);
Player move_player(Player, Screen, int input);

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
        ball.y = 3;
        ball.x = 3;
        ball.dir[0] = 1;
        ball.dir[1] = 1;
        return ball;
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

void print_game_screen_objects(Ball ball, Player player, Screen screen){
	wborder(screen.game_screen, '|', '|', '-', '-', '+' ,'+', '+', '+');
	for(int x = 0; x < player.size; x++){
                mvwprintw(screen.game_screen, player.y, player.x + x, "I");
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
Ball move_ball(Ball ball, Screen screen, Player player){
	screen = game_screen_init(screen);
	time_t start, now, limit;
	limit = clock();
	while(1){
		print_game_screen_objects(ball, player, screen);
		print_game_score_screen(ball, player, screen);
		int input = getch();
		if(input > 0){
			player = move_player(player, screen, input);
		}
        	start = clock();
		while(1){
			now = clock();
			if((double)(now - start)/(double)CLOCKS_PER_SEC >= 0.0167){
				break;
			}
		}
		ball.back_y = ball.y;
		ball.back_x = ball.x;
		ball.y += ball.dir[0];
		ball.x += ball.dir[1];
		ball = ball_collision_check(ball, screen, player);
		if((double)(now - limit)/(double)CLOCKS_PER_SEC >= 10.0){
			break;
		}//테스트용 타이머 
	}
}

Ball ball_collision_check(Ball ball, Screen screen, Player player){
	if(ball.y == 1 || ball.y == screen.game_y - 2){
		ball.dir[0] = - ball.dir[0];
	}
	if(ball.x == 1 || ball.x == screen.game_x - 2){
		ball.dir[1] = - ball.dir[1];
	}
	if(ball.y == player.y && (ball.x == player.x - 1 || ball.x == player.x + player.size)){
		ball.dir[1] = - ball.dir[1];
	}
	if(ball.y == player.y - 1 && (ball.x == player.x - 1 || ball.x == player.x + player.size)){
		ball.dir[0] = - ball.dir[0];
		ball.dir[1] = - ball.dir[1];
	}
	if(ball.y == player.y - 1 && (ball.x >= player.x && ball.x < player.x + player.size)){
		ball.dir[0] = - ball.dir[0];
	}
	return ball;
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
	t_ball = init_ball(t_ball);
	t_screen = init_screen(t_screen);
	t_player = init_player(t_player);
	initialize();
	move_ball(t_ball, t_screen, t_player);
	endwin();
}
