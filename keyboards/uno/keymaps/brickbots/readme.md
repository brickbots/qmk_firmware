# Uno: Brickbots - A toy password dongle made with Uno!

Copyright 2020 - Richard Sutherland (rich@brickbots.com).  Permission is
granted to alter/copy/re-use in any way you see fit.

*NOTE:*  Please don't use this code/system for storing anything important.
Minor attempts have been made to avoid big mistakes like committing your
passwords to git, but they are stored un-encrypted in the MCU eeprom
and can be recovered with physical access to the device.

## Intro and setup
This keymap implements a 'password manager' of sorts with passcode
protection.  Username/password combos and the unlock code are defined in the
secrets.h file. You can replace the included secrets.h file with your own, which
should live and be included from OUTSIDE the qmk_firmware tree.  See the comments in the
source/secrets file for more details.

## Unlock
To unlock the device and allow password access you'll need to enter your
four digit passcode. For each digit tap the key quickly to enter
the number (i.e. 5 taps to enter the number 5), then hold down for 300ms
to advance to the next digit in the code and repeat. If successful, the LED
will turn green, if you have made a mistake, you'll get a few flashes of 
red color and you can try again.

## Enter Passwords
Once unlocked, you can cycle between as many username/password combos as you
have setup, each represented by a color defined along with the information in
secrets.h.  Tap to cycle through them, once you have the one you want,
long-press (300ms) to select. You'll see a flash and the color will get
brighter.  Once selected, a single tap will send the
username, and a double tap the password.  A long-press will get you back to
username/password selection.

## Reset / Relock
A very long press (>3 seconds) will reset and re-lock the device at any time.
If left idle for 5 minutes, the device will auto-relock.


