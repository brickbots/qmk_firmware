#include QMK_KEYBOARD_H
#include <print.h>

// Include YOUR secret file here... from outside the code tree
// #include "~/.secrets.h"
#include "./secrets.h"

enum uno_keycode
{
  UNO = SAFE_RANGE,
};

uint16_t pressTimer = 0xFFFF;
uint8_t secret_index = 0;
uint8_t secret_stage = 0;
uint8_t pc_index = 0;
uint8_t current_code = 0;
uint8_t good_code = 0;
bool locked = 1;
bool pressed = 0;
bool double_tap = false;

#define LONGPRESS 250
#define RESET_LENGTH 3000

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
      [0] = LAYOUT(
            UNO
          )
};

void reset_device(void) {
    rgblight_sethsv_noeeprom(0, 255, 255);
    locked = 1;
    pc_index = 0;
    current_code = 0;
    good_code = 0;
    secret_index = 0;
    secret_stage = 0;
}

void matrix_scan_user(void) {

    uint16_t timeElapsed = timer_elapsed(pressTimer);

    // If 5 minutes have elapsed with no new presses... lock it up!
    if(timeElapsed > 300000) {
	reset_device();
    }
    if(pressed) {
	// If pressed for more than 3 seconds at any point... lock it up!
	if(timeElapsed > 3000) {
	    reset_device();
	}
        if(timeElapsed > LONGPRESS) {
    	    //dprintf("MSU: Longpres\n");
            if(locked) {
	        rgblight_sethsv_noeeprom(0, 255, 255);
            }
	    else {
		rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 128);
	    }
        }
    }

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
	if(timer_elapsed(pressTimer) < 200) {
	    dprint("DT1\n");
	    double_tap = true;
	}
	else
	{
	    double_tap = false;
	}
	pressTimer = timer_read();
	pressed = 1;
        rgblight_sethsv_noeeprom(0,0,0);
    } else {
	pressed = 0;
	uint16_t timeElapsed = timer_elapsed(pressTimer);
	bool long_pressed = 0;
	if(timeElapsed > LONGPRESS)
		long_pressed = 1;
	if(locked) {
	    if(long_pressed) {
	        rgblight_sethsv_noeeprom(0, 255, 255);
		if(current_code == passcode[pc_index])
		    good_code++;

		if(pc_index == 3) {
		    if(good_code == 4) {
			locked = 0;
			rgblight_sethsv_noeeprom(82,255,255);
			wait_ms(100);
			rgblight_sethsv_noeeprom(0,0,0);
			wait_ms(200);
			rgblight_sethsv_noeeprom(82,255,255);
			wait_ms(100);
			rgblight_sethsv_noeeprom(0,0,0);
			wait_ms(300);
			rgblight_sethsv_noeeprom(pw_colors[secret_index],255,128);
		    }
		    else
		    {
			rgblight_sethsv_noeeprom(0,0,0);
			wait_ms(200);
			rgblight_sethsv_noeeprom(0,255,255);
			wait_ms(100);
			rgblight_sethsv_noeeprom(0,0,0);
			wait_ms(200);
			rgblight_sethsv_noeeprom(0,255,255);
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
	        rgblight_sethsv_noeeprom(0, 255, 255);
		current_code++;
		if(current_code > 9)
		    current_code = 0;
		dprintf("Current Index: %d - Code: %d\n", pc_index,current_code);
	    }
	    return false;
	}
	switch(secret_stage) {
	    case 0:
                // Selecting password color
		if(long_pressed) {
		    // Select!
		    secret_stage = 1;
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 128);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 190);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 255);
		}
		else {
		    // Cycle
	      	    secret_index++;
	            if(secret_index == secret_count)
		        secret_index = 0;
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 128);
		}
		break;

	    case 1:
		if(long_pressed) {
		    // Back to selecting
		    secret_stage = 0;
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 255);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(200);
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 190);
		    wait_ms(100);
		    rgblight_sethsv_noeeprom(0,0,0);
		    wait_ms(300);
		    secret_index++;
		    if(secret_index == secret_count)
		        secret_index=0;
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 128);
		}
		else {
		    // Send Selected password or username
		    rgblight_sethsv_noeeprom(pw_colors[secret_index], 255, 255);
		    if(double_tap)
		        send_string(pw_passwords[secret_index]);
		    else
		        send_string(pw_usernames[secret_index]);
		}
		break;
	}


    }
    return false;
}

void keyboard_post_init_user(void) {
    debug_enable=true;
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(0, 255, 255);
    rgblight_mode(1);
}
