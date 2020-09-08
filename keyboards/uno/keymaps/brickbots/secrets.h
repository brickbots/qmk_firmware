/*
Copy this file and store it in your HOME directory
Do not store this along with your keymap file as you 
will almost certainly check it into git at some point and the world will know
your secrets.

*/

// This is the 4 digit passcode to unlock the device
const uint8_t passcode[4] = {1,2,3,4};

// This must match the number of secrets in the arrays
// below
const uint8_t secret_count = 4;

// Each of the arrays below must be the same length
// You can adjust the length of each string as needed
static char pw_usernames[4][30] = {
                         "joeuser",
                         "janedoe",
                         "luggage",
                         "root"
                     };

static char pw_passwords[4][30] = {
                         "cat",
                         "dog",
                         "12345",
                         "password"
                     };

static uint8_t pw_colors[4] = {
                       51, 102, 153, 204
                     };
