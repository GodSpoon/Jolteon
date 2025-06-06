#ifndef JOLTEON_H
#define JOLTEON_H

#include <Arduino.h>
#include <esp32_smartdisplay.h>

extern volatile bool sram_modified;
extern uint8_t btn_directions;
extern uint8_t btn_faces;

typedef uint16_t fbuffer_t;
extern uint16_t palette[];

// Game Boy button mapping
#define BTN_RIGHT  0x01
#define BTN_LEFT   0x02
#define BTN_UP     0x04
#define BTN_DOWN   0x08
#define BTN_A      0x01
#define BTN_B      0x02
#define BTN_SELECT 0x04
#define BTN_START  0x08

void jolteon_update(void);
void jolteon_init(void);
void jolteon_faint(const char* msg);
fbuffer_t* jolteon_get_framebuffer(void);
void jolteon_clear_framebuffer(fbuffer_t col);
void jolteon_end_frame(void);
void jolteon_clear_screen(uint16_t col);
void jolteon_set_palette(const uint32_t* col);
void jolteon_render_border(const uint8_t* img, uint32_t size);
void jolteon_save_sram(uint8_t* ram, uint32_t size);
void jolteon_load_sram(uint8_t* ram, uint32_t size);
const uint8_t* jolteon_load_rom(const char* path);
const uint8_t* jolteon_load_bootrom(const char* path);

#endif