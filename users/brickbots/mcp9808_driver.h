#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MCP9808_I2CADDR_DEFAULT 0x18 ///< MCP9080 default addres
#define MCP9808_REG_CONFIG 0x01      ///< MCP9808 config register

#define MCP9808_REG_AMBIENT_TEMP 0x05 ///< ambient temperature
#define MCP9808_REG_MANUF_ID 0x06     ///< manufacture ID
#define MCP9808_REG_DEVICE_ID 0x07    ///< device ID
#define MCP9808_REG_RESOLUTION 0x08   ///< resolutin


bool mcp9808_init(void);
float mcp9808_read_raw(void);
void mcp9808_read_str(char* temp_str);

uint16_t mcp9808_read16(uint8_t reg);

