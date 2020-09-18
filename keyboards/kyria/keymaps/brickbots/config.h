/* Copyright 2020 Richard Sutherland <rich@brickbots.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
  #define OLED_TIMEOUT 10000
#endif

#ifdef RGBLIGHT_ENABLE
//  #define RGBLIGHT_ANIMATIONS
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
#endif

#define TAP_CODE_DELAY 100
#define PERMISSIVE_HOLD
#define TAPPING_TERM 200
// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
// #define SPLIT_USB_DETECT
// #define NO_USB_STARTUP_CHECK

// Using the bottom encoder positions and my encoders
// my board reports the wrong direction
#undef ENCODERS_PAD_A
#undef ENCODERS_PAD_B
#undef ENCODERS_PAD_A_RIGHT
#undef ENCODERS_PAD_B_RIGHT

//This is for the wonky encoders on the brass board
#define ENCODERS_PAD_A { C6 }
#define ENCODERS_PAD_B { B5 }
#define ENCODERS_PAD_A_RIGHT { B5 }
#define ENCODERS_PAD_B_RIGHT { C6 }
#define ENCODER_RESOLUTION 2

// These are the standard encoders
//#define ENCODERS_PAD_A { B5 }
//#define ENCODERS_PAD_B { C6 }
//#define ENCODERS_PAD_A_RIGHT { C6 }
//#define ENCODERS_PAD_B_RIGHT { B5 }
//#define ENCODER_RESOLUTION 2
