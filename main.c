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
#define TERMS 10

#define BOARD_HEIGHT 32
#define BOARD_WIDTH BOARD_HEIGHT * 2
#define CELL_SIZE 20
#define SCREEN_WIDTH BOARD_WIDTH *CELL_SIZE
#define SCREEN_HEIGHT BOARD_HEIGHT *CELL_SIZE
#define PLAYER_RADIUS SCREEN_HEIGHT * 0.05
#define PLAYER_SPEED 100
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

float c_sin(float x) {
  float result = 0.0f;
  float term = x;    // Starts at x^(2*0 + 1)
  float sign = 1.0f; // Tracks the alternating sign
  int factorial = 1; // Starts at factorial(1)

  for (int i = 0; i < TERMS; i++) {
    result += sign * term / factorial;
    sign = -sign;
    term *= x * x;
    factorial *= (2 * i + 2) * (2 * i + 3);
  }
  return result;
}

float c_cos(float x) {
  float result = 0.0f;
  float term = 1.0f; // Starts at x^(2*0)
  float sign = 1.0f;
  int factorial = 1; // Starts at factorial(0) which is 1

  for (int i = 0; i < TERMS; i++) {
    result += sign * term / factorial;
    sign = -sign;
    term *= x * x;
    if (i == 0) {
      factorial *= 1;
    }
    factorial *= (2 * i + 1) * (2 * i + 2);
  }
  return result;
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
} vector2_t;

vector2_t multiply_vector2(vector2_t vector, float scalar) {
  return (vector2_t){vector.x * scalar, vector.y * scalar};
}

vector2_t add_vector2(vector2_t vector1, vector2_t vector2) {
  return (vector2_t){vector1.x + vector2.x, vector1.y + vector2.y};
}

typedef struct {
  vector2_t position;
  vector2_t velocity;
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
  switch (player) {
  case ONE:
    return (color_t){0xFF, 0xAA, 0xAA, 0xFF};
  case TWO:
    return (color_t){0xAA, 0xFF, 0xAA, 0xFF};
  }
}

void update_frame(float delta) {
  player1.position =
      add_vector2(player1.position, multiply_vector2(player1.velocity, delta));
  player2.position =
      add_vector2(player2.position, multiply_vector2(player2.velocity, delta));

  clear_with_color(BACKGROUND_COLOR);

  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      fill_rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                get_player_color(board[y][x]));
    }
  }

  fill_circle(player1.position.x, player1.position.y, PLAYER_RADIUS,
              get_player_color(TWO));
  fill_circle(player2.position.x, player2.position.y, PLAYER_RADIUS,
              get_player_color(ONE));
}

int main(void) {
  set_canvas_size(SCREEN_WIDTH, SCREEN_HEIGHT);
  populate_board();

  player1.position = (vector2_t){SCREEN_WIDTH / 4.0, SCREEN_HEIGHT / 2.0};
  player1.velocity = multiply_vector2(
      (vector2_t){c_cos(PI * .25), c_sin(PI * .25)}, PLAYER_SPEED);
  player2.position =
      (vector2_t){SCREEN_WIDTH / 2.0 + SCREEN_WIDTH / 4.0, SCREEN_HEIGHT / 2.0};

  set_update_frame(update_frame);
}
