// TFT_eSPI User_Setup.h for ESP32-2432S028R
// This file configures the TFT_eSPI library for the ILI9341 display

// Driver selection
#define ILI9341_DRIVER

// Display dimensions
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ESP32-2432S028R pin assignments
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   2   // Data Command control pin
#define TFT_RST  12  // Reset pin (could connect to RST pin)
#define TFT_BL   21  // LED back-light

// Touch controller pins
#define TOUCH_CS 33

// SPI frequency
#define SPI_FREQUENCY      40000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000

// Color depth
#define TFT_RGB_ORDER TFT_BGR  // Color order Blue-Green-Red

// Optional: Enable smooth fonts
#define SMOOTH_FONT

// Load fonts
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel high font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel high font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel high font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// VSPI port
#define USE_HSPI_PORT
