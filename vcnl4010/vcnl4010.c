
/* ===== INCLUDES =========================================================== */

#include "vcnl4010.h"

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

#define FROM_BE16(val) (IS_BIG_ENDIAN ? (val) : (((uint16_t)(val) >> 8) | ((uint16_t)(val) << 8)))

/* ===== TYPES ============================================================== */
/* ===== LOCAL FUNCTIONS PROTOTYPES ========================================= */
/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== LOCAL VARIABLES ==================================================== */
/* ===== GLOBAL FUNCTIONS IMPLEMENTATION ==================================== */

int vcnl4010_read_command(vcnl4010_driver_t* self, vcnl4010_reg_command_t* value)
{
    return self->read(VCNL4010_REG_COMMAND, value, 1);
}
int vcnl4010_write_command(vcnl4010_driver_t* self, vcnl4010_reg_command_t value)
{
    return self->write(VCNL4010_REG_COMMAND, &value, 1);
}

int vcnl4010_read_version(vcnl4010_driver_t* self, vcnl4010_reg_version_t* value)
{
    return self->read(VCNL4010_REG_VERSION, value, 1);
}

int vcnl4010_read_prox_rate(vcnl4010_driver_t* self, vcnl4010_reg_prox_rate_t* value)
{
    return self->read(VCNL4010_REG_PROX_RATE, value, 1);
}
int vcnl4010_write_prox_rate(vcnl4010_driver_t* self, vcnl4010_reg_prox_rate_t value)
{
    return self->write(VCNL4010_REG_PROX_RATE, &value, 1);
}

int vcnl4010_read_ir_current(vcnl4010_driver_t* self, vcnl4010_reg_ir_current_t* value)
{
    return self->read(VCNL4010_REG_IR_CURRENT, value, 1);
}
int vcnl4010_write_ir_current(vcnl4010_driver_t* self, vcnl4010_reg_ir_current_t value)
{
    return self->write(VCNL4010_REG_IR_CURRENT, &value, 1);
}

int vcnl4010_read_alight_param(vcnl4010_driver_t* self, vcnl4010_reg_alight_param_t* value)
{
    return self->read(VCNL4010_REG_ALIGHT_PARAM, value, 1);
}
int vcnl4010_write_alight_param(vcnl4010_driver_t* self, vcnl4010_reg_alight_param_t value)
{
    return self->write(VCNL4010_REG_ALIGHT_PARAM, &value, 1);
}

int vcnl4010_read_alight_value(vcnl4010_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4010_REG_ALIGHT_RESULT_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}

int vcnl4010_read_prox_value(vcnl4010_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4010_REG_PROX_RESULT_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}

int vcnl4010_read_int_ctrl(vcnl4010_driver_t* self, vcnl4010_reg_int_ctrl_t* value)
{
    return self->read(VCNL4010_REG_INT_CTRL, value, 1);
}
int vcnl4010_write_int_ctrl(vcnl4010_driver_t* self, vcnl4010_reg_int_ctrl_t value)
{
    return self->write(VCNL4010_REG_INT_CTRL, &value, 1);
}

int vcnl4010_read_low_threashold(vcnl4010_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4010_REG_LOW_THRSH_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}
int vcnl4010_write_low_threashold(vcnl4010_driver_t* self, uint16_t value)
{
    int res;

    value = FROM_BE16(value);

    res = self->write(VCNL4010_REG_LOW_THRSH_H, &value, 2);

    return res
}

int vcnl4010_read_high_threashold(vcnl4010_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4010_REG_HIGH_THRSH_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}
int vcnl4010_write_high_threashold(vcnl4010_driver_t* self, uint16_t value)
{
    int res;

    value = FROM_BE16(value);

    res = self->write(VCNL4010_REG_HIGH_THRSH_H, &value, 2);

    return res
}

int vcnl4010_read_int_status(vcnl4010_driver_t* self, vcnl4010_reg_int_status_t* value)
{
    return self->read(VCNL4010_REG_INT_STATUS, value, 1);
}
int vcnl4010_write_int_status(vcnl4010_driver_t* self, vcnl4010_reg_int_status_t value)
{
    return self->write(VCNL4010_REG_INT_STATUS, &value, 1);
}

int vcnl4010_read_prox_modulator(vcnl4010_driver_t* self, vcnl4010_reg_prox_modulator_t* value)
{
    return self->read(VCNL4010_REG_PROX_MODULATOR, value, 1);
}
int vcnl4010_write_prox_modulator(vcnl4010_driver_t* self, vcnl4010_reg_prox_modulator_t value)
{
    return self->write(VCNL4010_REG_PROX_MODULATOR, &value, 1);
}

/* ===== LOCAL FUNCTIONS IMPLEMENTATION ===================================== */
