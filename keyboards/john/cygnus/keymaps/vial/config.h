#pragma once

#define VIAL_KEYBOARD_UID {0x3B, 0x6B, 0xA0, 0x29, 0x80, 0x56, 0xED, 0xD1}
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define DYNAMIC_KEYMAP_LAYER_COUNT 1
#define TAPPING_TERM 180

#define USE_SERIAL_PD2

//#define MATRIX_ROWS 8
//#define MATRIX_COLS 5

//#define MATRIX_ROW_PINS { F5, F6, F7, B1, B3 }
//#define MATRIX_COL_PINS { D4, C6, D7, E6 }

//#define USE_MATRIX_I2C
//#ifdef KEYBOARD_crkbd_rev1_legacy
//#    undef USE_I2C
#    define USE_SERIAL
//#endif

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// #define DEBUG_MATRIX_SCAN_RATE