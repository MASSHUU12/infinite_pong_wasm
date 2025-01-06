typedef unsigned long size_t;
typedef unsigned char uint8_t;

void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    for (size_t i = 0; i < len; i++) {
        ptr[i] = (unsigned char)val;
    }
    return dest;
}

#define BOARD_HEIGHT 32
#define BOARD_WIDTH BOARD_HEIGHT * 2
#define CELL_SIZE 20
#define SCREEN_WIDTH BOARD_WIDTH *CELL_SIZE
#define SCREEN_HEIGHT BOARD_HEIGHT *CELL_SIZE
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

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

void populate_board() {
  for (int x = 0; x < BOARD_WIDTH/2; ++x) {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
      board[y][x] = ONE;
      board[y][x + BOARD_WIDTH/2] = TWO;
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
  clear_with_color(BACKGROUND_COLOR);

  for (int y = 0; y < BOARD_HEIGHT; ++y) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
      fill_rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                get_player_color(board[y][x]));
    }
  }
}

int main(void) {
  set_canvas_size(SCREEN_WIDTH, SCREEN_HEIGHT);
  populate_board();
  set_update_frame(update_frame);
}
