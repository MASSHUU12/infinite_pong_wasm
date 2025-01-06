typedef unsigned char uint8_t;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

extern void set_canvas_size(int width, int height);
extern void clear_with_color(color_t color);

int main(void) {
  set_canvas_size(800, 600);
  clear_with_color((color_t){0xAA, 0xBB, 0xCC, 0xDD});
}
