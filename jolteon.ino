#include "cpu.h"
#include "gbfiles.h"
#include "jolteon.h"
#include "lcd.h"
#include "mem.h"
#include "menu.h"
#include "rom.h"
#include "timer.h"
#include <esp32_smartdisplay.h>

void setup()
{
    Serial.begin(115200);
    Serial.println("Jolteon starting...");

    jolteon_init();

    menu_init();
    menu_loop();

    const uint8_t* rom = jolteon_load_rom(menu_get_rompath());
    if (!rom)
        rom = (const uint8_t*)gb_rom;

    const uint8_t* bootrom = jolteon_load_bootrom("/gb_bios.bin");
    if (!bootrom)
        bootrom = (const uint8_t*)gb_bios;

    if (!rom_init(rom))
        jolteon_faint("rom_init failed.");

    if (!mmu_init(bootrom))
        jolteon_faint("mmu_init failed.");

    if (!lcd_init())
        jolteon_faint("lcd_init failed.");

    cpu_init();

    jolteon_render_border((const uint8_t*)gb_border, gb_border_size);

    Serial.println("Jolteon initialized, starting emulation...");
}

void loop()
{
    uint32_t cycles = cpu_cycle();
    jolteon_update();
    lcd_cycle(cycles);
    timer_cycle(cycles);

    // Handle LVGL tasks
    lv_timer_handler();
}
