#include <vector>
#include <esp32_smartdisplay.h>
#include "menu.h"

static String rompath = "";
static const char* rompath_c = nullptr;
static bool running;
static std::vector<String> rom_files;
static int selected_item = 0;
static int menu_state = 0; // 0 = main menu, 1 = rom list

struct MenuItem {
    String title;
    int type; // 0 = action, 1 = rom file
};

static std::vector<MenuItem> menu_items;

void scan_rom_files()
{
    rom_files.clear();
    
    File root = SD.open("/");
    if (!root) return;
    
    File file = root.openNextFile();
    while (file) {
        String filename = file.name();
        if (filename.endsWith(".gb") || filename.endsWith(".gbc")) {
            rom_files.push_back(filename);
        }
        file = root.openNextFile();
    }
    root.close();
}

void draw_menu()
{
    smartdisplay_lcd_clear(TFT_BLACK);
    smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLACK);
    smartdisplay_lcd_set_text_size(2);
    
    // Title
    smartdisplay_lcd_set_cursor(50, 20);
    smartdisplay_lcd_print("Jolteon v1.0");
    
    smartdisplay_lcd_set_text_size(1);
    
    if (menu_state == 0) {
        // Main menu
        menu_items.clear();
        menu_items.push_back({"Last ROM (Flash)", 0});
        menu_items.push_back({"Load ROM (SD Card)", 0});
        menu_items.push_back({"Settings (TODO)", 0});
        
        smartdisplay_lcd_set_cursor(20, 60);
        smartdisplay_lcd_print("Main Menu:");
        
        for (int i = 0; i < menu_items.size(); i++) {
            int y = 90 + i * 30;
            
            if (i == selected_item) {
                smartdisplay_lcd_fill_rect(10, y - 5, 220, 25, TFT_BLUE);
                smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLUE);
            } else {
                smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLACK);
            }
            
            smartdisplay_lcd_set_cursor(20, y);
            smartdisplay_lcd_print(menu_items[i].title);
        }
    } else if (menu_state == 1) {
        // ROM list
        smartdisplay_lcd_set_cursor(20, 60);
        smartdisplay_lcd_print("Select ROM:");
        
        if (rom_files.size() == 0) {
            smartdisplay_lcd_set_cursor(20, 90);
            smartdisplay_lcd_print("No ROM files found!");
            smartdisplay_lcd_set_cursor(20, 110);
            smartdisplay_lcd_print("Touch screen to go back");
            return;
        }
        
        int start_idx = max(0, selected_item - 5);
        int end_idx = min((int)rom_files.size(), start_idx + 6);
        
        for (int i = start_idx; i < end_idx; i++) {
            int y = 90 + (i - start_idx) * 25;
            
            if (i == selected_item) {
                smartdisplay_lcd_fill_rect(10, y - 3, 220, 20, TFT_BLUE);
                smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLUE);
            } else {
                smartdisplay_lcd_set_text_color(TFT_WHITE, TFT_BLACK);
            }
            
            smartdisplay_lcd_set_cursor(20, y);
            String display_name = rom_files[i];
            if (display_name.length() > 25) {
                display_name = display_name.substring(0, 22) + "...";
            }
            smartdisplay_lcd_print(display_name);
        }
        
        // Draw scroll indicator
        if (rom_files.size() > 6) {
            int scroll_height = (6 * 150) / rom_files.size();
            int scroll_pos = (selected_item * (150 - scroll_height)) / (rom_files.size() - 1);
            
            smartdisplay_lcd_draw_rect(230, 90, 5, 150, TFT_GRAY);
            smartdisplay_lcd_fill_rect(230, 90 + scroll_pos, 5, scroll_height, TFT_WHITE);
        }
    }
    
    // Instructions
    smartdisplay_lcd_set_text_color(TFT_YELLOW, TFT_BLACK);
    smartdisplay_lcd_set_cursor(10, 280);
    smartdisplay_lcd_print("Touch screen to navigate");
}

void handle_touch(int16_t x, int16_t y)
{
    if (menu_state == 0) {
        // Main menu touch handling
        for (int i = 0; i < menu_items.size(); i++) {
            int item_y = 90 + i * 30;
            if (y >= item_y - 5 && y <= item_y + 20) {
                selected_item = i;
                
                switch (i) {
                    case 0: // Last ROM
                        rompath_c = nullptr;
                        running = false;
                        break;
                    case 1: // Load ROM
                        scan_rom_files();
                        if (rom_files.size() > 0) {
                            menu_state = 1;
                            selected_item = 0;
                            draw_menu();
                        }
                        break;
                    case 2: // Settings
                        // TODO: Implement settings menu
                        break;
                }
                break;
            }
        }
    } else if (menu_state == 1) {
        // ROM list touch handling
        if (rom_files.size() == 0) {
            // Go back to main menu
            menu_state = 0;
            selected_item = 0;
            draw_menu();
            return;
        }
        
        // Touch on ROM list
        int start_idx = max(0, selected_item - 5);
        
        for (int i = 0; i < min(6, (int)rom_files.size()); i++) {
            int item_y = 90 + i * 25;
            if (y >= item_y - 3 && y <= item_y + 17) {
                int rom_idx = start_idx + i;
                if (rom_idx < rom_files.size()) {
                    rompath = "/" + rom_files[rom_idx];
                    rompath_c = rompath.c_str();
                    running = false;
                }
                break;
            }
        }
        
        // Touch scroll area
        if (x >= 220 && y >= 90 && y <= 240 && rom_files.size() > 6) {
            int relative_y = y - 90;
            selected_item = (relative_y * rom_files.size()) / 150;
            selected_item = constrain(selected_item, 0, rom_files.size() - 1);
            draw_menu();
        }
        
        // Touch to go back (outside menu area)
        if (x < 10 || x > 230 || y < 60) {
            menu_state = 0;
            selected_item = 0;
            draw_menu();
        }
    }
}

const char* menu_get_rompath()
{
    return rompath_c;
}

void menu_init()
{
    running = true;
    menu_state = 0;
    selected_item = 0;
    draw_menu();
}

void menu_loop()
{
    while(running) {
        if (smartdisplay_lcd_get_touch_count() > 0) {
            int16_t x, y;
            if (smartdisplay_lcd_get_touch_point(0, &x, &y)) {
                handle_touch(x, y);
                draw_menu();
                
                // Debounce
                delay(200);
                while (smartdisplay_lcd_get_touch_count() > 0) {
                    delay(10);
                }
            }
        }
        delay(50);
    }
}