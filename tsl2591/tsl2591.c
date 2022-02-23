
/* ===== INCLUDES =========================================================== */

#include "tsl2591.h"

/* ===== DEFINITIONS ======================================================== */

#define IS_BIG_ENDIAN   \
    (!(union {          \
          uint16_t u16; \
          uint8_t c;    \
      }){.u16 = 0x0001} \
          .c)

#define IS_LITTLE_ENDIAN \
    (!(union {           \
          uint16_t u16;  \
          uint8_t c;     \
      }){.u16 = 0x0100}  \
          .c)

#define FROM_LE16(val) (IS_LITTLE_ENDIAN ? (val) : (((uint16_t)(val) >> 8) | ((uint16_t)(val) << 8)))

/* ===== TYPES ============================================================== */
/* ===== LOCAL FUNCTIONS PROTOTYPES ========================================= */
/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== LOCAL VARIABLES ==================================================== */
/* ===== GLOBAL FUNCTIONS IMPLEMENTATION ==================================== */

int tsl2591_read_enable(tsl2591_driver_t* self, tsl2591_reg_enable_t* value)
{
    return self->read(TSL2591_REG_ENABLE, value, 1);
}
int tsl2591_write_enable(tsl2591_driver_t* self, tsl2591_reg_enable_t value)
{
    return self->write(TSL2591_REG_ENABLE, &value, 1);
}

int tsl2591_read_config(tsl2591_driver_t* self, tsl2591_reg_config_t* value)
{
    return self->read(TSL2591_REG_CONFIG, value, 1);
}
int tsl2591_write_config(tsl2591_driver_t* self, tsl2591_reg_config_t value)
{
    return self->write(TSL2591_REG_CONFIG, &value, 1);
}

int tsl2591_read_int_low_threashold(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_INT_LOW_THRESHOLD_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}
int tsl2591_write_int_low_threashold(tsl2591_driver_t* self, uint16_t value)
{
    int res;
    value = FROM_LE16(value);
    res   = self->write(TSL2591_REG_INT_LOW_THRESHOLD_L, &value, 2);
    return res
}
int tsl2591_read_int_high_threashold(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_INT_HIGH_THRESHOLD_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}
int tsl2591_write_int_high_threashold(tsl2591_driver_t* self, uint16_t value)
{
    int res;
    value = FROM_LE16(value);
    res   = self->write(TSL2591_REG_INT_HIGH_THRESHOLD_L, &value, 2);
    return res
}

int tsl2591_read_np_int_low_threashold(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_NP_INT_LOW_THRESHOLD_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}
int tsl2591_write_np_int_low_threashold(tsl2591_driver_t* self, uint16_t value)
{
    int res;
    value = FROM_LE16(value);
    res   = self->write(TSL2591_REG_NP_INT_LOW_THRESHOLD_L, &value, 2);
    return res
}
int tsl2591_read_np_int_high_threashold(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_NP_INT_HIGH_THRESHOLD_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}
int tsl2591_write_np_int_high_threashold(tsl2591_driver_t* self, uint16_t value)
{
    int res;
    value = FROM_LE16(value);
    res   = self->write(TSL2591_REG_NP_INT_HIGH_THRESHOLD_L, &value, 2);
    return res
}

int tsl2591_read_int_pers_filter(tsl2591_driver_t* self, tsl2591_reg_int_pers_filter_t* value)
{
    return self->read(TSL2591_REG_INT_PERS_FILTER, value, 1);
}
int tsl2591_write_int_pers_filter(tsl2591_driver_t* self, tsl2591_reg_int_pers_filter_t value)
{
    return self->write(TSL2591_REG_INT_PERS_FILTER, &value, 1);
}

int tsl2591_read_package_id(tsl2591_driver_t* self, tsl2591_reg_package_id_t* value)
{
    return self->read(TSL2591_REG_PACKAGE_ID, value, 1);
}

int tsl2591_read_device_id(tsl2591_driver_t* self, uint8_t* value)
{
    return self->read(TSL2591_REG_DEVICE_ID, value, 1);
}

int tsl2591_read_status(tsl2591_driver_t* self, tsl2591_reg_status_t* value)
{
    return self->read(TSL2591_REG_STATUS, value, 1);
}

int tsl2591_read_channel0(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_CHANNEL0_DATA_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}
int tsl2591_read_channel1(tsl2591_driver_t* self, uint16_t* value)
{
    int res;
    res    = self->read(TSL2591_REG_CHANNEL1_DATA_L, value, 2);
    *value = FROM_LE16(*value);
    return res
}

/* ===== LOCAL FUNCTIONS IMPLEMENTATION ===================================== */
