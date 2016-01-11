
typedef struct
{
  uint8_t width;
  uint8_t height;
  uint8_t data[];
} bitmap_t;

enum
{
  GFX_DM_JAM1=0x07,
  GFX_DM_JAM2=0x03,
  GFX_DM_COMP=0x06,
  GFX_DM_JAM1_INV=0x08,
  GFX_DM_JAM2_INV=0x0c,
  GFX_DM_COMP_INV=0x09
};

uint8_t gfx_get_x();
uint8_t gfx_get_y();
void gfx_move(uint8_t x, uint8_t y);
void gfx_draw_mode(uint8_t mode);


void gfx_fill(uint8_t val);
uint8_t gfx_char_width(char ch, uint8_t prop);
uint8_t gfx_text_width(char *txt, uint8_t prop);
void gfx_print_char(char c, uint8_t prop);
void gfx_print_text(char *txt, uint8_t prop);

void gfx_draw_bitmap(bitmap_t * bmp);


