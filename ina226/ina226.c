#include "ina226.h"

#include <stdbool.h>
#include <stdlib.h>

#define CHECK_RESULT(res) \
    do                    \
    {                     \
        if (res != 0)     \
            return res;   \
    } while (0)

#define SAFE_ASSIGN(dptr, val) \
    if ((dptr) != NULL)        \
        do                     \
        {                      \
            *(dptr) = val;     \
    } while (0)

#define VBUS_SENSE  (1.25e-3)
#define SHUNT_SENSE (2.5e-6)
#define VBUS_VMAX   (40.96)
#define SHUNT_VMAX  (81.92e-3)

int ina226_init(ina226_handle_t* handle, ina266_reg_read_t read_cb, ina266_reg_write_t write_cb)
{
    handle->curr_sens = 0.0;
    handle->reg_read = read_cb;
    handle->reg_write = write_cb;

    int res = 0;

    res = ina226_reset(handle);
    CHECK_RESULT(res);

    ina226_chip_info_t info;
    res = ina226_get_chip_info(handle, &info);
    CHECK_RESULT(res);

    if ((info.chip_id != INA226_CHIP_IDENTIFIER) || (info.manufacturer != INA226_MANUFACTURER))
    {
        res = 1;
    }

    return res;
}

int ina226_reset(ina226_handle_t* handle)
{
    return ina226_set_configuration(handle, (ina226_configuration_t){.reset = true});
}

int ina226_reg_read(ina226_handle_t* handle, uint8_t reg, uint16_t* data)
{
    uint16_t tmp;
    int res;

    res = handle->reg_read(reg, &tmp);
    *data = (tmp >> 8) | (tmp << 8);
    return res;
}

int ina226_reg_write(ina226_handle_t* handle, uint8_t reg, uint16_t data)
{
    data = (data >> 8) | (data << 8);
    return handle->reg_write(reg, data);
}

int ina226_get_configuration(ina226_handle_t* handle, ina226_configuration_t* conf)
{
    return ina226_reg_read(handle, INA226_REG_CONFIGURATION, (uint16_t*)conf);
}

int ina226_set_configuration(ina226_handle_t* handle, ina226_configuration_t conf)
{
    return ina226_reg_write(handle, INA226_REG_CONFIGURATION, *(uint16_t*)&conf);
}

int ina226_set_mode(ina226_handle_t* handle, ina226_configuration_mode_t mode)
{
    int res = 0;

    ina226_configuration_t conf;
    res = ina226_get_configuration(handle, &conf);
    CHECK_RESULT(res);

    conf.mode = mode;
    res = ina226_set_configuration(handle, conf);
    CHECK_RESULT(res);

    return res;
}

int ina226_set_shunt_resistance(ina226_handle_t* handle, float shunt_resistance)
{
    handle->curr_sens = shunt_resistance * SHUNT_VMAX / (float)(1UL << 15);
    return 0;
}

int ina226_set_voltage_config(ina226_handle_t* handle, ina226_configuration_convert_time_t conv_time)
{
    int res = 0;

    ina226_configuration_t conf;
    res = ina226_get_configuration(handle, &conf);
    CHECK_RESULT(res);

    conf.shunt_conv_time = conv_time;
    res = ina226_set_configuration(handle, conf);
    CHECK_RESULT(res);

    return res;
}

int ina226_get_limitations(ina226_handle_t* handle,
                           float* vbus_min,
                           float* vbus_max,
                           float* current_min,
                           float* current_max)
{
    SAFE_ASSIGN(vbus_min, 0.0);
    SAFE_ASSIGN(vbus_min, VBUS_SENSE * (float)(1UL << 15));
    SAFE_ASSIGN(current_min, (float)(1UL << 15) / handle->curr_sens);
    SAFE_ASSIGN(current_max, -*current_max);

    return 0;
}

int ina226_get_voltage(ina226_handle_t* handle, float* value)
{
    uint16_t v;
    int res;

    res = ina226_reg_read(handle, INA226_REG_BUS_VOLTAGE, &v);
    CHECK_RESULT(res);

    *value = VBUS_SENSE * (float)v;

    return res;
}

int ina226_get_current(ina226_handle_t* handle, float* value)
{
    int16_t v;
    int res;

    res = ina226_reg_read(handle, INA226_REG_SHUNT_VOLTAGE, (uint16_t*)&v);
    CHECK_RESULT(res);

    *value = handle->curr_sens * (float)v;

    return res;
}

int ina226_get_power(ina226_handle_t* handle, float* power, float* voltage, float* current)
{
    uint16_t raw_v;
    int16_t raw_a;
    int res;

    res = ina226_reg_read(handle, INA226_REG_SHUNT_VOLTAGE, (uint16_t*)&raw_a);
    CHECK_RESULT(res);
    res = ina226_reg_read(handle, INA226_REG_BUS_VOLTAGE, &raw_v);
    CHECK_RESULT(res);

    float v = VBUS_SENSE * (float)raw_v;
    float a = handle->curr_sens * (float)raw_a;

    SAFE_ASSIGN(current, a);
    SAFE_ASSIGN(voltage, v);
    SAFE_ASSIGN(power, v * a);

    return res;
}

int ina226_get_mask(ina226_handle_t* handle, ina226_mask_t* value)
{
    return ina226_reg_read(handle, INA226_REG_MASK_ENABLE, (uint16_t*)value);
}

int ina226_set_mask(ina226_handle_t* handle, ina226_mask_t value)
{
    return ina226_reg_read(handle, INA226_REG_MASK_ENABLE, (uint16_t*)&value);
}

int ina226_get_chip_info(ina226_handle_t* handle, ina226_chip_info_t* value)
{
    ina226_reg_read(handle, INA226_REG_MANUFACTURER, &value->manufacturer);
    return ina226_reg_read(handle, INA226_REG_CHIP_ID, &value->chip_id);
}

float ina226_calc_optimal_shunt(float current_max)
{
    return SHUNT_VMAX / current_max;
}
