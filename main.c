typedef unsigned char uint8_t;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

const color_t BACKGROUND_COLOR = {0x18, 0x18, 0x18, 0xFF};

extern void set_canvas_size(int width, int height);
extern void clear_with_color(color_t color);
extern void fill_rect(int x, int y, int w, int h, color_t color);

int main(void) {
  set_canvas_size(800, 600);
  clear_with_color(BACKGROUND_COLOR);
  fill_rect(0, 0, 32, 32, (color_t){0xFF, 0, 0, 0xFF});
}
