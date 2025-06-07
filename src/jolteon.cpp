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
#include <TFT_eSPI.h>

#define JOYPAD_INPUT 5
#define JOYPAD_ADDR 0x88

#define GETBIT(x, b) (((x) >> (b)) & 0x01)

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

// Display scaling: 160x144 → 240x320
#define SCALE_X 1.5f
#define SCALE_Y 2.222f

// Center the Game Boy display
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define CENTER_X ((DISPLAY_WIDTH - (GAMEBOY_WIDTH * (int)SCALE_X)) >> 1)
#define CENTER_Y ((DISPLAY_HEIGHT - (GAMEBOY_HEIGHT * (int)SCALE_Y)) >> 1)

TFT_eSPI tft = TFT_eSPI();
static uint16_t* framebuffer = nullptr;
static uint16_t* display_buffer = nullptr;

volatile int spi_lock = 0;
volatile bool sram_modified = false;

// Game Boy green palette (classic colors)
uint16_t palette[] = { 0xE7FF, 0xAD75, 0x5ACB, 0x318C };

void jolteon_render_border(const uint8_t* img, uint32_t size)
{
    // Clear screen with black
    tft.fillScreen(TFT_BLACK);
    
    if (img && size > 0) {
        // Border implementation would go here
        // For now, just draw a simple frame around the game area
        int game_w = GAMEBOY_WIDTH * (int)SCALE_X;
        int game_h = GAMEBOY_HEIGHT * (int)SCALE_Y;
        tft.drawRect(CENTER_X - 2, CENTER_Y - 2, game_w + 4, game_h + 4, TFT_WHITE);
    }
}

void jolteon_display_test_pattern()
{
    Serial.println("Displaying TFT_eSPI test pattern...");
    
    // Clear screen
    tft.fillScreen(TFT_BLACK);
    
    // Draw color bars
    int bar_width = DISPLAY_WIDTH / 4;
    tft.fillRect(0, 0, bar_width, DISPLAY_HEIGHT, TFT_RED);
    tft.fillRect(bar_width, 0, bar_width, DISPLAY_HEIGHT, TFT_GREEN);
    tft.fillRect(bar_width * 2, 0, bar_width, DISPLAY_HEIGHT, TFT_BLUE);
    tft.fillRect(bar_width * 3, 0, bar_width, DISPLAY_HEIGHT, TFT_WHITE);
    
    // Draw text
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Jolteon", 10, 10);
    tft.drawString("TFT Test", 10, 40);
    
    Serial.println("Test pattern displayed");
}

bool jolteon_verify_system()
{
    Serial.println("=== Jolteon System Verification ===");
    bool all_ok = true;
    
    // Test 1: TFT Display
    Serial.print("TFT Display: ");
    tft.fillScreen(TFT_BLACK);
    delay(100);
    Serial.println("OK");
    
    // Test 2: Framebuffer
    Serial.print("Framebuffer: ");
    if (framebuffer && display_buffer) {
        Serial.println("OK");
    } else {
        Serial.println("FAILED");
        all_ok = false;
    }
    
    // Test 3: SD card
    Serial.print("SD card: ");
    if (SD.begin(5)) {  // CS pin 5 for ESP32-2432S028R
        Serial.println("OK");
    } else {
        Serial.println("FAILED - Insert SD card or check connections");
        all_ok = false;
    }
    
    // Test 4: Display dimensions
    Serial.printf("Display: %dx%d\n", tft.width(), tft.height());
    
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
    Serial.println("Initializing Jolteon with TFT_eSPI...");
    
    // Initialize TFT display
    tft.init();
    tft.setRotation(0);  // Portrait mode
    tft.fillScreen(TFT_BLACK);
    
    // Turn on backlight
    pinMode(21, OUTPUT);  // TFT_BL pin
    digitalWrite(21, HIGH);
    
    // Initialize SD card
    pinMode(5, OUTPUT);  // SD_CS pin
    if (!SD.begin(5)) {
        Serial.println("SD card initialization failed");
    } else {
        Serial.println("SD card initialized successfully");
    }
    
    // Allocate framebuffers
    framebuffer = (uint16_t*)calloc(GAMEBOY_HEIGHT * GAMEBOY_WIDTH, sizeof(uint16_t));
    display_buffer = (uint16_t*)calloc(DISPLAY_HEIGHT * DISPLAY_WIDTH, sizeof(uint16_t));
    
    if (!framebuffer || !display_buffer) {
        Serial.println("ERROR: Failed to allocate framebuffers!");
        return;
    }
    
    // Set default Game Boy palette (classic green)
    const uint32_t pal[] = { 0x9BBB0F, 0x8BAC0F, 0x306230, 0x0F380F };
    jolteon_set_palette(pal);
    
    Serial.println("Jolteon TFT_eSPI initialization complete");
}

void jolteon_update(void)
{
    // Handle input - placeholder for now
    // This would be where we read touch input or buttons
    // For the ESP32-2432S028R, we could read touch via XPT2046
}

void jolteon_faint(const char* msg)
{
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextSize(2);
    tft.drawString("Jolteon Fainted!", 10, 50);
    tft.setTextSize(1);
    tft.drawString(msg, 10, 100);
    
    Serial.printf("FATAL ERROR: %s\n", msg);
    
    while (true) {
        delay(1000);
    }
}

fbuffer_t* jolteon_get_framebuffer(void)
{
    return framebuffer;
}

void jolteon_clear_framebuffer(fbuffer_t col)
{
    if (framebuffer) {
        for (int i = 0; i < GAMEBOY_WIDTH * GAMEBOY_HEIGHT; i++) {
            framebuffer[i] = col;
        }
    }
}

void jolteon_clear_screen(uint16_t col)
{
    tft.fillScreen(col);
    if (framebuffer) {
        for (int i = 0; i < GAMEBOY_WIDTH * GAMEBOY_HEIGHT; i++) {
            framebuffer[i] = col;
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

    // Render Game Boy framebuffer to TFT display with scaling
    if (framebuffer) {
        // Calculate scaling factors
        int scale_x = DISPLAY_WIDTH / GAMEBOY_WIDTH;   // 240/160 = 1
        int scale_y = DISPLAY_HEIGHT / GAMEBOY_HEIGHT; // 320/144 = 2
        
        // Use integer scaling for performance
        if (scale_x < 1) scale_x = 1;
        if (scale_y < 1) scale_y = 1;
        
        int scaled_width = GAMEBOY_WIDTH * scale_x;
        int scaled_height = GAMEBOY_HEIGHT * scale_y;
        int offset_x = (DISPLAY_WIDTH - scaled_width) / 2;
        int offset_y = (DISPLAY_HEIGHT - scaled_height) / 2;
        
        // Simple pixel-perfect scaling
        for (int y = 0; y < GAMEBOY_HEIGHT; y++) {
            for (int x = 0; x < GAMEBOY_WIDTH; x++) {
                uint16_t pixel = framebuffer[y * GAMEBOY_WIDTH + x];
                
                // Draw scaled pixel block
                for (int sy = 0; sy < scale_y; sy++) {
                    for (int sx = 0; sx < scale_x; sx++) {
                        int screen_x = offset_x + (x * scale_x) + sx;
                        int screen_y = offset_y + (y * scale_y) + sy;
                        
                        if (screen_x >= 0 && screen_x < DISPLAY_WIDTH && 
                            screen_y >= 0 && screen_y < DISPLAY_HEIGHT) {
                            tft.drawPixel(screen_x, screen_y, pixel);
                        }
                    }
                }
            }
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
