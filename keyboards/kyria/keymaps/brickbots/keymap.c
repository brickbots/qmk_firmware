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
#include QMK_KEYBOARD_H
#include <stdio.h>


#ifdef VCNL4010_ENABLE
#include "vcnl4010_driver.h"
bool prox_init = false;
#endif

#ifdef MCP9808_ENABLE
#include "mcp9808_driver.h"
bool temp_init = false;
#endif

#ifdef OLED_DRIVER_ENABLE
uint16_t slave_oled_timeout = 0;
#endif

#ifdef WPM_ENABLE
uint16_t wpm_graph_timer = 0;
#endif

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum my_keycodes {
  VIS_ROT = SAFE_RANGE
};

#define OLED_VIS_COUNT 4
static uint8_t current_oled_vis = 2; 
/* 0 = Blank
 * 1 = WPM Graph
 * 2 = Key Matrix drops
 * 3 = Kyria Logo
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |GUI/GRV |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  BKSP  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |ALT/UNDS|   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | ESC  |      |  |      | TAB  |   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------|      |  |      |------+------+------+------+----------------------'
 *                        | ENC  | UNDS | CTRL |      | Enter|  |Space |      | CTRL | COLN |  ENC |
 *                        |      |      |      | Lower|      |  |      | Raise|      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      GUI_T(KC_GRV),    KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      ALT_T(KC_UNDS),   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_LSFT,  _______, _______, KC_RSFT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
			_______, KC_ESC, KC_LCTRL,  MO(_LOWER),   KC_ENT,   KC_SPC, MO(_RAISE), KC_RCTRL, KC_TAB, _______
    ),
/*
 * RAISE Layer: Symbols
 *
 */
    [_RAISE] = LAYOUT(
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,					KC_6,	  KC_7,	   KC_8,    KC_9,    KC_0,    _______,\
      _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,					KC_CIRC,  KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,\
      _______, KC_TILD, KC_GRV,  KC_PLUS, KC_MINS, KC_EQL,  _______, _______, _______, _______, KC_LBRC,  KC_RBRC, KC_LCBR, KC_RCBR, KC_BSLS, _______,\
                                 _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______
    ),
/*
 * Lower Layer: Navigation and functions
 *
 */
    [_LOWER] = LAYOUT(
      _______,  KC_F1,   KC_F2,   S(KC_TAB), KC_TAB,  _______,					  KC_F6,   KC_F7,  KC_F8,  KC_F9,    KC_F10,  KC_DEL, \
      _______,  KC_BTN4, KC_BTN2, KC_BTN3,  KC_BTN1,  KC_WH_U,					  KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT,  KC_VOLU, KC_MUTE, \
      _______, _______, _______, _______, _______, KC_WH_D, _______, _______, _______, _______,	  KC_HOME, KC_PGDN,  KC_PGUP, KC_END,  KC_VOLD, _______, \
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  F8  | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      |      |      | F11  | F12  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        | OLED | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     _______, _______, _______, KC_F11,  KC_F12,  _______,
      _______, VIS_ROT, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}


static void render_slave_status(void) {
    static char temp_str[5];

    oled_write_P(PSTR("Time: "), false);
    oled_write_P(PSTR("\n"), false);
#ifdef MCP9808_ENABLE
    if (temp_init) {
	oled_write_P(PSTR("Temp: "), false);
	mcp9808_read_str(temp_str);
	oled_write(temp_str, false);
    }
    oled_write_P(PSTR("\n"), false);
#endif

#ifdef VCNL4010_ENABLE
    if (prox_init) {
	static uint16_t prox_raw;
	static char prox_str[20];
	prox_raw = vcnl4010_read_raw();

	sprintf(prox_str, "Prox: %04d\n", prox_raw);
	oled_write(prox_str, false);
    }
#endif
#ifdef WPM_ENABLE
    static char wpm_str[10];
    // Write WPM
    sprintf(wpm_str, "WPM: %03d", get_current_wpm());
    oled_write_P(PSTR("\n"), false);
    oled_write(wpm_str, false);
#endif
}


static void render_kyria_logo(void) {
    static const char PROGMEM kyria_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
}

const uint8_t single_bit_masks[8] = {127, 191, 223, 239, 247, 251, 253, 254};

static void fade_display(void) {
    oled_buffer_reader_t reader;
    uint8_t buff_char;
    if(random() % 30==0) {
	srand(timer_read());
	reader = oled_read_raw(0);
        for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
            buff_char = *reader.current_element;
	    if (buff_char != 0) {
                oled_write_raw_byte(buff_char & single_bit_masks[rand() % 8], i);
	    }
	    reader.current_element++;
        }
    }
}

static void draw_box(uint8_t x, uint8_t y, bool inside) {
    uint16_t start_index = (y * OLED_DISPLAY_WIDTH) + (x*8);
    oled_buffer_reader_t reader;
    if (inside) {
	start_index += 2;

        for (uint8_t i=0; i < 4; i++)
	    oled_write_raw_byte(60, start_index + i);
    } else {
        reader = oled_read_raw(start_index);
	for (uint8_t i=0; i < 8; i++) {
	    if (i > 1 && i < 6)
		oled_write_raw_byte((*reader.current_element)++ | 195, start_index + i);
	    else
		oled_write_raw_byte(255, start_index + i);
	}
    }
}

static void render_keymap_vis(keyrecord_t *record) {
    uint8_t map_col = record->event.key.col;
    uint8_t map_row = record->event.key.row;

    if (map_row < 4) {
	map_col = 7 - map_col;
    }
    else {
	map_row=map_row - 4;
	map_col=map_col + 8;
    }

    // fill rest of display
    uint8_t rand_col = 15 - map_col;
    uint8_t rand_row = 3 - map_row;
    if (rand_row > 1)
	rand_row += 4;

    // Center on display
    map_row=map_row + 2;
    draw_box(map_col, map_row, record->event.pressed);
    draw_box(rand_col, rand_row, record->event.pressed);
}

#ifdef WPM_ENABLE
static void render_wpm_graph(void) {
    static uint8_t zero_bar_count = 0;
    static uint8_t bar_count = 0;
    uint8_t bar_height = 0;
    uint8_t bar_segment = 0;

    if (wpm_graph_timer == 0) {
	wpm_graph_timer = timer_read();
	return;
    }
    if(timer_elapsed(wpm_graph_timer) > 500) {
	wpm_graph_timer = timer_read();

	if(OLED_DISPLAY_HEIGHT == 64)
		bar_height = get_current_wpm() / 2;
	if(OLED_DISPLAY_HEIGHT == 32)
		bar_height = get_current_wpm() / 4;
	if(bar_height > OLED_DISPLAY_HEIGHT)
		bar_height = OLED_DISPLAY_HEIGHT;

	if(bar_height == 0) {
	    // keep track of how many zero bars we have drawn.  If
	    // there is a whole screen worth, turn the display off and 
	    // wait until there is something to do
	    if (zero_bar_count > OLED_DISPLAY_WIDTH) {
		oled_off();
		return;
	    }
	    zero_bar_count++;
	} else
	    zero_bar_count=0;

	oled_pan(false);
	bar_count++;
	for (uint8_t i = (OLED_DISPLAY_HEIGHT / 8); i > 0; i--) {
	    if (bar_height > 7) {
		if (i % 2 == 1 && bar_count % 3 == 0)
		    bar_segment = 254;
		else
		    bar_segment = 255;
		bar_height -= 8;
	    } else {
		switch (bar_height) {
		    case 0:
			bar_segment = 0;
			break;

		    case 1:
			bar_segment = 128;
			break;

		    case 2:
			bar_segment = 192;
			break;

		    case 3:
			bar_segment = 224;
			break;

		    case 4:
			bar_segment = 240;
			break;

		    case 5:
			bar_segment = 248;
			break;

		    case 6:
			bar_segment = 252;
			break;

		    case 7:
			bar_segment = 254;
			break;
		}
		bar_height = 0;

		if (i % 2 == 1 && bar_count % 3 == 0)
		    bar_segment++;
	    }
	    oled_write_raw_byte(bar_segment, (i-1) * OLED_DISPLAY_WIDTH);
	}
    }
}
#endif


static void oled_rotate_vis(void) {
    current_oled_vis++;
    if (current_oled_vis == OLED_VIS_COUNT)
	current_oled_vis = 0;

    // Setup anyting that needs doing on mode switch
    switch(current_oled_vis) {
        case 0: //Blank
	    oled_clear();
	    break;

        case 1: //WPM
	    oled_clear();
	    break;

        case 2: //Matrix drops
	    oled_clear();
	    break;

        case 3: //Logo
	    oled_clear();
	    render_kyria_logo();
	    oled_scroll_set_speed(0);
	    oled_scroll_left();
	    break;
    }
}

static void oled_keystroke_task(keyrecord_t *record) {
    if (current_oled_vis == 2)
	render_keymap_vis(record);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
	switch(current_oled_vis) {
	    case 1: //WPM
		#ifdef WPM_ENABLE
		render_wpm_graph();
		#endif
		break;

	    case 2: //Matrix drops
		fade_display();
		break;
	}
    } else {
	//Since we don't actually get keypress info on the slave side
	//Use changed WPM as a proxy
	if(get_current_wpm() > 0)
	    slave_oled_timeout=timer_read();
	// If we have not typed anything in 10 seconds, stop updating the displays
	if (timer_elapsed(slave_oled_timeout) > 10000) {
		oled_off();
		return;
	}
        render_slave_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    }
}
#endif

#ifdef ENCODER_ENABLE
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
	if (clockwise) {
	  if (!is_alt_tab_active) {
	    is_alt_tab_active = true;
	    register_code(KC_LALT);
	  }
	  alt_tab_timer = timer_read();
	  tap_code16(KC_TAB);
	} else {
	  if (!is_alt_tab_active) {
	    is_alt_tab_active = true;
	    register_code(KC_LALT);
	  }
	  alt_tab_timer = timer_read();
	  tap_code16(S(KC_TAB));
	}
    }
    else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code16(C(A(KC_RIGHT)));
        } else {
            tap_code16(C(A(KC_LEFT)));
        }
    }
}
#endif

void keyboard_post_init_user(void) {
    debug_enable=false;
#ifdef MCP9808_ENABLE
    if (!is_keyboard_master()) {
	temp_init = mcp9808_init();
    }
#endif
#ifdef VCNL4010_ENABLE
    if (!is_keyboard_master()) {
	prox_init = vcnl4010_init();
    }
#endif
    return;
}
void matrix_scan_user(void) {
#ifdef ENCODER_ENABLE
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 700) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == VIS_ROT)
	if (record->event.pressed)
	    oled_rotate_vis();

#ifdef OLED_DRIVER_ENABLE
    oled_keystroke_task(record);
#endif

    return true;
}
