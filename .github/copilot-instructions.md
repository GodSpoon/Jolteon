Jolteon: ESP32-2432S028R GB Emulator

## Hardware Specs
- **Display**: ILI9341 240x320 (TFT_eSPI)
- **Touch**: XPT2046
- **Storage**: SD SPI
- **Audio**: I2S (Phase V)
- **Extras**: RGB LED/LDR

## Core Architecture
```
src/
├── core/      // GB emu
├── display/   // TFT render
├── ui/        // Menu
└── hardware/  // HAL

// PascalCase classes, camelCase funcs
class GameBoyCore; void updateDisplay();
```

## Phase Workflow
1. Confirm current phase
2. Check `docs/phases/0X.md`
3. Verify deps
4. Implement criteria

## Critical Code Snippets
**Display Scaling** (Phase I):
```
#include <TFT_eSPI.h>  // 160x144→240x320 (1.5x)
```

**Memory Mgmt**:
```
static uint8_t vram[0x2000];  // PROGMEM palettes
```

**Dual-Core**:
```
xTaskCreatePinnedToCore(emulationTask, "emu", 4096, NULL, 1, NULL, 0);
```

## Pin Map
| Function   | Pins          |
|------------|---------------|
| TFT        | 15(CS),2(DC)  |
| Touch      | 33(CS),36(IRQ)|
| SD         | 5(CS),23-19   |

## Perf Targets
| Metric         | Target    |
|----------------|-----------|
| FPS            | 60        |
| Audio Latency  | <50ms     |
| SRAM           | <200KB    |

## M5 Migration
- `Lcd`→`tft`
- `Speaker`→I2S
- Update pin defines

// Testing: Unit/HIL/ROM validation
```


