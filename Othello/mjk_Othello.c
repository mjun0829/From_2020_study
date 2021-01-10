#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int start_x = 4, start_y = 6;
void window(); // 기본 윈도우 표시
void initialize_color();
void game(int **, int, int);
int keypad_manager(int **, int, int, int, int *, int *, int);
void search(int **mat, int row, int col,
            int turn); // 현재플레이어가 둘 수 있는 위치 검색
void search_loop(int **mat, int y, int x, int, int,
                 int); // search에서 출력하는 위치 검색함수
void update_color(int **mat, int y, int x,
                  int turn); // color_loop 8 times and update
void color_loop(int **mat, int y, int x, int, int,
                int turn); // 플레이어 위치 선정이후 돌색깔 변환 함수
void information();
void make_origin_othello(int **mat, int row, int col);
void update_turn(int turn);
void update_othello(int **mat, int row, int col);
void update_cursor(int **mat, int y, int x);
int change_turn(int **mat, int row, int col, int turn);
int mat_range_check(int y, int x, int range);
int mat_color_check(int **mat, int y, int x, int y_t, int x_t, int y_t2,
                    int x_t2, int turn);
int opposite_turn(int);
int direction_check(int, int, int, int, int);
int forward_search(int **, int *, int *, int, int, int);
int backward_coloring(int **, int *, int *, int, int, int, int, int);
void ai_test(int **, int, int);
void ai_search(int a[8][8], int **, int, int, int, int);
int ai_max_search(int a[8][8]);

enum value { Empty = 0, Player_1 = 1, Player_2 = 2, Available = 10 };

enum direction { Forward = 20, Backward = 30 };

void window() {
  int win_x = 0, win_y = 0; // 윈도우 최대치받기용
  noecho();
  getmaxyx(stdscr, win_y, win_x);
  WINDOW *win = newwin(0, 0, win_x, win_y);
}

void game(int **mat, int row, int col) {
  keypad(stdscr, TRUE);
  int y = start_y, x = start_x;
  int turn = Player_1;
  while (1) {
    update_turn(turn);
    search(mat, row, col, turn);
    update_othello(mat, row, col);
    if (turn == Player_2) {
      refresh();
      sleep(2);
      ai_test(mat, row, col);
      turn = change_turn(mat, row, col, turn);
      continue;
    }
    attron(COLOR_PAIR(3)); // 커서 색깔
    mvprintw(y, x, "O");
    refresh();
    int input = getch();
    if (input == 'q') {
      break;
    }
    update_cursor(mat, y, x);
    turn = keypad_manager(mat, row, col, input, &y, &x, turn);
  }
}

int keypad_manager(int **mat, int row, int col, int input, int *y, int *x,
                   int turn) {
  int y_origin = *y, x_origin = *x;
  switch (input) { // 키보드 입력시 커서  위치변동
  case KEY_UP:
    if ((*y - 1) < start_y) {
      break;
    }
    *y = *y - 1;
    break;
  case KEY_DOWN:
    if ((*y + 1) > (start_y + (row - 1))) {
      break;
    }
    *y = *y + 1;
    break;
  case KEY_RIGHT:
    if ((*x + 2) > (start_x + (col - 1) * 2)) {
      break;
    }
    *x = *x + 2;
    break;
  case KEY_LEFT:
    if ((*x - 2) < start_x) {
      break;
    }
    *x = *x - 2;
    break;
  case ' ': // 플레이어가 선택 시 색깔바꿔주기 및 커서위치 초기화, 플레이어 전환
    if (mat[*y - start_y][(*x - start_x) / 2] == Available) {
      update_color(mat, y_origin, x_origin, turn);
      turn = change_turn(mat, row, col, turn);
      *y = start_y;
      *x = start_x;
    }
    break;
  }
  return turn;
}

void initialize_color() {
  curs_set(FALSE);
  start_color();
  init_pair(Player_1, COLOR_RED, COLOR_BLACK);  // 플레이어 1 색
  init_pair(Player_2, COLOR_BLUE, COLOR_BLACK); // 플레이어 2 색
  init_pair(3, COLOR_BLACK, COLOR_WHITE);       // 커서 색
  init_pair(4, COLOR_WHITE, COLOR_BLACK);       // 기본 색
  init_pair(5, COLOR_GREEN, COLOR_BLACK);       // 유효칸 색
}

void search(int **mat, int row, int col, int turn) {
  int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                   {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  for (int y = 0; y < row; y++) {
    for (int x = 0; x < col; x++) {
      for (int z = 0; z < 8; z++) {
        search_loop(mat, y, x, dir[z][0], dir[z][1], turn);
      }
    }
  }
}

// ydir, xdir 은 8방향 중에 나아갈 방향
void search_loop(int **mat, int y, int x, int y_dir, int x_dir, int turn) {
  int y_t = y;
  int x_t = x;
  while (1) { // 배열에 들어간 값 범위체크, 다음 칸 빈칸 또는 이미 유효칸이면
              // 패스, 같은 색깔 칸이면 패스
    if (!mat_range_check(y_t + y_dir, x_t + x_dir, 8)) {
      break;
    }
    if (mat[y_t + y_dir][x_t + x_dir] == Empty ||
        mat[y_t + y_dir][x_t + x_dir] == Available) {
      break;
    }
    if (mat_color_check(mat, y, x, y_t, x_t, y_t + y_dir, x_t + x_dir, turn)) {
      if (!(x == x_t && y == y_t)) {
        mat[y][x] = Available;
        break;
      }
    }
    if (mat[y_t + y_dir][x_t + x_dir] == turn) {
      break;
    }
    y_t = y_t + y_dir;
    x_t = x_t + x_dir;
  }
}

void update_color(int **mat, int y, int x, int turn) {
  int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                   {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  mat[y - start_y][(x - start_x) / 2] = turn;
  for (int z = 0; z < 8; z++) {
    color_loop(mat, (y - start_y), (x - start_x) / 2, dir[z][0], dir[z][1],
               turn);
  }
  attron(COLOR_PAIR(turn));
  mvprintw(y, x, "O");
  refresh();
}

void color_loop(int **mat, int y, int x, int y_dir, int x_dir, int turn) {
  int y_now = y, x_now = x;
  int dir_check = Forward;
  if (mat_range_check(y + y_dir, x + x_dir, 8) &&
      (mat[y][x] != mat[y + y_dir][x + x_dir])) {
    if (dir_check == Forward) {
      if (forward_search(mat, &y_now, &x_now, y_dir, x_dir, turn)) {
        dir_check = Backward;
      }
    }
    if (dir_check == Backward) {
      backward_coloring(mat, &y_now, &x_now, y_dir, x_dir, y, x, turn);
    }
  }
}

void information() {
  mvprintw(3, 2, "Press Q button to exit");
  mvprintw(1, 2, "White space is your cursor");
  mvprintw(2, 2, "You can move cursor with Keypad");
  mvprintw(4, 2, "1P is RED, 2P is BLUE, GREEN SPACE is active spot");
}

void make_origin_othello(int **mat, int row, int col) {
  for (int a = 0; a < row; a++) {
    for (int b = 0; b < col; b++) {
      attron(COLOR_PAIR(4));
      if ((a == 3 && b == 3) || (a == 4 && b == 4)) {
        attron(COLOR_PAIR(Player_1));
        mat[a][b] = Player_1;
      }
      if ((a == 3 && b == 4) || (a == 4 && b == 3)) {
        attron(COLOR_PAIR(Player_2));
        mat[a][b] = Player_2;
      }
      mvprintw(a + start_y, 2 * b + start_x, "O");
    }
  }
}

void update_turn(int turn) {
  attron(COLOR_PAIR(turn));
  mvprintw(5, 8, "%dP Turn", turn);
}

void update_othello(int **mat, int row, int col) {
  for (int a = 0; a < row; a++) { // 오셀로 판 업데이트
    for (int b = 0; b < col; b++) {
      if (mat[a][b] == Empty) {
        attron(COLOR_PAIR(4));
      } // 빈칸
      if (mat[a][b] == Player_1 || mat[a][b] == Player_2) {
        attron(COLOR_PAIR(mat[a][b]));
      } // 플레이어고른칸
      if (mat[a][b] == Available) {
        attron(COLOR_PAIR(5));
      } // 플레이어가 고를수 있는 칸
      mvprintw(a + start_y, 2 * b + start_x, "O");
    }
  }
  refresh();
}

void update_cursor(int **mat, int y, int x) {
  if (mat[y - start_y][(x - start_x) / 2] == Empty) {
    attron(COLOR_PAIR(4));
  }
  if (mat[y - start_y][(x - start_x) / 2] != Empty) {
    attron(COLOR_PAIR(mat[y - start_y][(x - start_x) / 2]));
  }
  mvprintw(y, x, "O"); //커서이동후 원래 색깔로 돌려줌
}

int change_turn(int **mat, int row, int col, int turn) {
  for (int q = 0; q < row; q++) { // 현재 플레이어 유효칸 초기화(중복표시 방지)
    for (int w = 0; w < col; w++) {
      if (mat[q][w] == Available) {
        mat[q][w] = Empty;
      }
    }
  }
  return opposite_turn(turn);
}

int mat_range_check(int y, int x, int range) {
  return (y >= 0 && y < range && x >= 0 && x < range);
}

int mat_color_check(int **mat, int y_origin, int x_origin, int y_now, int x_now,
                    int y_next, int x_next, int turn) {
  return (mat[y_origin][x_origin] == Empty && mat[y_next][x_next] == turn &&
          mat[y_now][x_now] == opposite_turn(turn));
}

int opposite_turn(int turn) { return (3 - turn); }

int forward_search(int **mat, int *y_now, int *x_now, int y_dir, int x_dir,
                   int turn) {
  while (1) {
    *y_now = *y_now + y_dir;
    *x_now = *x_now + x_dir;
    if (!mat_range_check(*y_now, *x_now, 8)) {
      return false;
    }
    if (mat[*y_now][*x_now] == turn) {
      return true;
    }
    if (mat[*y_now][*x_now] == Empty) {
      return false;
    }
  }
}

int backward_coloring(int **mat, int *y_now, int *x_now, int y_dir, int x_dir,
                      int y, int x, int turn) {
  while (1) {
    *y_now = *y_now - y_dir;
    *x_now = *x_now - x_dir;
    if (y == *y_now && x == *x_now) {
      break;
    }
    if (mat[*y_now][*x_now] == opposite_turn(turn)) {
      mat[*y_now][*x_now] = turn;
    }
  }
}

void ai_test(int **mat, int row, int col) {
  int index = 0, index_y = 0, index_x = 0;
  int greedy_array[8][8] = {
      0,
  };
  int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                   {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  for (int y = 0; y < row; y++) {
    for (int x = 0; x < col; x++) {
      if (mat[y][x] == Available) {
        for (int z = 0; z < 8; z++) {
          ai_search(greedy_array, mat, y, x, dir[z][0], dir[z][1]);
        }
      }
      mvprintw(y + start_y, 2 * x + 24, "%d", greedy_array[y][x]);
    }
  }
  index = ai_max_search(greedy_array);
  index_y = (index / 8) + start_y;
  index_x = (index % 8) * 2 + start_x;
  mvprintw(0, 0, "y : %d x : %d", index / 8, index % 8);
  update_color(mat, index_y, index_x, Player_2);
}

void ai_search(int greedy[8][8], int **mat, int y, int x, int y_dir,
               int x_dir) {
  int greed = 0;
  int y_now = y, x_now = x;
  while (1) {
    if (!mat_range_check(y_now + y_dir, x_now + x_dir, 8) ||
        mat[y_now + y_dir][x_now + x_dir] == Empty) {
      greed = 0;
      break;
    }
    if (mat[y_now + y_dir][x_now + x_dir] == Player_1) {
      greed++;
    }
    if (mat[y_now + y_dir][x_now + x_dir] == Player_2) {
      break;
    }
    y_now = y_now + y_dir;
    x_now = x_now + x_dir;
  }
  greedy[y][x] = greedy[y][x] + greed;
}

int ai_max_search(int greedy[8][8]) {
  mvprintw(1, 0, "fuck");
  int max = 0;
  int max_index = 0;
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      if (greedy[y][x] > max) {
        max = greedy[y][x];
        max_index = (y * 8) + x;
      }
    }
  }
  return max_index;
}
int main() {
  int **o_matrix; // 2차원 배열 포인터 선언
  int row_size = 8;
  int col_size = 8;
  o_matrix = malloc(sizeof(int *) * row_size);
  for (int x = 0; x < row_size; x++) {
    o_matrix[x] = malloc(sizeof(int) * col_size);
    for (int y = 0; y < col_size; y++) {
      o_matrix[x][y] = 0;
    }
  }
  initscr();
  window();
  initialize_color();
  information();
  make_origin_othello(o_matrix, row_size, col_size);
  game(o_matrix, row_size, col_size);
  endwin();
  for (int x = 0; x < row_size; x++) {
    free(o_matrix[x]);
  }
  free(o_matrix);
}
