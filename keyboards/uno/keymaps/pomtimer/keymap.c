#include QMK_KEYBOARD_H
#include <print.h>


enum uno_keycode
{
  UNO = SAFE_RANGE,
};

// All the stuff to handle basic interaciotns
enum interations
{
  SINGLE_TAP,
  DOUBLE_TAP,
  LONG_PRESS, // > LONG_PRESS_LENGTH
  RESET_PRESS, // > RESET_LENGTH
  TIMEOUT,
};

uint16_t press_timer = 0;
uint32_t press_timer_long = 0;
bool pressed = 0;
bool double_tap = false;
bool tap_wait = false;
bool interaction_pending = false;

#define LONG_PRESS_LENGTH 250
#define RESET_LENGTH 3000
#define TAP_TERM 175

// Specific program stuff
enum timer_modes
{
  IDLE,
  FOCUSTIME,
  BREAKTIME,
};

bool paused = 0;
uint8_t mode = IDLE;
#define FOCUS_MINUTES 25
#define BREAK_MINUTES 5

const uint32_t focus_ticks = FOCUS_MINUTES * 1000 * 60;
const uint32_t break_ticks = BREAK_MINUTES * 1000 * 60;

uint32_t cycle_end = 0;
uint32_t pause_start = 0;
uint32_t pause_start = 0;

#define BRIGHT 128
#define DIM 64
#define BLUE 170
#define GREEN 85
#define RED 0

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
      [0] = LAYOUT(
            UNO
          )
};

void reset_device(void) {

    // Always lock for safety, but only
    // reset the rest and flash if we are not
    // already in the reset state
    if(mode != IDLE) {
	mode = IDLE;
        press_timer = 0;
        press_timer_long = 0;
        rgblight_sethsv_noeeprom(GREEN, 255, 0);
        wait_ms(200);
        rgblight_sethsv_noeeprom(GREEN, 255, BRIGHT);
        wait_ms(100);
        rgblight_sethsv_noeeprom(0, 255, 0);
        wait_ms(200);
        rgblight_sethsv_noeeprom(GREEN, 255, DIM);
    }
}


void interaction_handler(uint8_t interaction_type) {
    if(interaction_type == RESET_PRESS) {
	reset_device();
	return;
    }
    if(interaction_type == TIMEOUT) {
        // Timer for whatever mode we are in has elapsed
    }
    if(locked) {
	if(interaction_type == LONG_PRESS) {
	    rgblight_sethsv_noeeprom(0, 255, BRIGHT);
	    if(current_code == passcode[pc_index])
		good_code++;
	    if(pc_index == 3) {
		if(good_code == 4) {
		    locked = 0;
		    rgblight_sethsv_noeeprom(82,255,BRIGHT);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(82,255,BRIGHT);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(300);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index],255,DIM);
		}
		else
		{
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(0,255,BRIGHT);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(0,255,BRIGHT);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    reset_device();
		}
	    }
	    else {
		pc_index++;
		current_code = 0;
	    }
	}
	else {
	    rgblight_sethsv_noeeprom(0, 255, BRIGHT);
	    current_code++;
	    if(current_code > 9)
		current_code = 0;
	    dprintf("Current Index: %d - Code: %d\n", pc_index,current_code);
	}
	return;
    } // End locked / code handling

    // If unlocked
    switch(secret_stage) {
	case 0:
            // Selecting password color
	    if(interaction_type == LONG_PRESS) {
		// Select!
		secret_stage = 1;
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, DIM);
		wait_ms(100);
		rgblight_sethsv_noeeprom(0,0,0);
		wait_ms(200);
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, BRIGHT);
	    }
	    else {
		// Cycle
	      	secret_index++;
	        if(secret_index == secret_count)
		    secret_index = 0;
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, DIM);
	    }
	    break;

	case 1:
	    if(interaction_type == LONG_PRESS) {
		// Back to selecting
		secret_stage = 0;
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, DIM);
		wait_ms(100);
		rgblight_sethsv_noeeprom(0,0,0);
		wait_ms(300);
		secret_index++;
		if(secret_index == secret_count)
		    secret_index=0;
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, DIM);
	    }
	    else {
		// Send Selected password or username
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, BRIGHT);
		if(interaction_type == DOUBLE_TAP)
		    send_string(pw_passwords[secret_index]);
		else
		    send_string(pw_usernames[secret_index]);
	    }
	    break;
    }

}


void matrix_scan_user(void) {

    uint16_t timeElapsed = timer_elapsed(press_timer);

    // If 5 minutes have elapsed with no new presses... lock it up!
    if(timer_read32() - press_timer_long > 300000) {
	interaction_handler(TIMEOUT);
    }
    if(pressed) {
	// If pressed for more than 3 seconds at any point... lock it up!
	if(timeElapsed > RESET_LENGTH) {
	    interaction_handler(RESET_PRESS);
	}
        if(timeElapsed > LONG_PRESS_LENGTH) {
	    if(!interaction_pending) {
		interaction_pending = true;
	        interaction_handler(LONG_PRESS);
	    }
    	    //dprintf("MSU: Longpres\n");
        }
    }

    if(tap_wait && timeElapsed > TAP_TERM) {
	tap_wait = false;
	interaction_handler(SINGLE_TAP);
    }

}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
	if(timer_elapsed(press_timer) < TAP_TERM) {
	    dprint("DT1\n");
	    double_tap = true;
	    tap_wait = false;
	}
	else
	{
	    double_tap = false;
	}
	// Get both here to have the ability to time > 65 seconds
	press_timer_long = timer_read32();
	press_timer = timer_read();
	pressed = 1;
        rgblight_sethsv_noeeprom(0,0,0);
    } else {
	pressed = 0;
	uint16_t timeElapsed = timer_elapsed(press_timer);
	if(timeElapsed < LONG_PRESS_LENGTH) {
	    if(locked)
		interaction_handler(SINGLE_TAP);
	    else {
		if(double_tap)
		    interaction_handler(DOUBLE_TAP);
		else
		    tap_wait = true;
	    }
	}
    }
    interaction_pending = false;
    return false;
}

void keyboard_post_init_user(void) {
    //debug_enable=true;
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(85, 255, DIM);
    rgblight_mode(1);
}
