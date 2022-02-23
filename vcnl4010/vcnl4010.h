#ifndef __VCNL4010_H__
#define __VCNL4010_H__

/* ===== INCLUDES =========================================================== */

#include <stdint.h>

/* ===== DEFINITIONS ======================================================== */

#define VCNL4010_I2C_ADDRESS   (0x13)
#define VCNL4010_I2C_MAX_SPEED (3400000)

#define VCNL4010_PRODUCT_ID      (2)
#define VCNL4010_CONVERSION_TIME (300.0e-6f)

#define VCNL4010_ALIGHT_RESOLUTION_LX (0.25f)
#define VCNL4010_ALIGHT_RAW2PHYS(raw) ((raw)*VCNL4010_ALIGHT_RESOLUTION_LX)

/* ===== TYPES ============================================================== */

typedef enum
{
    VCNL4010_REG_COMMAND         = 0x00,
    VCNL4010_REG_VERSION         = 0x01,
    VCNL4010_REG_PROX_RATE       = 0x02,
    VCNL4010_REG_IR_CURRENT      = 0x03,
    VCNL4010_REG_ALIGHT_PARAM    = 0x04,
    VCNL4010_REG_ALIGHT_RESULT_H = 0x05,
    VCNL4010_REG_ALIGHT_RESULT_L = 0x06,
    VCNL4010_REG_PROX_RESULT_H   = 0x07,
    VCNL4010_REG_PROX_RESULT_L   = 0x08,
    VCNL4010_REG_INT_CTRL        = 0x09,
    VCNL4010_REG_LOW_THRSH_H     = 0x0A,
    VCNL4010_REG_LOW_THRSH_L     = 0x0B,
    VCNL4010_REG_HIGH_THRSH_H    = 0x0C,
    VCNL4010_REG_HIGH_THRSH_L    = 0x0D,
    VCNL4010_REG_INT_STATUS      = 0x0E,
    VCNL4010_REG_PROX_MODULATOR  = 0x0F,
} vcnl4010_reg_t;

typedef int (*vcnl4010_read_t)(uint8_t reg, void* data, uint8_t size);
typedef int (*vcnl4010_write_t)(uint8_t reg, const void* data, uint8_t size);

typedef struct
{
    vcnl4010_read_t read;
    vcnl4010_read_t write;
} vcnl4010_driver_t;

/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== GLOBAL FUNCTIONS PROTOTYPES ======================================== */

typedef struct
{
    uint8_t self_timed_en   : 1;
    uint8_t prox_en         : 1;
    uint8_t alight_en       : 1;
    uint8_t prox_ondemand   : 1;
    uint8_t alight_ondemand : 1;
    uint8_t prox_ready      : 1; // read-only
    uint8_t alight_ready    : 1; // read-only
    uint8_t config_lock     : 1; // read-only
} vcnl4010_reg_command_t;

int vcnl4010_read_command(vcnl4010_driver_t* self, vcnl4010_reg_command_t* value);
int vcnl4010_write_command(vcnl4010_driver_t* self, vcnl4010_reg_command_t value);

typedef union
{
    uint8_t version;

    struct
    {
        uint8_t revision_id : 4;
        uint8_t product_id  : 4; //! \ref vcnl40xx_product_id_t
    };
} vcnl4010_reg_version_t;

int vcnl4010_read_version(vcnl4010_driver_t* self, vcnl4010_reg_version_t* value);

typedef enum
{
    VCNL4010_REG_PROX_RATE_1p95HZ,
    VCNL4010_REG_PROX_RATE_3p90625HZ,
    VCNL4010_REG_PROX_RATE_7p8125HZ,
    VCNL4010_REG_PROX_RATE_16p625HZ,
    VCNL4010_REG_PROX_RATE_31p25HZ,
    VCNL4010_REG_PROX_RATE_62p5HZ,
    VCNL4010_REG_PROX_RATE_125HZ,
    VCNL4010_REG_PROX_RATE_250HZ,
} vcnl4010_reg_prox_rate_t;

int vcnl4010_read_prox_rate(vcnl4010_driver_t* self, vcnl4010_reg_prox_rate_t* value);
int vcnl4010_write_prox_rate(vcnl4010_driver_t* self, vcnl4010_reg_prox_rate_t value);

typedef struct
{
    uint8_t value : 6; //! Physical value is value * 10mA, but limited by 200mA
    uint8_t fuse  : 2;
} vcnl4010_reg_ir_current_t;

int vcnl4010_read_ir_current(vcnl4010_driver_t* self, vcnl4010_reg_ir_current_t* value);
int vcnl4010_write_ir_current(vcnl4010_driver_t* self, vcnl4010_reg_ir_current_t value);

typedef enum
{
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_1,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_2,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_4,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_8,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_16,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_32,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_64,
    VCNL4010_REG_ALIGHT_PARAM_AVERAGE_128,
} vcnl4010_reg_alight_param_average_t;

typedef enum
{
    VCNL4010_REG_ALIGHT_PARAM_RATE_1HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_2HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_3HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_4HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_5HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_6HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_8HZ,
    VCNL4010_REG_ALIGHT_PARAM_RATE_10HZ,
} vcnl4010_reg_alight_param_rate_t;

typedef struct
{
    uint8_t average          : 3; //! \ref vcnl4010_reg_alight_param_average_t
    uint8_t auto_offset_comp : 1;
    uint8_t rate             : 3; //! \ref vcnl4010_reg_alight_param_rate_t
    uint8_t continuous       : 1;
} vcnl4010_reg_alight_param_t;

int vcnl4010_read_alight_param(vcnl4010_driver_t* self, vcnl4010_reg_alight_param_t* value);
int vcnl4010_write_alight_param(vcnl4010_driver_t* self, vcnl4010_reg_alight_param_t value);

int vcnl4010_read_alight_value(vcnl4010_driver_t* self, uint16_t* value);

int vcnl4010_read_prox_value(vcnl4010_driver_t* self, uint16_t* value);

typedef enum
{
    VCNL4010_REG_INT_CTRL_THRSH_SEL_PROX,
    VCNL4010_REG_INT_CTRL_THRSH_SEL_ALIGHT,
} vcnl4010_reg_int_ctrl_thrsh_sel_t;

typedef enum
{
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_1,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_2,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_4,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_8,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_16,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_32,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_64,
    VCNL4010_REG_INT_CTRL_COUNT_EXCEED_128,
} vcnl4010_reg_int_ctrl_count_exceed_t;

typedef struct
{
    uint8_t threshold_sel   : 1; //! \ref vcnl4010_reg_int_ctrl_thrsh_sel_t
    uint8_t threshold_en    : 1;
    uint8_t alight_ready_en : 1;
    uint8_t prox_ready_en   : 1;
    uint8_t                 : 1;
    uint8_t count_exceed    : 3; //! \ref vcnl4010_reg_int_ctrl_count_exceed_t
} vcnl4010_reg_int_ctrl_t;

int vcnl4010_read_int_ctrl(vcnl4010_driver_t* self, vcnl4010_reg_int_ctrl_t* value);
int vcnl4010_write_int_ctrl(vcnl4010_driver_t* self, vcnl4010_reg_int_ctrl_t value);

int vcnl4010_read_low_threashold(vcnl4010_driver_t* self, uint16_t* value);
int vcnl4010_write_low_threashold(vcnl4010_driver_t* self, uint16_t value);

int vcnl4010_read_high_threashold(vcnl4010_driver_t* self, uint16_t* value);
int vcnl4010_write_high_threashold(vcnl4010_driver_t* self, uint16_t value);

typedef struct
{
    uint8_t threshold_high : 1;
    uint8_t threshold_low  : 1;
    uint8_t alight_ready   : 1;
    uint8_t prox_ready     : 1;
    uint8_t                : 4;
} vcnl4010_reg_int_status_t;

int vcnl4010_read_int_status(vcnl4010_driver_t* self, vcnl4010_reg_int_status_t* value);
int vcnl4010_write_int_status(vcnl4010_driver_t* self, vcnl4010_reg_int_status_t value);

typedef enum
{
    VCNL4010_REG_PROX_MODULATOR_FREQ_390p625kHz,
    VCNL4010_REG_PROX_MODULATOR_FREQ_781p25kHz,
    VCNL4010_REG_PROX_MODULATOR_FREQ_1p5625MHZ,
    VCNL4010_REG_PROX_MODULATOR_FREQ_3p125MHz,

    VCNL4010_REG_PROX_MODULATOR_FREQ_MIN = VCNL4010_REG_PROX_MODULATOR_FREQ_390p625kHz,
    VCNL4010_REG_PROX_MODULATOR_FREQ_MAX = VCNL4010_REG_PROX_MODULATOR_FREQ_3p125MHz,
} vcnl4010_reg_prox_modulator_freq_t;

typedef struct
{
    uint8_t dead_time  : 3;
    uint8_t freq       : 2; //! \ref vcnl4010_reg_prox_modulator_freq_t
    uint8_t delay_time : 3;
} vcnl4010_reg_prox_modulator_t;

int vcnl4010_read_prox_modulator(vcnl4010_driver_t* self, vcnl4010_reg_prox_modulator_t* value);
int vcnl4010_write_prox_modulator(vcnl4010_driver_t* self, vcnl4010_reg_prox_modulator_t value);

#endif /* __VCNL4010_H__ */
