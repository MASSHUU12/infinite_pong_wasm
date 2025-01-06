typedef unsigned char uint8_t;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

typedef void (*func_ptr)(float);

const color_t BACKGROUND_COLOR = {0x18, 0x18, 0x18, 0xFF};

extern void set_canvas_size(int width, int height);
extern void clear_with_color(color_t color);
extern void fill_rect(int x, int y, int w, int h, color_t color);
extern void set_update_frame(func_ptr f);

int x = 0, y = 0;

void update_frame(float delta) {
  clear_with_color(BACKGROUND_COLOR);

  x += 100 * delta;
  y += 100 * delta;

  fill_rect(x, y, 32, 32, (color_t){0xFF, 0, 0, 0xFF});
}

int main(void) {
  set_canvas_size(800, 600);
  set_update_frame(update_frame);
}
