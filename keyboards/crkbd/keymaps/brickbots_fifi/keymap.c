// generated from users/manna-harbour_miryoku/miryoku.org  -*- buffer-read-only: t -*-

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "print.h"


#ifdef WPM_ENABLE
uint16_t wpm_graph_timer = 0;
#endif

#ifdef OLED_DRIVER_ENABLE
uint16_t slave_oled_timeout = 0;

bool logo_rendered = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (false) {
    return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

enum my_keycodes {
  VIS_ROT = SAFE_RANGE
};

/*
void oled_render_BB_logo(void) {
    static const char PROGMEM bb_logo[] = {
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x31, 0x31, 0x31, 0x31, 0x31, 0x01, 0x01, 0xc1, 0xe3, 0xff, 
0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x31, 0x31, 0x31, 0x31, 0x31, 0x01, 0x01, 0xc1, 0xe3, 0xff, 
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x81, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x81, 0xff, 
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0x4f, 0x4f, 0x0f, 0x0f, 0xff, 
0x0f, 0x0f, 0x4f, 0x4f, 0x0f, 0x8f, 0xff, 0x3f, 0x1f, 0x4f, 0x4f, 0x4f, 0x4f, 0xff, 0xff, 0xff, 
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x80, 0x80, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x90, 0x90, 0xbe, 0xbe, 0xb0, 0xb0, 0xbf, 
0xb0, 0xb0, 0xb2, 0xb2, 0xb0, 0xb0, 0xbf, 0xb2, 0xb2, 0xb2, 0xb2, 0xb8, 0xbc, 0xbf, 0xbf, 0xff
    };
    oled_write_raw_P(bb_logo, sizeof(bb_logo));
}
*/
void oled_render_BB_logo(void) {
    static const char PROGMEM bb_logo[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x01, 0x01, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xff, 0xff,
0x01, 0x01, 0x01, 0x01, 0xc1, 0xc1, 0x01, 0x01, 0x01, 0x01, 0xc1, 0xc1, 0x01, 0x01, 0x01, 0xff,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x00, 0xf8, 0xff, 0x99, 0x99, 0x99, 0x81, 0x81, 0x99, 0x83, 0x87, 0xff, 0xff, 0xff, 0xff,
0xc0, 0x80, 0x80, 0x80, 0x87, 0x87, 0x80, 0xc0, 0xe0, 0xe0, 0xc7, 0x87, 0x80, 0x80, 0xc0, 0xff,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x00, 0xff, 0xff, 0x40, 0x40, 0xcc, 0x40, 0x60, 0x4c, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff,
0x03, 0x03, 0x03, 0x03, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0xff,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0xff, 0x80, 0xff, 0xff, 0xf8, 0xf0, 0xe7, 0xe0, 0xe0, 0xfe, 0xe0, 0xe1, 0xff, 0xff, 0xff, 0xff,
0xc0, 0x80, 0x80, 0x80, 0x87, 0x87, 0x80, 0xc0, 0xe0, 0xe0, 0xc7, 0x87, 0x80, 0x80, 0xc0, 0xff
    };
    oled_write_raw_P(bb_logo, sizeof(bb_logo));
    logo_rendered=1;
}


const uint8_t single_bit_masks[8] = {127, 191, 223, 239, 247, 251, 253, 254};

static void fade_display(void) {
    oled_buffer_reader_t reader;
    uint8_t buff_char;
    if(random() % 30==0) {
	srand(timer_read());
	reader = oled_read_raw(0);
        for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
	    if (i % 128 < 96) {
		buff_char = *reader.current_element;
		if (buff_char != 0) {
		    oled_write_raw_byte(buff_char & single_bit_masks[rand() % 8], i);
		}
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

    if (map_row > 3) {
	map_row = map_row - 4;
	map_col = 11 - map_col;
    }
    // Center on display
    draw_box(map_col, map_row, record->event.pressed);
}
#define OLED_VIS_COUNT 4
static uint8_t current_oled_vis = 1;
/* 0 = Blank
 * 1 = WPM Graph
 * 2 = Key Matrix drops
 * 3 = Kyria Logo
*/


#ifdef WPM_ENABLE
static void render_wpm_graph(void) {
    static uint8_t zero_bar_count = 0;
    static uint8_t bar_count = 0;
    uint8_t bar_height = 0;
    uint8_t bar_segment = 0;
    static char wpm_str[10];

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
		if (i % 2 == 1 && bar_count % 4 == 0)
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

		if (i % 2 == 1 && bar_count % 4 == 0)
		    bar_segment++;
	    }
	    oled_write_raw_byte(bar_segment, (i-1) * OLED_DISPLAY_WIDTH);
	}
	oled_set_cursor(18,0);
        sprintf(wpm_str, "%03d", get_current_wpm());
        oled_write(wpm_str, false);
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
	    oled_scroll_set_speed(0);
	    oled_scroll_left();
	    break;
    }
}

static void oled_keystroke_task(keyrecord_t *record) {
    if (is_master) {
	render_keymap_vis(record);
    }
    return;
}

void oled_task_user(void) {
    if (!is_master) {
#ifdef WPM_ENABLE
	render_wpm_graph();
#endif
    } else {
	if (!logo_rendered)
	    oled_render_BB_logo();
	fade_display();
    }
}

#endif //OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_DRIVER_ENABLE
    if (keycode == VIS_ROT)
	if (record->event.pressed)
	    oled_rotate_vis();
    oled_keystroke_task(record);
#endif

    return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
