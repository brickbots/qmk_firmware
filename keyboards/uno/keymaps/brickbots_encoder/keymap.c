/*
This is a keymap for doing some volume and scrolling stuff 
with the Uno v2. Started with the copypasta keymap
by u/Snipeye and got out of hand. :)

Copyright 2021 Dan Wright <dtwright@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "print.h"

#define RGB_STEPS 16
#define LED_INDEX_START 8
// how long allowed between double-taps to switch scroll direction?
#define SCROLL_HOLD_TIME 300 

// start colors for each mode
#define VOL_COLOR 140
#define V_SCROLL_COLOR 192
#define H_SCROLL_COLOR 32

enum uno_keycode
{
    MODE = SAFE_RANGE
};

enum encoder_names {
    _ENCODER,
};

uint16_t scrollTimer = 0;
uint8_t scrollClicks = 0;
// uint16_t scrollKeyCode = 0;
bool scrollMode = false;
bool scrollHoriz = false;

uint8_t ledIndex = LED_INDEX_START;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        MODE
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case MODE:
        if (record->event.pressed) {
        } else {
            if(scrollMode) {
                dprint("in scroll mode\n");
                dprintf("%d %d\n", scrollTimer, scrollClicks);
                scrollClicks++;
                if(scrollTimer == 0) scrollTimer = timer_read();
            } else {
                dprint("entering scroll mode\n");
                rgblight_sethsv_noeeprom(
                        scrollHoriz ? H_SCROLL_COLOR : V_SCROLL_COLOR, 
                        255, 
                        64
                );
                scrollMode = 1;
                scrollClicks = 0;
                scrollTimer = 0;
            }
        }
        break;
    }
    return false;
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(1);
    rgblight_sethsv_noeeprom(VOL_COLOR, 255, 64);

    // set to true to get hid console output from various print statements
    debug_enable = false;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _ENCODER) { /* First encoder */
        if (!clockwise) {
            // ledIndex++;
            // ledIndex %= RGB_STEPS;
            if(!scrollMode) {
                register_code(KC_VOLU);
                unregister_code(KC_VOLU);
            } else {
                if(scrollHoriz) {
                    register_code(KC_WH_R);
                    unregister_code(KC_WH_R);
                } else {
                    register_code(KC_WH_D);
                    unregister_code(KC_WH_D);
                }
            }
        } else {
            // ledIndex += RGB_STEPS;
            // ledIndex--;
            // ledIndex %= RGB_STEPS;
            if(!scrollMode) {
                register_code(KC_VOLD);
                unregister_code(KC_VOLD);
            } else {
                if(scrollHoriz) {
                    register_code(KC_WH_L);
                    unregister_code(KC_WH_L);
                } else {
                    register_code(KC_WH_U);
                    unregister_code(KC_WH_U);
                }
            }
        }
        // Uncomment the following 2 lines and the "ledIndex" lines above to 
        // change colors as the encoder turns.
        // rgblight_sethsv_noeeprom((255/RGB_STEPS)*ledIndex, 255, 255);
        // rgblight_mode_noeeprom(1);
    }
}

void matrix_scan_user(void) {
    if(scrollMode) {
        if(timer_elapsed(scrollTimer) < SCROLL_HOLD_TIME && scrollClicks > 1) {
            dprint("switching scroll direction\n");
            scrollHoriz = !scrollHoriz;
            scrollClicks = 0;
            scrollTimer = 0;
            rgblight_sethsv_noeeprom(
                scrollHoriz ? H_SCROLL_COLOR : V_SCROLL_COLOR, 
                255, 
                64
            );
        } else if(timer_elapsed(scrollTimer) > SCROLL_HOLD_TIME && scrollClicks == 1) {
            dprint("leaving scroll mode\n");
            rgblight_sethsv_noeeprom(VOL_COLOR, 255, 64);
            scrollMode = 0;
        }
    }
}
