/**
 * @file lv_conf.h
 * Configuration file for LVGL v9.x
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
 C OLOR SETTINGS   *
 *====================*/

/* Color depth: 1 (1 bpp), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH 16

/*=========================
 M EMORY SETTINGS  *
 *=========================*/

/* 1: use custom malloc/free, 0: use the built-in `lv_mem_alloc()` and `lv_mem_free()` */
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
/* Size of the memory available for `lv_mem_alloc()` in bytes (>= 2kB) */
#define LV_MEM_SIZE (64U * 1024U) /*[bytes]*/
/* Set an address for the memory pool instead of allocating it as a normal array. Can be in external SRAM too. */
#define LV_MEM_ADR 0 /*0: unused*/
/* Instead of an address give a memory allocator that will be called to get a memory pool for LVGL. E.g. my_malloc */
#if LV_MEM_ADR == 0
#undef LV_MEM_POOL_INCLUDE
#undef LV_MEM_POOL_ALLOC
#endif
#endif /*LV_MEM_CUSTOM*/

/*====================
 H AL SETTINGS     *
 *====================*/

/* Default display refresh period. LVG will redraw changed areas with this period time */
#define LV_DISP_DEF_REFR_PERIOD 30 /*[ms]*/

/* Input device read period in milliseconds */
#define LV_INDEV_DEF_READ_PERIOD 30 /*[ms]*/

/*===================
 F EATURE SETTINGS *
 *===================*/

/* 1: Show CPU usage and FPS count */
#define LV_USE_PERF_MONITOR 0
#if LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif

/* 1: Show the used memory and the memory fragmentation
 * Requires LV_MEM_CUSTOM = 0 */
#define LV_USE_MEM_MONITOR 0
#if LV_USE_MEM_MONITOR
#define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#endif

/* 1: Draw random colored rectangles over the redrawn areas */
#define LV_USE_REFR_DEBUG 0

/*==================
 * WIDGET USAGE
 *================*/

/*-------------------------------------
 * Documentation of all widgets:
 * https://docs.lvgl.io/latest/en/widgets/index.html
 *-------------------------------------*/

#define LV_USE_WIDGETS_DEFAULT 1

#if LV_USE_WIDGETS_DEFAULT
#define LV_USE_LABEL 1
#define LV_USE_BTN 1
#define LV_USE_BTNMATRIX 1
#define LV_USE_CANVAS 1
#define LV_USE_LIST 1
#endif /*LV_USE_WIDGETS_DEFAULT*/

/*==================
 * THEME USAGE
 *================*/

/* A simple, impressive and very complete theme */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

/* 0: Light mode; 1: Dark mode */
#define LV_THEME_DEFAULT_DARK 1

/* 1: Enable grow on press */
#define LV_THEME_DEFAULT_GROW 1

/* Default transition time in [ms] */
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/*==================
 * FONT USAGE
 *================*/

/* Montserrat fonts with various styles and sizes: https://fonts.google.com/specimen/Montserrat */
#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* Demonstrate special features */
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0 /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0 /*Hebrew, Arabic, Persian letters and all their forms*/
#define LV_FONT_SIMSUN_16_CJK 0 /*1000 most common CJK radicals*/

/*Pixel perfect monospace fonts*/
#define LV_FONT_UNSCII_8 0
#define LV_FONT_UNSCII_16 0

/*==================
 * TEXT SETTINGS
 *================*/

/**
 * Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Can break (wrap) texts on these chars*/
#define LV_TXT_BREAK_CHARS " ,.;:-_"

/*==================
 * LV_SPRINTF
 *================*/
#define LV_SPRINTF_CUSTOM 0
#if LV_SPRINTF_CUSTOM
#define LV_SPRINTF_INCLUDE <stdio.h>
#define lv_snprintf snprintf
#define lv_vsnprintf vsnprintf
#else /*LV_SPRINTF_CUSTOM*/
#define LV_SPRINTF_USE_FLOAT 0
#endif /*LV_SPRINTF_CUSTOM*/

#define LV_USE_USER_DATA 1

/*==================
 * ASSERT
 *================*/
#define LV_USE_ASSERT_NULL 1 /*Check if the parameter is NULL. (Very fast, recommended)*/
#define LV_USE_ASSERT_MALLOC 1 /*Checks is the memory is successfully allocated or no. (Very fast, recommended)*/
#define LV_USE_ASSERT_STYLE 0 /*Check if the styles are properly initialized. (Very fast, recommended)*/
#define LV_USE_ASSERT_MEM_INTEGRITY 0 /*Check the integrity of `lv_mem` after critical operations. (Slow)*/
#define LV_USE_ASSERT_OBJ 0 /*Check the object's type and existence (e.g. not deleted). (Slow)*/

/*Add a custom handler when assert happens e.g. to restart the MCU*/
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER \
    while (1)             \
        ; /*Halt by default*/

/*==================
 * OTHERS
 *================*/

/* 1: Show error log on the console */
#define LV_USE_LOG 1
#if LV_USE_LOG

/*How important log should be added:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 *LV_LOG_LEVEL_INFO        Log important events
 *LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 *LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 *LV_LOG_LEVEL_USER        Only logs added by the user
 *LV_LOG_LEVEL_NONE        Do not log anything
 */
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/* 1: Print the log with 'printf';
 * 0: User need to register a callback with `lv_log_register_print_cb()`*/
#define LV_LOG_PRINTF 1

/* Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs */
#define LV_LOG_TRACE_MEM 1
#define LV_LOG_TRACE_TIMER 1
#define LV_LOG_TRACE_INDEV 1
#define LV_LOG_TRACE_DISP_REFR 1
#define LV_LOG_TRACE_EVENT 1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT 1
#define LV_LOG_TRACE_ANIM 1

#endif /*LV_USE_LOG*/

#endif /*LV_CONF_H*/
