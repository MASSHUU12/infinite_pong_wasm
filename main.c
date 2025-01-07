#define PI 3.14159265358979323846

#define BOARD_HEIGHT 20
#define BOARD_WIDTH (BOARD_HEIGHT * 2)
#define BOARD_WIDTH2 (BOARD_WIDTH / 2)
#define CELL_SIZE 20
#define SCREEN_WIDTH (BOARD_WIDTH * CELL_SIZE)
#define SCREEN_WIDTH2 (SCREEN_WIDTH / 2.f)
#define SCREEN_WIDTH4 (SCREEN_WIDTH / 4.f)
#define SCREEN_HEIGHT (BOARD_HEIGHT * CELL_SIZE)
#define SCREEN_HEIGHT2 (SCREEN_HEIGHT / 2.f)
#define PLAYER_RADIUS (SCREEN_HEIGHT * .05)
#define PLAYER_SPEED 400

typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef void (*func_ptr)(float);

void *memset(void *dest, int val, size_t len) {
  unsigned char *ptr = dest;
  for (size_t i = 0; i < len; i++) {
    ptr[i] = (unsigned char)val;
  }
  return dest;
}

int floor_f(float x) {
  int i = (int)x;
  if (x < 0 && x != i) {
    i--;
  }
  return i;
}

float min(float a, float b) { return a < b ? a : b; }
float max(float a, float b) { return a > b ? a : b; }

int rect_circle_collision(float l, float r, float t, float b, float cx,
                          float cy, float radius) {
  float x = min(max(l, cx), r);
  float y = min(max(t, cy), b);
  float dx = cx - x;
  float dy = cy - y;

  return dx * dx + dy * dy <= radius * radius;
}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

typedef struct {
  float x;
  float y;
} vec2_t;

vec2_t multiply_vec2(vec2_t vector, float scalar) {
  return (vec2_t){vector.x * scalar, vector.y * scalar};
}

vec2_t add_vec2(vec2_t vector1, vec2_t vector2) {
  return (vec2_t){vector1.x + vector2.x, vector1.y + vector2.y};
}

typedef enum {
  ONE,
  TWO,
} player_t;

player_t get_opposite_player(player_t kind) { return kind == ONE ? TWO : ONE; }

player_t board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
  vec2_t position;
  vec2_t velocity;
  color_t color;
  player_t kind;
} player_state_t;

player_state_t player1, player2;

int player_eats_enemy_cell(float cx, float cy, player_t kind) {
  int bx = floor_f((cx - PLAYER_RADIUS) / CELL_SIZE);
  int by = floor_f((cy - PLAYER_RADIUS) / CELL_SIZE);
  int tx = floor_f((cx + PLAYER_RADIUS) / CELL_SIZE);
  int ty = floor_f((cy + PLAYER_RADIUS) / CELL_SIZE);

  for (int x = bx; x < tx; ++x) {
    for (int y = by; y <= ty; ++y) {
      float l = x * CELL_SIZE;
      float r = (x + 1) * CELL_SIZE;
      float t = y * CELL_SIZE;
      float b = (y + 1) * CELL_SIZE;
      if (rect_circle_collision(l, r, t, b, cx, cy, PLAYER_RADIUS) &&
          board[y][x] == get_opposite_player(kind)) {
        board[y][x] = kind;
        return 1;
      }
    }
  }

  return 0;
}

void player_update(player_state_t *player, float delta) {
  float nx = player->position.x + player->velocity.x * delta;
  if (nx - PLAYER_RADIUS < 0 || nx + PLAYER_RADIUS >= SCREEN_WIDTH ||
      player_eats_enemy_cell(nx, player->position.y, player->kind)) {
    player->velocity.x *= -1;
  } else {
    player->position.x = nx;
  }

  float ny = player->position.y + player->velocity.y * delta;
  if (ny - PLAYER_RADIUS < 0 || ny + PLAYER_RADIUS >= SCREEN_HEIGHT ||
      player_eats_enemy_cell(player->position.x, ny, player->kind)) {
    player->velocity.y *= -1;
  } else {
    player->position.y = ny;
  }
}

extern void set_canvas_size(int width, int height);
extern void clear_with_color(color_t color);
extern void fill_rect(float x, float y, float w, float h, color_t color);
extern void fill_circle(float x, float y, float radius, color_t color);
extern void set_update_frame(func_ptr f);
extern double cos(double x);
extern double sin(double x);

const color_t BACKGROUND_COLOR = {0x18, 0x18, 0x18, 0xFF};
const color_t BOARD_LEFT_COLOR = {0xAA, 0xFF, 0xAA, 0xFF};
const color_t BOARD_RIGHT_COLOR = {0xFF, 0xAA, 0xAA, 0xFF};

void populate_board() {
  for (int x = 0; x < BOARD_WIDTH2; ++x) {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
      board[y][x] = ONE;
      board[y][x + BOARD_WIDTH2] = TWO;
    }
  }
}

color_t get_player_color(player_t player) {
  return player == ONE ? player2.color : player1.color;
}

void draw_board() {
  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      fill_rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                get_player_color(board[y][x]));
    }
  }
}

void calculate_player_position(const float delta) {
  player1.position =
      add_vec2(player1.position, multiply_vec2(player1.velocity, delta));
  player2.position =
      add_vec2(player2.position, multiply_vec2(player2.velocity, delta));
}

void draw_player() {
  fill_circle(player1.position.x, player1.position.y, PLAYER_RADIUS,
              player1.color);
  fill_circle(player2.position.x, player2.position.y, PLAYER_RADIUS,
              player2.color);
}

void update_frame(float delta) {
  player_update(&player1, delta);
  player_update(&player2, delta);

  clear_with_color(BACKGROUND_COLOR);
  draw_board();
  draw_player();
}

int main(void) {
  set_canvas_size(SCREEN_WIDTH, SCREEN_HEIGHT);
  populate_board();

  player1 = (player_state_t){
      .position = (vec2_t){SCREEN_WIDTH4, SCREEN_HEIGHT2},
      .velocity =
          multiply_vec2((vec2_t){cos(PI * .25), sin(PI * .25)}, PLAYER_SPEED),
      .color = BOARD_LEFT_COLOR,
      .kind = ONE,
  };
  player2 = (player_state_t){
      .position = (vec2_t){SCREEN_WIDTH2 + SCREEN_WIDTH4, SCREEN_HEIGHT2},
      .velocity =
          multiply_vec2((vec2_t){cos(PI * 1.25), sin(PI * 1.25)}, PLAYER_SPEED),
      .color = BOARD_RIGHT_COLOR,
      .kind = TWO,
  };

  set_update_frame(update_frame);
}
