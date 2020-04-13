#pragma once
#include <stdint.h>
#include <stdbool.h>

#define VCNL4010_I2CADDR_DEFAULT 0x13		///< I2C address of the sensor

/** Registers */
#define VCNL4010_COMMAND 0x80           ///< Command
#define VCNL4010_PRODUCTID 0x81         ///< Product ID Revision
#define VCNL4010_PROXRATE 0x82          ///< Proximity rate
#define VCNL4010_IRLED 0x83             ///< IR LED current
#define VCNL4010_AMBIENTPARAMETER 0x84  ///< Ambient light parameter
#define VCNL4010_AMBIENTDATA 0x85       ///< Ambient light result (16 bits)
#define VCNL4010_PROXIMITYDATA 0x87     ///< Proximity result (16 bits)


bool vcnl4010_init(void);
uint16_t vcnl4010_read_raw(void);

uint16_t vcnl4010_read16(uint8_t reg);

