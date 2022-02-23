#ifndef __VCNL4000_H__
#define __VCNL4000_H__

/* ===== INCLUDES =========================================================== */

#include <stdint.h>

/* ===== DEFINITIONS ======================================================== */

#define VCNL4000_I2C_ADDRESS   (0x13)
#define VCNL4000_I2C_MAX_SPEED (3400000)

#define VCNL4000_PRODUCT_ID      (1)
#define VCNL4000_CONVERSION_TIME (300.0e-6f)

#define VCNL4000_ALIGHT_RESOLUTION_LX (0.25f)
#define VCNL4000_ALIGHT_RAW2PHYS(raw) ((raw)*VCNL4000_ALIGHT_RESOLUTION_LX)

/* ===== TYPES ============================================================== */

typedef enum
{
    VCNL4000_REG_COMMAND         = 0x00,
    VCNL4000_REG_VERSION         = 0x01,
    VCNL4000_REG_IR_CURRENT      = 0x03,
    VCNL4000_REG_ALIGHT_PARAM    = 0x04,
    VCNL4000_REG_ALIGHT_RESULT_H = 0x05,
    VCNL4000_REG_ALIGHT_RESULT_L = 0x06,
    VCNL4000_REG_PROX_RESULT_H   = 0x07,
    VCNL4000_REG_PROX_RESULT_L   = 0x08,
    VCNL4000_REG_PROX_FREQ       = 0x09,
    VCNL4000_REG_PROX_MODULATOR  = 0x0A,
} vcnl4000_reg_t;

typedef int (*vcnl4000_read_t)(uint8_t reg, void* data, uint8_t size);
typedef int (*vcnl4000_write_t)(uint8_t reg, const void* data, uint8_t size);

typedef struct
{
    vcnl4000_read_t read;
    vcnl4000_read_t write;
} vcnl4000_driver_t;

/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== GLOBAL FUNCTIONS PROTOTYPES ======================================== */

typedef struct
{
    uint8_t                 : 3;
    uint8_t prox_ondemand   : 1;
    uint8_t alight_ondemand : 1;
    uint8_t prox_ready      : 1; // read-only
    uint8_t alight_ready    : 1; // read-only
    uint8_t config_lock     : 1; // read-only
} vcnl4000_reg_command_t;

int vcnl4000_read_command(vcnl4000_driver_t* self, vcnl4000_reg_command_t* value);
int vcnl4000_write_command(vcnl4000_driver_t* self, vcnl4000_reg_command_t value);

typedef union
{
    uint8_t version;

    struct
    {
        uint8_t revision_id : 4;
        uint8_t product_id  : 4; //! \ref vcnl40xx_product_id_t
    };
} vcnl4000_reg_version_t;

int vcnl4000_read_version(vcnl4000_driver_t* self, vcnl4000_reg_version_t* value);

typedef struct
{
    uint8_t value : 6; //! Physical value is value * 10mA, but limited by 200mA
    uint8_t fuse  : 2;
} vcnl4000_reg_ir_current_t;

int vcnl4000_read_ir_current(vcnl4000_driver_t* self, vcnl4000_reg_ir_current_t* value);
int vcnl4000_write_ir_current(vcnl4000_driver_t* self, vcnl4000_reg_ir_current_t value);

typedef enum
{
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_1,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_2,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_4,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_8,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_16,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_32,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_64,
    VCNL4000_REG_ALIGHT_PARAM_AVERAGE_128,
} vcnl4000_reg_alight_param_average_t;

typedef struct
{
    uint8_t average          : 3; //! \ref vcnl4000_reg_alight_param_average_t
    uint8_t auto_offset_comp : 1;
    uint8_t                  : 3;
    uint8_t continuous       : 1;
} vcnl4000_reg_alight_param_t;

int vcnl4000_read_alight_param(vcnl4000_driver_t* self, vcnl4000_reg_alight_param_t* value);
int vcnl4000_write_alight_param(vcnl4000_driver_t* self, vcnl4000_reg_alight_param_t value);

int vcnl4000_read_alight_value(vcnl4000_driver_t* self, uint16_t* value);

int vcnl4000_read_prox_value(vcnl4000_driver_t* self, uint16_t* value);

typedef enum
{
    VCNL4000_REG_PROX_FREQ_3p125MHz,
    VCNL4000_REG_PROX_FREQ_1p5625MHZ,
    VCNL4000_REG_PROX_FREQ_781p25kHz,
    VCNL4000_REG_PROX_FREQ_390p625kHz,

    VCNL4000_REG_PROX_FREQ_MIN = VCNL4000_REG_PROX_FREQ_390p625kHz,
    VCNL4000_REG_PROX_FREQ_MAX = VCNL4000_REG_PROX_FREQ_3p125MHz,
} vcnl4000_reg_prox_freq_t;

int vcnl4000_read_prox_freq(vcnl4000_driver_t* self, vcnl4000_reg_prox_freq_t* value);
int vcnl4000_write_prox_freq(vcnl4000_driver_t* self, vcnl4000_reg_prox_freq_t value);

typedef struct
{
    uint8_t dead_time : 3;
    uint8_t           : 2;
    uint8_t delay     : 3;
} vcnl4000_reg_prox_modulator_t;

int vcnl4000_read_prox_modulator(vcnl4000_driver_t* self, vcnl4000_reg_prox_modulator_t* value);
int vcnl4000_write_prox_modulator(vcnl4000_driver_t* self, vcnl4000_reg_prox_modulator_t value);

#endif /* __VCNL4000_H__ */
