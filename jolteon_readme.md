![Jolteon](/logo/jolteon.svg)

# Jolteon - Game Boy Emulator for ESP32

Jolteon is a Game Boy emulator specifically designed for the ESP32_2432S028Rv3 "Cheap Yellow Display" board. This project is a port of the original [Espeon emulator](https://github.com/Ryuzaki-MrL/m5stack-gameboy), optimized for the affordable CYD development board.

## About Jolteon

This emulator takes advantage of the ESP32's dual-core architecture and provides a touch-based Game Boy gaming experience on an affordable 2.8" display. The project is designed specifically for the ESP32_2432S028Rv3 board but can be adapted to other ESP32 displays.

### Hardware Requirements

**Recommended setup:**
* ESP32_2432S028Rv3 "Cheap Yellow Display" board
* 240x320 ST7789 display (built-in)
* Touch screen interface (XPT2046, built-in)
* SD card for ROM storage (built-in slot)
* USB-C for programming and power

### Key Features

* **Game Boy CPU Emulation**: Full LR35902 CPU emulation
* **Touch Controls**: Virtual D-pad and buttons on screen
* **SD Card Support**: Load ROMs directly from SD card
* **Save States**: SRAM saves automatically to SD card
* **Dual Core**: Takes advantage of ESP32's dual-core architecture
* **Memory Bank Controllers**: Support for MBC1, MBC2, and MBC3
* **Compact Design**: Perfect for portable gaming

## Getting Started

### Building with PlatformIO

1. **Install PlatformIO**: Follow the [PlatformIO installation guide](https://platformio.org/install)

2. **Clone the repository**:
   ```bash
   git clone [your-repo-url]
   cd jolteon
   ```

3. **Install dependencies**: PlatformIO will automatically install the required esp32-smartdisplay library

4. **Build and upload**:
   ```bash
   pio run --target upload
   ```

### Building with Arduino IDE

1. **Install ESP32 board support** in Arduino IDE
2. **Install required libraries**:
   - esp32-smartdisplay
3. **Open `jolteon.ino`** in Arduino IDE
4. **Select board**: "ESP32 Dev Module" or custom ESP32_2432S028Rv3 board
5. **Upload** to your device

### Configuration

The project uses the ESP32_2432S028Rv3 board configuration by default. You can modify `platformio.ini` to target other boards if needed.

## Usage

### Loading ROMs

1. **Prepare SD Card**: Format a micro SD card (FAT32 recommended)
2. **Add ROM files**: Copy Game Boy ROM files (.gb, .gbc) to the SD card root
3. **Insert SD card**: Place the SD card in the CYD board's slot
4. **Power on**: The device will show a touch menu

### Touch Controls

The emulator displays virtual controls on the bottom portion of the screen:

- **D-Pad**: Left side touch area for directional input
- **A/B Buttons**: Right side for action buttons  
- **Select/Start**: Smaller buttons for menu functions

### Menu Navigation

- Touch any menu item to select it
- **"Last ROM (Flash)"**: Run the previously flashed ROM
- **"Load ROM (SD Card)"**: Browse and select a ROM from SD card
- Touch ROM files to select and load them

### Save Data

Game saves are automatically stored on the SD card as `.sav` files, named after the ROM title.

## Development

### Project Structure

```
jolteon/
├── src/
│   ├── jolteon.cpp          # Main emulator interface
│   ├── cpu.cpp              # Game Boy CPU emulation
│   ├── mem.cpp              # Memory management
│   ├── lcd.cpp              # Display and graphics
│   ├── mbc.cpp              # Memory bank controllers
│   ├── menu.cpp             # Touch menu system
│   └── ...
├── include/
│   ├── jolteon.h
│   └── ...
├── platformio.ini           # PlatformIO configuration
└── README.md
```

### Key Components

- **CPU Core**: Accurate LR35902 instruction set implementation
- **Memory Management**: Full Game Boy memory map with banking
- **LCD Controller**: Pixel-perfect Game Boy display emulation
- **Input System**: Touch-based virtual controls
- **File System**: SD card ROM and save management

## Performance

- **CPU Speed**: Runs at or above original Game Boy speed
- **Display**: 60 FPS refresh rate on 240x320 display
- **Compatibility**: Supports most Game Boy and Game Boy Color titles
- **Memory**: Efficiently uses ESP32's limited RAM

## Troubleshooting

### Common Issues

1. **No ROMs detected**: Ensure SD card is formatted as FAT32 and ROM files have .gb or .gbc extension
2. **Touch not responsive**: Check touch calibration in board configuration
3. **Black screen**: Verify display connections and power supply
4. **Slow performance**: Ensure proper CPU speed settings in platformio.ini

### Debug Output

Enable serial output at 115200 baud for debugging information:
```cpp
Serial.begin(115200);
```

## Contributing

Contributions are welcome! Areas for improvement:

- **Audio Emulation**: Add Game Boy sound processing
- **Enhanced Controls**: Physical button support
- **Save States**: Full state save/load functionality  
- **Color Palettes**: Custom color scheme support
- **Performance**: Further optimization

## Credits

- **Original Espeon**: Code structure inspired by [Espeon](https://github.com/Ryuzaki-MrL/m5stack-gameboy)
- **Base emulation**: CPU core inspired by [zid's gameboy emulator](https://github.com/zid)
- **Hardware support**: Built with [esp32-smartdisplay](https://github.com/rzeldent/esp32-smartdisplay)
- **Jolteon artwork**: Inspired by Pokémon's electric-type evolution

## License

This project follows the same license terms as the original Espeon emulator. Please respect ROM copyrights and only use ROMs you legally own.

## Hardware Links

- [ESP32_2432S028Rv3 Board](https://www.aliexpress.com/item/1005004502250619.html) - The "Cheap Yellow Display"
- [Alternative suppliers](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display) - More info on CYD boards

---

**Disclaimer**: This emulator is for educational purposes. Ensure you own the rights to any ROM files you use with this emulator.