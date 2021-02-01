#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct _Ball{
        int y;
	int x;
	int dir[2];
}Ball;

void window();
Ball init_ball(Ball);
Ball print_ball(Ball);
Ball move_ball(Ball);
Ball ball_collision_check(Ball);

void window(){
	initscr();
	noecho();
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	WINDOW *win = newwin(0, 0, max_x, max_y);
}

Ball init_ball(Ball ball){
	ball.y = 0;
	ball.x = 0;
	ball.dir[0] = 1;
	ball.dir[1] = 1;
	return ball;
}

Ball print_ball(Ball ball){
	clear();
	mvprintw(ball.y, ball.x, "O");
	refresh();
}
Ball move_ball(Ball ball){
	time_t start, now, limit;
	limit = clock();
	while(1){
		print_ball(ball);
        	start = clock();
		while(1){
			now = clock();
			if((double)(now - start)/(double)CLOCKS_PER_SEC >= 0.05){
				break;
			}
		}
		ball.y += ball.dir[0];
		ball.x += ball.dir[1];
		ball = ball_collision_check(ball);
		if((double)(now - limit)/(double)CLOCKS_PER_SEC >= 10.0){
			break;
		}
	}
}

Ball ball_collision_check(Ball ball){
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	if(ball.y == 0 || ball.y == max_y - 1){
		ball.dir[0] = - ball.dir[0];
	}
	if(ball.x == 0 || ball.x == max_x - 2){
		ball.dir[1] = - ball.dir[1];
	}
	return ball;
}

int main(){
	Ball t_ball;
	t_ball = init_ball(t_ball);
	window();
	keypad(stdscr, TRUE);
	move_ball(t_ball);
	endwin();
}
