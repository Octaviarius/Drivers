
/* ===== INCLUDES =========================================================== */

#include "vcnl4000.h"

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

int vcnl4000_read_command(vcnl4000_driver_t* self, vcnl4000_reg_command_t* value)
{
    return self->read(VCNL4000_REG_COMMAND, value, 1);
}
int vcnl4000_write_command(vcnl4000_driver_t* self, vcnl4000_reg_command_t value)
{
    return self->write(VCNL4000_REG_COMMAND, &value, 1);
}

int vcnl4000_read_version(vcnl4000_driver_t* self, vcnl4000_reg_version_t* value)
{
    return self->read(VCNL4000_REG_VERSION, value, 1);
}

int vcnl4000_read_ir_current(vcnl4000_driver_t* self, vcnl4000_reg_ir_current_t* value)
{
    return self->read(VCNL4000_REG_IR_CURRENT, value, 1);
}
int vcnl4000_write_ir_current(vcnl4000_driver_t* self, vcnl4000_reg_ir_current_t value)
{
    return self->write(VCNL4000_REG_IR_CURRENT, &value, 1);
}

int vcnl4000_read_alight_param(vcnl4000_driver_t* self, vcnl4000_reg_alight_param_t* value)
{
    return self->read(VCNL4000_REG_ALIGHT_PARAM, value, 1);
}
int vcnl4000_write_alight_param(vcnl4000_driver_t* self, vcnl4000_reg_alight_param_t value)
{
    return self->write(VCNL4000_REG_ALIGHT_PARAM, &value, 1);
}

int vcnl4000_read_alight_value(vcnl4000_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4000_REG_ALIGHT_RESULT_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}

int vcnl4000_read_prox_value(vcnl4000_driver_t* self, uint16_t* value)
{
    int res;

    res = self->read(VCNL4000_REG_PROX_RESULT_H, value, 2);

    *value = FROM_BE16(*value);

    return res
}

int vcnl4000_read_prox_freq(vcnl4000_driver_t* self, vcnl4000_reg_prox_freq_t* value)
{
    return self->read(VCNL4000_REG_PROX_FREQ, value, 1);
}
int vcnl4000_write_prox_freq(vcnl4000_driver_t* self, vcnl4000_reg_prox_freq_t value)
{
    return self->write(VCNL4000_REG_PROX_FREQ, &value, 1);
}

int vcnl4000_read_prox_modulator(vcnl4000_driver_t* self, vcnl4000_reg_prox_modulator_t* value)
{
    return self->read(VCNL4000_REG_PROX_MOD, value, 1);
}
int vcnl4000_write_prox_modulator(vcnl4000_driver_t* self, vcnl4000_reg_prox_modulator_t value)
{
    return self->write(VCNL4000_REG_PROX_MOD, &value, 1);
}

/* ===== LOCAL FUNCTIONS IMPLEMENTATION ===================================== */
