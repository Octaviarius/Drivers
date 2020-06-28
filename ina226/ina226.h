#pragma once

#include <stdint.h>

#define INA226_REG_CONFIGURATION 0x00
#define INA226_REG_SHUNT_VOLTAGE 0x01
#define INA226_REG_BUS_VOLTAGE   0x02
#define INA226_REG_POWER         0x03
#define INA226_REG_CURRENT       0x04
#define INA226_REG_CALIBRATION   0x05
#define INA226_REG_MASK_ENABLE   0x06
#define INA226_REG_ALERT_LIMIT   0x07
#define INA226_REG_MANUFACTURER  0xFE
#define INA226_REG_CHIP_ID       0xFF

#define INA226_MANUFACTURER    0x5449
#define INA226_CHIP_IDENTIFIER 0x2260

#define INA226_ADDRESS1 0x40 // (A0+A1=GND)
#define INA226_ADDRESS2 0x41 // (A0=Vcc, A1=GND)
#define INA226_ADDRESS3 0x44 // (A0=GND, A1=Vcc)
#define INA226_ADDRESS4 0x45 // (A0+A1=Vcc)

typedef int (*ina266_reg_read_t)(uint8_t reg, uint16_t* data);
typedef int (*ina266_reg_write_t)(uint8_t reg, uint16_t data);

typedef struct
{
    ina266_reg_read_t reg_read;
    ina266_reg_write_t reg_write;
    float curr_sens;
} ina226_handle_t;

int ina226_init(ina226_handle_t* handle, ina266_reg_read_t read_cb, ina266_reg_write_t write_cb);
int ina226_reset(ina226_handle_t* handle);

int ina226_reg_read(ina226_handle_t* handle, uint8_t reg, uint16_t* data);
int ina226_reg_write(ina226_handle_t* handle, uint8_t reg, uint16_t data);

typedef enum
{
    INA226_MODE_SHUTDOWN = 0,
    INA226_MODE_SHUNT,
    INA226_MODE_VBUS,
    INA226_MODE_SHUNT_AND_VBUS,
    INA226_MODE_CONTINUOUS_SHUNT = 5,
    INA226_MODE_CONTINUOUS_VBUS,
    INA226_MODE_CONTINUOUS_SHUNT_AND_VBUS,
} ina226_configuration_mode_t;

typedef enum
{
    INA226_CONVERT_TIME_140,
    INA226_CONVERT_TIME_204,
    INA226_CONVERT_TIME_332,
    INA226_CONVERT_TIME_588,
    INA226_CONVERT_TIME_1100,
    INA226_CONVERT_TIME_2116,
    INA226_CONVERT_TIME_4156,
    INA226_CONVERT_TIME_8244,
} ina226_configuration_convert_time_t;

typedef enum
{
    INA226_AVERAGE_1,
    INA226_AVERAGE_4,
    INA226_AVERAGE_16,
    INA226_AVERAGE_64,
    INA226_AVERAGE_128,
    INA226_AVERAGE_256,
    INA226_AVERAGE_512,
    INA226_AVERAGE_1024,
} ina226_configuration_average_t;

typedef struct
{
    uint16_t mode : 3;
    uint16_t shunt_conv_time : 3;
    uint16_t vbus_conv_time : 3;
    uint16_t average : 3;
    uint16_t : 3;
    uint16_t reset : 1;
} ina226_configuration_t;

int ina226_get_configuration(ina226_handle_t* handle, ina226_configuration_t* conf);
int ina226_set_configuration(ina226_handle_t* handle, ina226_configuration_t conf);
int ina226_set_mode(ina226_handle_t* handle, ina226_configuration_mode_t mode);

int ina226_set_shunt_resistance(ina226_handle_t* handle, float shunt_resistance);

int ina226_get_limitations(ina226_handle_t* handle,
                           float* vbus_min,
                           float* vbus_max,
                           float* current_min,
                           float* current_max);

int ina226_get_voltage(ina226_handle_t* handle, float* value);
int ina226_get_current(ina226_handle_t* handle, float* value);
int ina226_get_power(ina226_handle_t* handle, float* power, float* voltage, float* current);

typedef struct
{
    uint16_t alert_latch_en : 1;
    uint16_t alert_polarity : 1;
    uint16_t math_overflow_flag : 1;
    uint16_t conv_ready_flag : 1;
    uint16_t alert_function : 1;
    uint16_t : 6;
    uint16_t conv_ready_alert : 1;
    uint16_t power_overlimit_alert : 1;
    uint16_t vbus_under_limit_alert : 1;
    uint16_t vbus_over_limit_alert : 1;
    uint16_t shunt_under_limit_alert : 1;
    uint16_t shunt_over_limit_alert : 1;
} ina226_mask_t;

int ina226_get_mask(ina226_handle_t* handle, ina226_mask_t* value);
int ina226_set_mask(ina226_handle_t* handle, ina226_mask_t value);

typedef struct
{
    uint16_t manufacturer;
    uint16_t chip_id;
} ina226_chip_info_t;

int ina226_get_chip_info(ina226_handle_t* handle, ina226_chip_info_t* value);

float ina226_calc_optimal_shunt(float current_max);