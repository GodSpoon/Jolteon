#ifndef INTERNAL_H
#define INTERNAL_H

#include <cstdint>

/* Uncomment this to include a fallback ROM */
// #define USE_INTERNAL_ROM

/* Uncomment if including a bios on compile */
//#define USE_INTERNAL_BIOS

#define USE_INTERNAL_BORDER

#ifdef USE_INTERNAL_ROM
	#include "gbrom.h"
#else
	extern const uint8_t* gb_rom;
	extern const uint32_t gb_rom_size;
#endif

#ifdef USE_INTERNAL_BIOS
	#include "gbbios.h"
#else
	extern const uint8_t* gb_bios;
	extern const uint32_t gb_bios_size;
#endif

#ifdef USE_INTERNAL_BORDER
	#include "jolteon_border.h"  // Updated to use Jolteon theme
#else
	const uint8_t* gb_border = nullptr;
	const uint32_t gb_border_size = 0;
#endif

#endif