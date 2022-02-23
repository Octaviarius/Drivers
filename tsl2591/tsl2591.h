#ifndef __TSL2591_H__
#define __TSL2591_H__

/* ===== INCLUDES =========================================================== */

#include <stdint.h>

/* ===== DEFINITIONS ======================================================== */

#define TSL2591_I2C_ADDRESS 0x29

#define TSL2591_CHANNEL0_RESOLUTION_WHITE_WM2 3.786444e-5f
#define TSL2591_CHANNEL0_RESOLUTION_850NM_WM2 3.883495e-5f
#define TSL2591_CHANNEL0_GAIN_LOW             1.0f
#define TSL2591_CHANNEL0_GAIN_MEDIUM          24.5f
#define TSL2591_CHANNEL0_GAIN_HIGH            400.0f
#define TSL2591_CHANNEL0_GAIN_MAX             9200.0f

#define TSL2591_CHANNEL1_RESOLUTION_WHITE_WM2 2.865329e-4f
#define TSL2591_CHANNEL1_RESOLUTION_850NM_WM2 6.493506e-5f
#define TSL2591_CHANNEL1_GAIN_LOW             1.0f
#define TSL2591_CHANNEL1_GAIN_MEDIUM          24.5f
#define TSL2591_CHANNEL1_GAIN_HIGH            400.0f
#define TSL2591_CHANNEL1_GAIN_MAX             9900.0f

/* ===== TYPES ============================================================== */

typedef enum
{
    TSL2591_REG_ENABLE                  = 0x00,
    TSL2591_REG_CONFIG                  = 0x01,
    TSL2591_REG_INT_LOW_THRESHOLD_L     = 0x04,
    TSL2591_REG_INT_LOW_THRESHOLD_H     = 0x05,
    TSL2591_REG_INT_HIGH_THRESHOLD_L    = 0x06,
    TSL2591_REG_INT_HIGH_THRESHOLD_H    = 0x07,
    TSL2591_REG_NP_INT_LOW_THRESHOLD_L  = 0x08,
    TSL2591_REG_NP_INT_LOW_THRESHOLD_H  = 0x09,
    TSL2591_REG_NP_INT_HIGH_THRESHOLD_L = 0x0A,
    TSL2591_REG_NP_INT_HIGH_THRESHOLD_H = 0x0B,
    TSL2591_REG_INT_PERS_FILTER         = 0x0C,
    TSL2591_REG_PACKAGE_ID              = 0x11,
    TSL2591_REG_DEVICE_ID               = 0x12,
    TSL2591_REG_STATUS                  = 0x13,
    TSL2591_REG_CHANNEL0_DATA_L         = 0x14,
    TSL2591_REG_CHANNEL0_DATA_H         = 0x15,
    TSL2591_REG_CHANNEL1_DATA_L         = 0x16,
    TSL2591_REG_CHANNEL1_DATA_H         = 0x17,
} tsl2591_reg_t;

typedef int (*tsl2591_read_t)(uint8_t reg, void* data, uint8_t size);
typedef int (*tsl2591_write_t)(uint8_t reg, const void* data, uint8_t size);

typedef struct
{
    tsl2591_read_t read;
    tsl2591_read_t write;
} tsl2591_driver_t;

/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== GLOBAL FUNCTIONS PROTOTYPES ======================================== */

typedef struct
{
    uint8_t power_on        : 1;
    uint8_t als             : 1;
    uint8_t                 : 2;
    uint8_t interrupt       : 1;
    uint8_t                 : 1;
    uint8_t sleep_after_int : 1;
    uint8_t no_persist_int  : 1;
} tsl2591_reg_enable_t;

int tsl2591_read_enable(tsl2591_driver_t* self, tsl2591_reg_enable_t* value);
int tsl2591_write_enable(tsl2591_driver_t* self, tsl2591_reg_enable_t value);

typedef enum
{
    TSL2591_REG_CONFIG_INTEGR_TIME_100ms,
    TSL2591_REG_CONFIG_INTEGR_TIME_200ms,
    TSL2591_REG_CONFIG_INTEGR_TIME_300ms,
    TSL2591_REG_CONFIG_INTEGR_TIME_400ms,
    TSL2591_REG_CONFIG_INTEGR_TIME_500ms,
    TSL2591_REG_CONFIG_INTEGR_TIME_600ms,
} tsl2591_reg_config_integr_time_t;

typedef enum
{
    TSL2591_REG_CONFIG_GAIN_LOW,
    TSL2591_REG_CONFIG_GAIN_MEDIUM,
    TSL2591_REG_CONFIG_GAIN_HIGH,
    TSL2591_REG_CONFIG_GAIN_MAX,
} tsl2591_reg_config_gain_t;

typedef struct
{
    uint8_t integr_time : 3; //! \ref tsl2591_reg_config_integr_time_t
    uint8_t             : 1;
    uint8_t gain        : 2; //! \ref tsl2591_reg_config_gain_t
    uint8_t             : 1;
    uint8_t sw_reset    : 1;
} tsl2591_reg_config_t;

int tsl2591_read_config(tsl2591_driver_t* self, tsl2591_reg_config_t* value);
int tsl2591_write_config(tsl2591_driver_t* self, tsl2591_reg_config_t value);

int tsl2591_read_int_low_threashold(tsl2591_driver_t* self, uint16_t* value);
int tsl2591_write_int_low_threashold(tsl2591_driver_t* self, uint16_t value);
int tsl2591_read_int_high_threashold(tsl2591_driver_t* self, uint16_t* value);
int tsl2591_write_int_high_threashold(tsl2591_driver_t* self, uint16_t value);

int tsl2591_read_np_int_low_threashold(tsl2591_driver_t* self, uint16_t* value);
int tsl2591_write_np_int_low_threashold(tsl2591_driver_t* self, uint16_t value);
int tsl2591_read_np_int_high_threashold(tsl2591_driver_t* self, uint16_t* value);
int tsl2591_write_np_int_high_threashold(tsl2591_driver_t* self, uint16_t value);

typedef enum
{
    TSL2591_REG_INT_PERS_FILTER_EVERY,
    TSL2591_REG_INT_PERS_FILTER_OUTSIDE_OF_THRSH,
    TSL2591_REG_INT_PERS_FILTER_2,
    TSL2591_REG_INT_PERS_FILTER_3,
    TSL2591_REG_INT_PERS_FILTER_5,
    TSL2591_REG_INT_PERS_FILTER_10,
    TSL2591_REG_INT_PERS_FILTER_15,
    TSL2591_REG_INT_PERS_FILTER_20,
    TSL2591_REG_INT_PERS_FILTER_25,
    TSL2591_REG_INT_PERS_FILTER_30,
    TSL2591_REG_INT_PERS_FILTER_35,
    TSL2591_REG_INT_PERS_FILTER_40,
    TSL2591_REG_INT_PERS_FILTER_45,
    TSL2591_REG_INT_PERS_FILTER_50,
    TSL2591_REG_INT_PERS_FILTER_55,
    TSL2591_REG_INT_PERS_FILTER_60,
} tsl2591_reg_int_pers_filter_t;

int tsl2591_read_int_pers_filter(tsl2591_driver_t* self, tsl2591_reg_int_pers_filter_t* value);
int tsl2591_write_int_pers_filter(tsl2591_driver_t* self, tsl2591_reg_int_pers_filter_t value);

typedef struct
{
    uint8_t    : 4;
    uint8_t id : 2;
    uint8_t    : 2;
} tsl2591_reg_package_id_t;

int tsl2591_read_package_id(tsl2591_driver_t* self, tsl2591_reg_package_id_t* value);

int tsl2591_read_device_id(tsl2591_driver_t* self, uint8_t* value);

typedef struct
{
    uint8_t valid        : 1;
    uint8_t              : 3;
    uint8_t interrupt    : 1;
    uint8_t np_interrupt : 1;
    uint8_t              : 2;
} tsl2591_reg_status_t;

int tsl2591_read_status(tsl2591_driver_t* self, tsl2591_reg_status_t* value);

int tsl2591_read_channel0(tsl2591_driver_t* self, uint16_t* value);
int tsl2591_read_channel1(tsl2591_driver_t* self, uint16_t* value);

#endif /* __TSL2591_H__ */
