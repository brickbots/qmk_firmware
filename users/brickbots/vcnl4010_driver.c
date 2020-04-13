
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "vcnl4010_driver.h"
#include "i2c_master.h"
#include "print.h"

#if defined(__AVR__)
// already defined on ARM
#    define I2C_TIMEOUT 100
#endif  // defined(__AVR__)
#define I2C_WRITE_REG(mode, data) i2c_writeReg((VCNL4010_I2CADDR_DEFAULT << 1), mode, &data[0], sizeof(data), I2C_TIMEOUT)
#define I2C_READ_REG(mode, data) i2c_readReg((VCNL4010_I2CADDR_DEFAULT << 1), mode, &data[0], sizeof(data), I2C_TIMEOUT)

uint16_t vcnl4010_poll_timer = 0;

uint8_t vcnl4010_read8(uint8_t reg) {
    uint8_t tmpbytes[1];

    if (I2C_READ_REG(reg, tmpbytes) == I2C_STATUS_SUCCESS) {
	return tmpbytes[0];
    }

    return 0xFF;
}

uint16_t vcnl4010_read16(uint8_t reg) {
    uint16_t val = 0xFFFF;
    uint8_t tmpbytes[2];

    if (I2C_READ_REG(reg, tmpbytes) == I2C_STATUS_SUCCESS) {
      val = tmpbytes[0];
      val <<= 8;
      val |= tmpbytes[1];
    }

    return val;
}

bool vcnl4010_init(void) {
    i2c_init();
    if((vcnl4010_read8(VCNL4010_PRODUCTID) & 0xF0) != 0x20) {
        print("VCNL: Product ID Mismatch");
        return false;
    }

    //set led power to highest (200ma)
    uint8_t led_power[1] = {20};
    if(I2C_WRITE_REG(VCNL4010_IRLED, led_power) != I2C_STATUS_SUCCESS) {
        print("MCP: Could not set led power \n");
        return false;
    }

    //set frequency for proxmity reading
    uint8_t freq[1] = {0x02};
    if(I2C_WRITE_REG(VCNL4010_PROXRATE, freq) != I2C_STATUS_SUCCESS) {
        print("MCP: Could not setup proximity sampling rate\n");
        return false;
    }

/*
    //set periodic proxmity reading
    uint8_t command[1] = {0x02};
    if(I2C_WRITE_REG(VCNL4010_COMMAND, command) != I2C_STATUS_SUCCESS) {
        print("MCP: Could not setup command register\n");
        return false;
    }
*/
    return true;

}

// Because this takes some ms, even at low res, only do it every 1/2 seconds
uint16_t vcnl4010_read_raw(void) {
    static uint16_t raw_proximity = 1;
    if (vcnl4010_poll_timer == 0) {
	vcnl4010_poll_timer = timer_read();
	return raw_proximity;
    }
    if(timer_elapsed(vcnl4010_poll_timer) > 500) {
	vcnl4010_poll_timer = timer_read();

        uint8_t command[1] = {0x08};
        if(I2C_WRITE_REG(VCNL4010_COMMAND, command) != I2C_STATUS_SUCCESS)
	      return 9999;
        while (1) {
	    uint8_t result = vcnl4010_read8(VCNL4010_COMMAND);
	    if (result & 0x20) {
	        raw_proximity = vcnl4010_read16(VCNL4010_PROXIMITYDATA);
	        return raw_proximity;
	    }
        }
    }
    return raw_proximity;
}

