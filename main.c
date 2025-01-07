typedef unsigned long size_t;
typedef unsigned char uint8_t;

void *memset(void *dest, int val, size_t len) {
  unsigned char *ptr = dest;
  for (size_t i = 0; i < len; i++) {
    ptr[i] = (unsigned char)val;
  }
  return dest;
}

#define PI 3.14159265358979323846

#define BOARD_HEIGHT 32
#define BOARD_WIDTH (BOARD_HEIGHT * 2)
#define CELL_SIZE 20
#define SCREEN_WIDTH (BOARD_WIDTH * CELL_SIZE)
#define SCREEN_HEIGHT (BOARD_HEIGHT * CELL_SIZE)
#define PLAYER_RADIUS (SCREEN_HEIGHT * 0.05)
#define PLAYER_SPEED 100
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

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

typedef struct {
  vec2_t position;
  vec2_t velocity;
  color_t color;
} player_state_t;

typedef enum {
  ONE,
  TWO,
} player_t;

typedef void (*func_ptr)(float);

extern void set_canvas_size(int width, int height);
extern void clear_with_color(color_t color);
extern void fill_rect(float x, float y, float w, float h, color_t color);
extern void fill_circle(float x, float y, float radius, color_t color);
extern void set_update_frame(func_ptr f);
extern double cos(double x);
extern double sin(double x);

const color_t BACKGROUND_COLOR = {0x18, 0x18, 0x18, 0xFF};
const color_t CELL_COLOR = {0, 0, 0, 0xFF};

player_t board[BOARD_HEIGHT][BOARD_WIDTH];
player_state_t player1, player2;

void populate_board() {
  for (int x = 0; x < BOARD_WIDTH / 2; ++x) {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
      board[y][x] = ONE;
      board[y][x + BOARD_WIDTH / 2] = TWO;
    }
  }
}

color_t get_player_color(player_t player) {
  return player == ONE ? player1.color : player2.color;
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

void update_frame(float delta) {
  calculate_player_position(delta);
  clear_with_color(BACKGROUND_COLOR);
  draw_board();

  fill_circle(player1.position.x, player1.position.y, PLAYER_RADIUS,
              player1.color);
  fill_circle(player2.position.x, player2.position.y, PLAYER_RADIUS,
              player2.color);
}

int main(void) {
  set_canvas_size(SCREEN_WIDTH, SCREEN_HEIGHT);
  populate_board();

  player1.position = (vec2_t){SCREEN_WIDTH / 4.0, SCREEN_HEIGHT / 2.0};
  player1.velocity =
      multiply_vec2((vec2_t){cos(PI * .25), sin(PI * .25)}, PLAYER_SPEED);
  player1.color = (color_t){0xFF, 0xAA, 0xAA, 0xFF};

  player2.position =
      (vec2_t){SCREEN_WIDTH / 2.0 + SCREEN_WIDTH / 4.0, SCREEN_HEIGHT / 2.0};
  player2.velocity =
      multiply_vec2((vec2_t){cos(PI * 1.25), sin(PI * 1.25)}, PLAYER_SPEED);
  player2.color = (color_t){0xAA, 0xFF, 0xAA, 0xFF};

  set_update_frame(update_frame);
}
