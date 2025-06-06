#include <esp32_smartdisplay.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_partition.h>

#include "jolteon.h"
#include "interrupt.h"
#include "mbc.h"
#include "rom.h"

#define PARTITION_ROM (esp_partition_subtype_t(0x40))
#define MAX_ROM_SIZE (2*1024*1024)

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

// Center Game Boy screen on 240x320 display
#define CENTER_X ((240 - GAMEBOY_WIDTH)  >> 1)
#define CENTER_Y ((320 - GAMEBOY_HEIGHT) >> 1)

// Scale factor for Game Boy display (1x, 2x, etc.)
#define SCALE_FACTOR 1

static fbuffer_t* pixels;

volatile int spi_lock = 0;
volatile bool sram_modified = false;

uint16_t palette[] = { 0xFFFF, 0xAAAA, 0x5555, 0x2222 };

// Touch button areas (virtual D-pad and buttons)
struct TouchButton {
    int x, y, w, h;
    uint8_t button_mask;
    bool is_direction;
};

static TouchButton touch_buttons[] = {
    // D-pad
    {20, 200, 40, 40, BTN_LEFT, true},     // Left
    {100, 200, 40, 40, BTN_RIGHT, true},   // Right  
    {60, 160, 40, 40, BTN_UP, true},       // Up
    {60, 240, 40, 40, BTN_DOWN, true},     // Down
    
    // Action buttons
    {160, 180, 35, 35, BTN_B, false},      // B
    {200, 160, 35, 35, BTN_A, false},      // A
    {140, 140, 30, 30, BTN_SELECT, false}, // Select
    {180, 140, 30, 30, BTN_START, false},  // Start
};

void jolteon_render_border(const uint8_t* img, uint32_t size)
{
    smartdisplay_lcd_clear(TFT_BLACK);
    
    // TODO: Add custom border support from SD card
    // For now, just clear to black
}

static void jolteon_request_sd_write()
{
    spi_lock = 1;
}

void jolteon_init(void)
{
    // Initialize the smart display
    smartdisplay_init();
    
    // Set brightness
    smartdisplay_lcd_set_backlight(0.5f); // 50%
    
    // Initialize SD card
    smartdisplay_init_sd();
    
    pixels = (fbuffer_t*)calloc(GAMEBOY_HEIGHT * GAMEBOY_WIDTH, sizeof(fbuffer_t));
    
    const uint32_t pal[] = {0xEFFFDE, 0xADD794, 0x525F73, 0x183442}; // Default greenscale palette
    jolteon_set_palette(pal);
    
    // Draw virtual controls
    jolteon_draw_controls();
}

void jolteon_draw_controls(void)
{
    // Draw virtual D-pad and buttons
    for (int i = 0; i < sizeof(touch_buttons) / sizeof(TouchButton); i++) {
        TouchButton* btn = &touch_buttons[i];
        
        // Draw button outline
        smartdisplay_lcd_draw_rect(btn->x, btn->y, btn->w, btn->h, TFT_WHITE);
        
        // Draw button labels
        smartdisplay_lcd_set_cursor(btn->x + btn->w/2 - 6, btn->y + btn->h/2 - 8);
        smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLACK);
        
        if (btn->is_direction) {
            switch (btn->button_mask) {
                case BTN_UP: smartdisplay_lcd_print("^"); break;
                case BTN_DOWN: smartdisplay_lcd_print("v"); break;
                case BTN_LEFT: smartdisplay_lcd_print("<"); break;
                case BTN_RIGHT: smartdisplay_lcd_print(">"); break;
            }
        } else {
            switch (btn->button_mask) {
                case BTN_A: smartdisplay_lcd_print("A"); break;
                case BTN_B: smartdisplay_lcd_print("B"); break;
                case BTN_SELECT: smartdisplay_lcd_print("SEL"); break;
                case BTN_START: smartdisplay_lcd_print("ST"); break;
            }
        }
    }
}

void jolteon_update(void)
{
    // Handle touch input
    if (smartdisplay_lcd_get_touch_count() > 0) {
        int16_t x, y;
        if (smartdisplay_lcd_get_touch_point(0, &x, &y)) {
            // Reset button states
            btn_directions = 0x0F;
            btn_faces = 0x0F;
            
            // Check which buttons are pressed
            for (int i = 0; i < sizeof(touch_buttons) / sizeof(TouchButton); i++) {
                TouchButton* btn = &touch_buttons[i];
                
                if (x >= btn->x && x <= btn->x + btn->w &&
                    y >= btn->y && y <= btn->y + btn->h) {
                    
                    if (btn->is_direction) {
                        btn_directions &= ~btn->button_mask;
                    } else {
                        btn_faces &= ~btn->button_mask;
                    }
                    
                    interrupt(INTR_JOYPAD);
                }
            }
        }
    } else {
        // No touch, all buttons released
        btn_directions = 0x0F;
        btn_faces = 0x0F;
    }
}

void jolteon_faint(const char* msg)
{
    smartdisplay_lcd_clear(TFT_BLACK);
    smartdisplay_lcd_set_cursor(2, 2);
    smartdisplay_lcd_set_text_color(TFT_RED, TFT_BLACK);
    smartdisplay_lcd_printf("Jolteon fainted!\n%s", msg);
    while(true) {
        delay(100);
    }
}

fbuffer_t* jolteon_get_framebuffer(void)
{
    return pixels;
}

void jolteon_clear_framebuffer(fbuffer_t col)
{
    memset(pixels, col, GAMEBOY_HEIGHT * GAMEBOY_WIDTH * sizeof(fbuffer_t));
}

void jolteon_clear_screen(uint16_t col)
{
    smartdisplay_lcd_clear(col);
}

void jolteon_set_palette(const uint32_t* col)
{
    /* RGB888 -> RGB565 */
    for (int i = 0; i < 4; ++i) {
        palette[i] = ((col[i]&0xFF)>>3)+((((col[i]>>8)&0xFF)>>2)<<5)+((((col[i]>>16)&0xFF)>>3)<<11);
    }
}

void jolteon_end_frame(void)
{
    if (spi_lock) {
        const s_rominfo* rominfo = rom_get_info();
        if (rominfo->has_battery && rom_get_ram_size())
            jolteon_save_sram(mbc_get_ram(), rom_get_ram_size());
        spi_lock = 0;
    }
    
    // Draw the Game Boy screen centered
    smartdisplay_lcd_draw_bitmap(CENTER_X, CENTER_Y, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, pixels);
}

void jolteon_save_sram(uint8_t* ram, uint32_t size)
{
    if (!ram) return;
    
    static char path[30];
    sprintf(path, "/%.8s.sav", rom_get_title());
    
    File sram = SD.open(path, FILE_WRITE);
    if (sram) {
        sram.seek(0);
        sram.write(ram, size);
        sram.close();
    }
}

void jolteon_load_sram(uint8_t* ram, uint32_t size)
{
    if (!ram) return;
    
    static char path[30];
    sprintf(path, "/%.8s.sav", rom_get_title());
    
    File sram = SD.open(path, FILE_READ);
    if (sram) {
        sram.seek(0);
        sram.read(ram, size);
        sram.close();
    }
}

const uint8_t* jolteon_load_bootrom(const char* path)
{
    static uint8_t bootrom[256];
    
    File bf = SD.open(path, FILE_READ);
    if (bf) {
        bf.seek(0);
        bf.read(bootrom, sizeof(bootrom));
        bf.close();
        return bootrom;
    }
    
    return nullptr;
}

static inline const uint8_t* jolteon_get_last_rom(const esp_partition_t* part)
{
    spi_flash_mmap_handle_t hrom;
    const uint8_t* romdata;
    esp_err_t err;
    err = esp_partition_mmap(part, 0, MAX_ROM_SIZE, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom);
    if (err != ESP_OK)
        return nullptr;
    return romdata;
}

const uint8_t* jolteon_load_rom(const char* path)
{
    const esp_partition_t* part;
    part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, PARTITION_ROM, NULL);
    if (!part)
        return nullptr;
    
    if (!path)
        return jolteon_get_last_rom(part);
    
    File romfile = SD.open(path, FILE_READ);
    if (!romfile)
        return nullptr;
    
    esp_err_t err;
    err = esp_partition_erase_range(part, 0, MAX_ROM_SIZE);
    if (err != ESP_OK)
        return nullptr;
    
    const size_t bufsize = 32 * 1024;
    size_t romsize = romfile.size();
    if (romsize > MAX_ROM_SIZE)
        return nullptr;
    
    uint8_t* rombuf = (uint8_t*)calloc(bufsize, 1);
    if (!rombuf)
        return nullptr;
    
    smartdisplay_lcd_clear(TFT_BLACK);
    smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLACK);
    smartdisplay_lcd_draw_string("Flashing ROM...", 0, 0);
    
    size_t offset = 0;
    while(romfile.available()) {
        romfile.read(rombuf, bufsize);
        esp_partition_write(part, offset, (const void*)rombuf, bufsize);
        offset += bufsize;
        
        // Draw progress bar
        int progress = (offset * 100) / romsize;
        smartdisplay_lcd_draw_rect(50, 100, 140, 20, TFT_WHITE);
        smartdisplay_lcd_fill_rect(52, 102, (progress * 136) / 100, 16, TFT_GREEN);
    }
    
    smartdisplay_lcd_clear(TFT_BLACK);
    free(rombuf);
    romfile.close();
    
    return jolteon_get_last_rom(part);
}