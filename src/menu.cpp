#include "menu.h"
#include "jolteon.h"
#include <FS.h>
#include <SD.h>
#include <TFT_eSPI.h>
#include <vector>

extern TFT_eSPI tft;

static std::vector<String> rom_files;
static String rompath = "";
static const char* rompath_c = nullptr;
static bool running = true;
static int selected_index = 0;
static int scroll_offset = 0;

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
    // Clear screen with black background
    tft.fillScreen(TFT_BLACK);
    
    // Draw title
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Jolteon v1.0", 10, 10);
    
    // Draw ROM selection info
    tft.setTextSize(1);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Game Boy Emulator", 10, 40);
    tft.drawString("ESP32-2432S028R", 10, 55);
    
    // For Phase I, show simple menu
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("Phase I - Foundation", 10, 80);
    
    if (rom_files.size() > 0) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString("ROMs found:", 10, 110);
        
        // Show first few ROM files
        int y_pos = 130;
        for (int i = 0; i < min((int)rom_files.size(), 8); i++) {
            if (i == selected_index) {
                tft.setTextColor(TFT_BLACK, TFT_WHITE);
                tft.fillRect(5, y_pos - 2, 230, 16, TFT_WHITE);
            } else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.drawString(rom_files[i].c_str(), 10, y_pos);
            y_pos += 18;
        }
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("No ROMs found on SD", 10, 110);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString("Insert SD card with", 10, 130);
        tft.drawString(".gb or .gbc files", 10, 145);
    }
    
    // Instructions
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("Touch to select", 10, 280);
    tft.drawString("Phase I Test Mode", 10, 295);
}

const char* menu_get_rompath()
{
    return rompath_c;
}

void menu_init()
{
    Serial.println("Initializing menu system...");
    
    // Scan for ROM files on SD card
    scan_rom_files();
    
    Serial.printf("Found %d ROM files\n", rom_files.size());
    
    // For Phase I, set a default ROM path if available
    if (rom_files.size() > 0) {
        rompath = String("/") + rom_files[0];
        rompath_c = rompath.c_str();
        Serial.printf("Default ROM selected: %s\n", rompath_c);
    }
}

void menu_loop()
{
    Serial.println("Starting menu loop (Phase I simplified)...");
    
    // Draw the menu
    draw_menu();
    
    // For Phase I, just show the menu for a few seconds then continue
    // In later phases, this would handle touch input and user selection
    delay(3000);
    
    // Simple selection logic for Phase I testing
    if (rom_files.size() > 0) {
        Serial.printf("Phase I: Using ROM: %s\n", rompath_c);
    } else {
        Serial.println("Phase I: No ROMs found, will use built-in test ROM");
        rompath_c = nullptr;
    }
    
    running = false;
    Serial.println("Menu loop completed");
}
