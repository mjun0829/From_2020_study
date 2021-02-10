#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct _Ball {
  int y;
  int x;
  int back_y;
  int back_x;
  int dir[2];
} Ball;

typedef struct _Screen {
  int max_y;
  int max_x;
  int game_y;
  int game_x;
  int game_score_y;
  int game_score_x;
} Screen;

typedef struct _Player {
  int y;
  int x;
  int size;
} Player;

void window(Screen);
void game_window(Screen);
void print_player(Player);
void print_ball(Ball);
Ball init_ball(Ball);
Ball move_ball(Ball, Screen, Player);
Ball ball_collision_check(Ball, Screen, Player);
Screen init_screen(Screen);
Player init_player(Player);

void window(Screen screen) {
  initscr();
  noecho();
  WINDOW *win = newwin(0, 0, screen.max_x, screen.max_y);
}

void game_window(Screen screen) {
  mvprintw(
      0, 0,
      "I--------------------------------------------------II----------------I");
  for (int y = 1; y < screen.max_y; y++) {
    mvprintw(y, 0,
             "I                                                  II            "
             "    I");
  }
  mvprintw(
      screen.max_y, 0,
      "I--------------------------------------------------II----------------I");
  refresh();
}

void print_player(Player player) {
  for (int x = 0; x < player.size; x++) {
    mvprintw(player.y, player.x + x, "I");
  }
  refresh();
}
Ball init_ball(Ball ball) {
  ball.y = 3;
  ball.x = 3;
  ball.dir[0] = 1;
  ball.dir[1] = 1;
  return ball;
}

void print_ball(Ball ball) {
  mvprintw(ball.back_y, ball.back_x, " ");
  mvprintw(ball.y, ball.x, "O");
  refresh();
}

Ball move_ball(Ball ball, Screen screen, Player player) {
  game_window(screen);
  print_player(player);
  time_t start, now, limit;
  limit = clock();
  while (1) {
    print_ball(ball);
    start = clock();
    while (1) {
      now = clock();
      if ((double)(now - start) / (double)CLOCKS_PER_SEC >= 0.0167) {
        break;
      }
    }
    ball.back_y = ball.y;
    ball.back_x = ball.x;
    ball.y += ball.dir[0];
    ball.x += ball.dir[1];
    ball = ball_collision_check(ball, screen, player);
    if ((double)(now - limit) / (double)CLOCKS_PER_SEC >= 10.0) {
      break;
    }
  }
}

Ball ball_collision_check(Ball ball, Screen screen, Player player) {
  if (ball.y == 1 || ball.y == screen.game_y + 1) {
    ball.dir[0] = -ball.dir[0];
  }
  if (ball.x == 1 || ball.x == screen.game_x) {
    ball.dir[1] = -ball.dir[1];
  }
  if (ball.y == player.y &&
      (ball.x == player.x - 1 || ball.x == player.x + player.size)) {
    ball.dir[1] = -ball.dir[1];
  }
  if (ball.y == player.y - 1 &&
      (ball.x == player.x - 1 || ball.x == player.x + player.size)) {
    ball.dir[0] = -ball.dir[0];
    ball.dir[1] = -ball.dir[1];
  }
  if (ball.y == player.y - 1 &&
      (ball.x >= player.x && ball.x < player.x + player.size)) {
    ball.dir[0] = -ball.dir[0];
  }
  return ball;
}

Screen init_screen(Screen screen) {
  screen.max_y = 35;
  screen.max_x = 70;
  screen.game_y = 33;
  screen.game_x = 50;
  screen.game_score_y = 33;
  screen.game_score_x = 16;
  return screen;
}

Player init_player(Player player) {
  player.y = 33;
  player.x = 10;
  player.size = 32;
  return player;
}

int main() {
  Ball t_ball;
  Screen t_screen;
  Player t_player;
  t_ball = init_ball(t_ball);
  t_screen = init_screen(t_screen);
  t_player = init_player(t_player);
  window(t_screen);
  move_ball(t_ball, t_screen, t_player);
  endwin();
}
