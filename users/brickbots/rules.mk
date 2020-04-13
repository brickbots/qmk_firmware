ifeq ($(strip $(MCP9808_ENABLE)), yes)
    SRC += mcp9808_driver.c
    OPT_DEFS += -DMCP9808_ENABLE
endif

ifeq ($(strip $(VCNL4010_ENABLE)), yes)
    SRC += vcnl4010_driver.c
    OPT_DEFS += -DVCNL4010_ENABLE
endif
