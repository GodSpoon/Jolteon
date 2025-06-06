#include "timer.h"
#include "rom.h"
#include "mem.h"
#include "cpu.h"
#include "lcd.h"
#include "jolteon.h"
#include "menu.h"

#include "gbfiles.h"

void setup()
{
    Serial.begin(115200);
    
    jolteon_init();
    
    menu_init();
    menu_loop();
    
    const uint8_t* rom = jolteon_load_rom(menu_get_rompath());
    if (!rom) rom = (const uint8_t*)gb_rom;
    
    const uint8_t* bootrom = jolteon_load_bootrom("/gb_bios.bin");
    if (!bootrom) bootrom = (const uint8_t*)gb_bios;
    
    if (!rom_init(rom))
        jolteon_faint("rom_init failed.");
    
    if (!mmu_init(bootrom))
        jolteon_faint("mmu_init failed.");
    
    if (!lcd_init())
        jolteon_faint("lcd_init failed.");
    
    cpu_init();
    
    jolteon_render_border((const uint8_t*)gb_border, gb_border_size);
    
    // Show that we're starting
    jolteon_clear_screen(TFT_BLACK);
    smartdisplay_lcd_set_text_color(TFT_GREEN, TFT_BLACK);
    smartdisplay_lcd_set_cursor(60, 150);
    smartdisplay_lcd_print("Starting Game...");
    delay(1000);
    
    jolteon_clear_screen(TFT_BLACK);
    jolteon_draw_controls();
}

void loop()
{
    uint32_t cycles = cpu_cycle();
    jolteon_update();
    lcd_cycle(cycles);
    timer_cycle(cycles);
}