#include "jolteon.h"
#include "cpu.h"
#include "gbfiles.h"
#include "lcd.h"
#include "mbc.h"
#include "mem.h"
#include "menu.h"
#include "rom.h"
#include "timer.h"
#include <FS.h>
#include <SD.h>
#include <esp32_smartdisplay.h>

#define JOYPAD_INPUT 5
#define JOYPAD_ADDR 0x88

#define GETBIT(x, b) (((x) >> (b)) & 0x01)

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

#define CENTER_X ((smartdisplay_lcd_width() - GAMEBOY_WIDTH) >> 1)
#define CENTER_Y ((smartdisplay_lcd_height() - GAMEBOY_HEIGHT) >> 1)

static fbuffer_t* pixels;
static lv_obj_t* canvas = nullptr;
static lv_color_t* canvas_buf = nullptr;

volatile int spi_lock = 0;
volatile bool sram_modified = false;

uint16_t palette[] = { 0xFFFF, 0xAAAA, 0x5555, 0x2222 };

void jolteon_render_border(const uint8_t* img, uint32_t size)
{
    lv_obj_clean(lv_scr_act());

    if (img && size > 0) {
        // Try to display border image - simplified for now
        // Could implement JPEG decoding here if needed
    }
}

void jolteon_display_test_pattern()
{
    // Create a test pattern to verify display is working
    if (!canvas_buf) return;
    
    // Fill with color bars
    for (int y = 0; y < GAMEBOY_HEIGHT; y++) {
        for (int x = 0; x < GAMEBOY_WIDTH; x++) {
            uint16_t color;
            int segment = (x * 4) / GAMEBOY_WIDTH;
            
            switch (segment) {
                case 0: color = 0xF800; break; // Red
                case 1: color = 0x07E0; break; // Green  
                case 2: color = 0x001F; break; // Blue
                default: color = 0xFFFF; break; // White
            }
            
            canvas_buf[y * GAMEBOY_WIDTH + x] = lv_color_hex(color);
        }
    }
    
    if (canvas) {
        lv_canvas_set_buffer(canvas, canvas_buf, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, LV_COLOR_FORMAT_RGB565);
    }
    
    Serial.println("Display test pattern rendered");
}

bool jolteon_verify_system()
{
    Serial.println("=== Jolteon System Verification ===");
    bool all_ok = true;
    
    // Test 1: Display system
    Serial.print("Display canvas: ");
    if (canvas && canvas_buf) {
        Serial.println("OK");
    } else {
        Serial.println("FAILED");
        all_ok = false;
    }
    
    // Test 2: Framebuffer
    Serial.print("Framebuffer: ");
    if (pixels) {
        Serial.println("OK");
    } else {
        Serial.println("FAILED");
        all_ok = false;
    }
    
    // Test 3: SD card
    Serial.print("SD card: ");
    if (SD.begin()) {
        Serial.println("OK");
    } else {
        Serial.println("FAILED - Insert SD card or check connections");
        all_ok = false;
    }
    
    // Test 4: LVGL
    Serial.print("LVGL: ");
    if (lv_is_initialized()) {
        Serial.println("OK");
    } else {
        Serial.println("FAILED");
        all_ok = false;
    }
    
    Serial.printf("Overall system status: %s\n", all_ok ? "READY" : "ISSUES DETECTED");
    Serial.println("=====================================");
    
    return all_ok;
}

static void jolteon_request_sd_write()
{
    spi_lock = 1;
}

void jolteon_init(void)
{
    smartdisplay_init();

    // Initialize SD card
    if (!SD.begin()) {
        Serial.println("SD card initialization failed");
    }

    pixels = (fbuffer_t*)calloc(GAMEBOY_HEIGHT * GAMEBOY_WIDTH, sizeof(fbuffer_t));

    // Create canvas for Game Boy display
    canvas = lv_canvas_create(lv_scr_act());
    canvas_buf = (lv_color_t*)lv_malloc(GAMEBOY_WIDTH * GAMEBOY_HEIGHT * sizeof(lv_color_t));

    if (canvas_buf) {
        lv_canvas_set_buffer(canvas, canvas_buf, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, LV_COLOR_FORMAT_RGB565);
        lv_obj_center(canvas);
    }

    const uint32_t pal[] = { 0xEFFFDE, 0xADD794, 0x525F73, 0x183442 }; // Default greenscale palette
    jolteon_set_palette(pal);
}

void jolteon_update(void)
{
    // Handle input - simplified version
    // You may need to implement touch or button input based on your hardware

    // For touch input:
    lv_indev_t* indev = lv_indev_get_next(nullptr);
    if (indev && lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER) {
        // Read touch state - LVGL v9 simplified API
        lv_indev_read(indev);

        // Process touch input for joypad emulation
        // This is a placeholder - implement based on your UI design
    }
}

void jolteon_faint(const char* msg)
{
    lv_obj_clean(lv_scr_act());

    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(label, "Jolteon fainted!\n%s", msg);
    lv_obj_center(label);

    while (true) {
        lv_timer_handler();
        delay(5);
    }
}

fbuffer_t* jolteon_get_framebuffer(void)
{
    return pixels;
}

void jolteon_clear_framebuffer(fbuffer_t col)
{
    for (int i = 0; i < GAMEBOY_WIDTH * GAMEBOY_HEIGHT; i++) {
        pixels[i] = col;
    }
}

void jolteon_clear_screen(uint16_t col)
{
    if (canvas_buf) {
        lv_color_t lv_col = lv_color_hex(col);
        for (int i = 0; i < GAMEBOY_WIDTH * GAMEBOY_HEIGHT; i++) {
            canvas_buf[i] = lv_col;
        }
    }
}

void jolteon_set_palette(const uint32_t* col)
{
    /* RGB888 -> RGB565 */
    for (int i = 0; i < 4; ++i) {
        palette[i] = ((col[i] & 0xFF) >> 3) + ((((col[i] >> 8) & 0xFF) >> 2) << 5) + ((((col[i] >> 16) & 0xFF) >> 3) << 11);
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

    // Convert framebuffer to LVGL canvas
    if (canvas_buf && pixels) {
        for (int i = 0; i < GAMEBOY_WIDTH * GAMEBOY_HEIGHT; i++) {
            canvas_buf[i] = lv_color_hex(pixels[i]);
        }

        if (canvas) {
            lv_canvas_set_buffer(canvas, canvas_buf, GAMEBOY_WIDTH, GAMEBOY_HEIGHT, LV_COLOR_FORMAT_RGB565);
        }
    }
}

void jolteon_save_sram(uint8_t* ram, uint32_t size)
{
    if (!ram)
        return;

    static char path[30];
    sprintf(path, "/%.8s.bin", rom_get_title());

    File sram = SD.open(path, FILE_WRITE);
    if (sram) {
        sram.seek(0);
        sram.write(ram, size);
        sram.close();
    }
}

void jolteon_load_sram(uint8_t* ram, uint32_t size)
{
    if (!ram)
        return;

    static char path[30];
    sprintf(path, "/%.8s.bin", rom_get_title());

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

const uint8_t* jolteon_load_rom(const char* path)
{
    // For now, return internal ROM or load from SPIFFS
    // ROM loading from SD would need more complex implementation
    if (!path) {
        return (const uint8_t*)gb_rom;
    }

    // Simplified - in full implementation, you'd load ROM to flash
    File romfile = SD.open(path, FILE_READ);
    if (!romfile) {
        return (const uint8_t*)gb_rom;
    }

    romfile.close();
    return (const uint8_t*)gb_rom; // Placeholder
}
