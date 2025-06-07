#include "gbfiles.h"
#include "jolteon_border.h"

// Define the global variables here
#ifndef USE_INTERNAL_ROM
const uint8_t* gb_rom = nullptr;
const uint32_t gb_rom_size = 0;
#endif

#ifndef USE_INTERNAL_BIOS
const uint8_t* gb_bios = nullptr;
const uint32_t gb_bios_size = 0;
#endif

// Define jolteon_border variables
const uint8_t* jolteon_border = nullptr;
const uint32_t jolteon_border_size = 0;
