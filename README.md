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

# Jolteon - Game Boy Emulator for ESP32

Jolteon is a Game Boy emulator specifically designed for the ESP32-2432S028R "Cheap Yellow Display" board. This project is a comprehensive port of the original [Espeon emulator](https://github.com/Ryuzaki-MrL/Espeon), completely redesigned for the affordable CYD development board using modern PlatformIO development practices.

## About Jolteon

This emulator leverages the ESP32's dual-core architecture to deliver an authentic Game Boy gaming experience on a 2.8" touchscreen display. The project implements a full-featured emulation platform with advanced touch controls, comprehensive file management, and hardware integration optimized for the ESP32-2432S028R board.

### Hardware Requirements

**Target Platform:**
* ESP32-2432S028R "Cheap Yellow Display" board
* 240x320 ILI9341 TFT display (integrated)
* XPT2046 touch controller (integrated)
* MicroSD card slot (integrated)
* RGB LED (integrated)
* Light sensor (LDR, integrated)
* Speaker connection
* USB-C programming interface

### Planned Features

#### Core Emulation
* **Complete Game Boy CPU**: Full LR35902 instruction set emulation
* **Memory Bank Controllers**: Support for MBC1, MBC2, MBC3, and MBC5
* **Audio Processing Unit**: Four-channel Game Boy sound emulation
* **Save State System**: Multiple save slots with thumbnails and metadata
* **Battery SRAM**: Persistent save game support

#### User Interface
* **Touch-Based Controls**: Customizable virtual D-pad and buttons
* **Advanced Menu System**: ROM browser with search and organization
* **Theme Support**: Customizable UI elements and color schemes
* **Multi-Language Support**: Internationalization framework

#### Hardware Integration
* **Bluetooth Controllers**: External gamepad support with pairing management
* **RGB LED Integration**: System status and game event feedback
* **Automatic Brightness**: Light sensor-based display adjustment
* **Power Management**: Intelligent sleep modes and battery optimization
* **SD Card Management**: Comprehensive file organization and ROM library

#### Advanced Features
* **Performance Optimization**: Dual-core processing with real-time monitoring
* **Diagnostic Tools**: System information and debugging capabilities
* **OTA Updates**: Over-the-air firmware update support
* **Configuration Backup**: Settings persistence and restore functionality

## Development Roadmap

This project follows a structured development approach with clearly defined phases:

1. **Foundation Setup**: PlatformIO configuration and core emulation porting
2. **User Interface**: Touch system and menu implementation
3. **Gaming Features**: Save states and file management
4. **Hardware Integration**: Bluetooth, sensors, and peripheral support
5. **Audio Implementation**: Complete Game Boy sound emulation
6. **System Optimization**: Power management and performance tuning
7. **Documentation**: Comprehensive guides and community framework

## Getting Started with Development

### Prerequisites

* **PlatformIO Core** or **PlatformIO IDE** (VS Code extension)
* **ESP32-2432S028R** development board
* **Git** for version control

### Development Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/jolteon
   cd jolteon
   ```

2. **PlatformIO initialization**:
   ```bash
   pio project init
   ```

3. **Install dependencies**:
   ```bash
   pio lib install
   ```

4. **Build the project**:
   ```bash
   pio run
   ```

5. **Upload to board**:
   ```bash
   pio run --target upload
   ```

### Project Configuration

The `platformio.ini` file contains board-specific configurations:

```ini
[env:esp32-2432s028r]
platform = espressif32
board = esp32dev
framework = arduino
board_build.partitions = huge_app.csv
build_flags = 
    -DCORE_DEBUG_LEVEL=3
    -DUSER_SETUP_LOADED=1
    -DILI9341_DRIVER=1
    -DTFT_WIDTH=240
    -DTFT_HEIGHT=320
monitor_speed = 115200
lib_deps = 
    bodmer/TFT_eSPI
    paulstoffregen/XPT2046_Touchscreen
    arduino-libraries/SD
```

## Project Structure

```
jolteon/
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── emulation/
│   │   ├── cpu.cpp             # Game Boy CPU core
│   │   ├── memory.cpp          # Memory management
│   │   ├── ppu.cpp             # Picture processing unit
│   │   ├── apu.cpp             # Audio processing unit
│   │   └── cartridge.cpp       # ROM and MBC handling
│   ├── hardware/
│   │   ├── display.cpp         # TFT display interface
│   │   ├── touch.cpp           # Touch input handling
│   │   ├── audio.cpp           # I2S audio output
│   │   ├── storage.cpp         # SD card management
│   │   └── bluetooth.cpp       # Controller connectivity
│   ├── ui/
│   │   ├── menu.cpp            # Menu system
│   │   ├── controls.cpp        # Virtual controls
│   │   ├── themes.cpp          # UI theming
│   │   └── settings.cpp        # Configuration interface
│   └── utils/
│       ├── config.cpp          # Configuration management
│       ├── power.cpp           # Power management
│       └── diagnostics.cpp     # System monitoring
├── include/
│   ├── jolteon.h               # Main header
│   ├── emulation/              # Emulation headers
│   ├── hardware/               # Hardware abstraction
│   ├── ui/                     # User interface
│   └── utils/                  # Utility headers
├── data/                       # SPIFFS data files
├── test/                       # Unit tests
├── platformio.ini              # PlatformIO configuration
└── README.md
```

## Development Guidelines

### Code Organization
- **Modular Architecture**: Separate concerns with clear interfaces
- **Hardware Abstraction**: Abstract hardware-specific functionality
- **Error Handling**: Comprehensive error checking and recovery
- **Performance Focus**: Optimize for dual-core ESP32 architecture

### Testing Strategy
- **Unit Testing**: Individual component validation
- **Integration Testing**: Hardware and software interaction testing
- **Compatibility Testing**: ROM compatibility validation
- **Performance Testing**: Real-time performance monitoring

### Documentation Standards
- **Code Documentation**: Comprehensive inline documentation
- **API Documentation**: Clear interface specifications
- **User Guides**: Step-by-step usage instructions
- **Development Guides**: Contribution and extension guidelines

## Contributing

Jolteon is designed as a community-driven project. Contributions are welcome in all areas:

### Priority Areas
- **Audio Emulation**: Game Boy APU implementation
- **Performance Optimization**: Memory and CPU efficiency improvements
- **Hardware Features**: Enhanced peripheral integration
- **User Experience**: Interface and usability enhancements
- **Compatibility**: ROM support and accuracy improvements

### Development Process
1. **Fork the repository** and create a feature branch
2. **Follow coding standards** and documentation requirements
3. **Test thoroughly** on actual hardware
4. **Submit pull requests** with detailed descriptions
5. **Participate in code review** process

## Technical Specifications

### Performance Targets
- **Emulation Speed**: 100%+ original Game Boy performance
- **Display Refresh**: 60 FPS consistent frame rate
- **Audio Latency**: <50ms audio processing delay
- **Touch Response**: <100ms input to action latency
- **Battery Life**: 4+ hours continuous gaming

### Memory Management
- **SRAM Utilization**: Optimized for 520KB constraint
- **Flash Storage**: Efficient 4MB flash usage
- **Dynamic Allocation**: Minimal heap fragmentation
- **Cache Strategy**: Intelligent ROM and asset caching

## License

This project follows the same license terms as the original Espeon emulator. The code is provided for educational and development purposes.

## Acknowledgments

- **Original Espeon**: Foundation codebase and emulation core
- **Game Boy Development Community**: Technical documentation and research
- **ESP32 Community**: Hardware abstraction and optimization techniques
- **PlatformIO Team**: Advanced embedded development framework

---

**Note**: This project is under active development. Features and specifications may change as development progresses. Check the project roadmap for current status and upcoming milestones.

