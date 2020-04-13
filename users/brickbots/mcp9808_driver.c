
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "mcp9808_driver.h"
#include "i2c_master.h"
#include "print.h"

#if defined(__AVR__)
// already defined on ARM
#    define I2C_TIMEOUT 100
#endif  // defined(__AVR__)
#define I2C_WRITE_REG(mode, data) i2c_writeReg((MCP9808_I2CADDR_DEFAULT << 1), mode, &data[0], sizeof(data), I2C_TIMEOUT)
#define I2C_READ_REG(mode, data) i2c_readReg((MCP9808_I2CADDR_DEFAULT << 1), mode, &data[0], sizeof(data), I2C_TIMEOUT)

uint16_t mcp9808_poll_timer = 0;

uint16_t mcp9808_read16(uint8_t reg) {
    uint16_t val = 0xFFFF;
    uint8_t tmpbytes[2];

    if (I2C_READ_REG(reg, tmpbytes) == I2C_STATUS_SUCCESS) {
      val = tmpbytes[0];
      val <<= 8;
      val |= tmpbytes[1];
    }

    return val;
}

bool mcp9808_init(void) {
    i2c_init();
    if(mcp9808_read16(MCP9808_REG_MANUF_ID) != 0x0054) {
        print("MCP: Manufacturer ID Mismatch");
        return false;
    }
    if(mcp9808_read16(MCP9808_REG_DEVICE_ID) != 0x0400) {
        print("MCP: Device ID Mismatch");
        return false;
    }

    uint8_t tmp[2] = {0x0,0x0};
    if(I2C_WRITE_REG(MCP9808_REG_CONFIG, tmp) != I2C_STATUS_SUCCESS) {
        print("MCP: Could not set reg config \n");
        return false;
    }

    //set resoultion to lowest/fastest
    uint8_t resolution[1] = {0x03};
    if(I2C_WRITE_REG(MCP9808_REG_RESOLUTION, resolution) != I2C_STATUS_SUCCESS) {
        print("MCP: Could not set resolution \n");
        return false;
    }

    return true;

}
// Because this takes some ms, even at low res, only do it every 2 seconds
float mcp9808_read_raw(void) {
    static float raw_temp = 0.1;
    if (mcp9808_poll_timer == 0) {
	mcp9808_poll_timer = timer_read();
	return raw_temp;
    }
    if(timer_elapsed(mcp9808_poll_timer) > 2000) {
	mcp9808_poll_timer = timer_read();

        uint16_t t = mcp9808_read16(MCP9808_REG_AMBIENT_TEMP);

        if (t != 0xFFFF) {
          raw_temp = t & 0x0FFF;
          raw_temp /= 16.0;
          if (t & 0x1000)
            raw_temp -= 256;
        }
    }
    return raw_temp;
}

void mcp9808_read_str(char* temp_string) {
    uint16_t raw_temp = (uint16_t) (mcp9808_read_raw() * 10);

    sprintf(temp_string, "%02d.%d", (uint8_t)(raw_temp / 10), raw_temp % 10);

}
