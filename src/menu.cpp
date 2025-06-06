#include "menu.h"
#include <FS.h>
#include <SD.h>
#include <esp32_smartdisplay.h>
#include <vector>

static std::vector<String> rom_files;
static String rompath = "";
static const char* rompath_c = nullptr;
static bool running = true;
static int selected_index = 0;
static int scroll_offset = 0;

// LVGL objects
static lv_obj_t* main_screen = nullptr;
static lv_obj_t* list = nullptr;

void scan_rom_files()
{
    rom_files.clear();

    File root = SD.open("/");
    if (!root)
        return;

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
    // Clear screen
    lv_obj_clean(lv_scr_act());

    // Create main container
    main_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_screen, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(main_screen, lv_color_black(), 0);

    // Title
    lv_obj_t* title = lv_label_create(main_screen);
    lv_label_set_text(title, "Jolteon v1.0");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Create list
    list = lv_list_create(main_screen);
    lv_obj_set_size(list, LV_PCT(90), LV_PCT(70));
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 20);

    // Add default options
    lv_obj_t* btn_last_rom = lv_list_add_btn(list, nullptr, "Last ROM (Flash)");
    lv_obj_add_event_cb(btn_last_rom, [](lv_event_t* e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            rompath_c = nullptr;
            running = false;
        } }, LV_EVENT_CLICKED, nullptr);

    // Add ROM files from SD card
    for (const String& filename : rom_files) {
        lv_obj_t* btn = lv_list_add_btn(list, nullptr, filename.c_str());

        // Store filename in user data
        char* stored_name = (char*)lv_mem_alloc(filename.length() + 1);
        strcpy(stored_name, filename.c_str());
        lv_obj_set_user_data(btn, stored_name);

        lv_obj_add_event_cb(btn, [](lv_event_t* e) {
            if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
                lv_obj_t* btn = lv_event_get_target(e);
                char* filename = (char*)lv_obj_get_user_data(btn);
                if (filename) {
                    rompath = String("/") + filename;
                    rompath_c = rompath.c_str();
                    running = false;
                }
            } }, LV_EVENT_CLICKED, nullptr);
    }

    // Add settings option
    lv_obj_t* btn_settings = lv_list_add_btn(list, nullptr, "Settings (TODO)");
    lv_obj_add_event_cb(btn_settings, [](lv_event_t* e) {
        // TODO: Implement settings
    },
        LV_EVENT_CLICKED, nullptr);

    // Instructions
    lv_obj_t* instructions = lv_label_create(main_screen);
    lv_label_set_text(instructions, "Touch to select ROM");
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xFFFF00), 0);
    lv_obj_align(instructions, LV_ALIGN_BOTTOM_MID, 0, -10);
}

const char* menu_get_rompath()
{
    return rompath_c;
}

void menu_init()
{
    // Scan for ROM files on SD card
    scan_rom_files();

    // Create menu UI
    draw_menu();

    running = true;
}

void menu_loop()
{
    while (running) {
        lv_timer_handler();
        delay(5);
    }
}
