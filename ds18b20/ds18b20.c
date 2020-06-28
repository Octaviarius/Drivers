#include "ds18b20.h"

#include <string.h>

#define CMD_READ_ROM   (0x33)
#define CMD_MATCH_ROM  (0x55)
#define CMD_SKIP_ROM   (0xCC)
#define CMD_SEARCH_ROM (0xF0)

#define SCR_TL   0
#define SCR_TH   1
#define SCR_CONF 2

#define CONF2RESOLUTION(reg) (((reg) >> 5) & 3)
#define RESOLUTION2CONF(res) (((res)&3) << 5)

static int _preambule(ds18b20_handle_t* handle);
static int _read_scratchpad(ds18b20_handle_t* handle);
static int _write_scratchpad(ds18b20_handle_t* handle);

static const float _sens_list[4] = {
    [DS18B20_RESOLUTION_9BIT] = 0.5,
    [DS18B20_RESOLUTION_10BIT] = 0.25,
    [DS18B20_RESOLUTION_11BIT] = 0.125,
    [DS18B20_RESOLUTION_12BIT] = 0.0625,
};

static const uint16_t _conv_time_list[4] = {
    [DS18B20_RESOLUTION_9BIT] = 94,
    [DS18B20_RESOLUTION_10BIT] = 188,
    [DS18B20_RESOLUTION_11BIT] = 375,
    [DS18B20_RESOLUTION_12BIT] = 750,
};

int ds18b20_init(ds18b20_handle_t* handle, uint64_t device_id)
{
    handle->inited = false;
    handle->dev_id = device_id;

    int res;

    res = _preambule(handle);
    if (res != 0)
    {
        return res;
    }

    res = _read_scratchpad(handle);
    if (res != 0)
    {
        return res;
    }

    handle->inited = true;
    handle->convertion_period = _conv_time_list[CONF2RESOLUTION(handle->scratchpad[SCR_CONF])];
    return res;
}

int ds18b20_set_id_usable(ds18b20_handle_t* handle, bool is_use)
{
    if (!handle->inited)
    {
        return 1;
    }

    handle->use_id = is_use;
    return 0;
}

int ds18b20_start_convertion(ds18b20_handle_t* handle)
{
    if (!handle->inited)
    {
        return 1;
    }

    _preambule(handle);
    return handle->write_data((uint8_t[1]){DS18B20_CMD_CONVERT}, 1, true);
}

int ds18b20_start_convertion_all(ds18b20_handle_t* handle)
{
    if (!handle->inited)
    {
        return 1;
    }

    handle->reset();
    uint8_t d[2] = {CMD_SKIP_ROM, DS18B20_CMD_CONVERT};
    return handle->write_data(d, 2, true);
}

int ds18b20_get_temperature(ds18b20_handle_t* handle, float* value)
{
    if (!handle->inited)
    {
        return 1;
    }

    _preambule(handle);
    handle->write_data((uint8_t[1]){DS18B20_CMD_READ_SCRATCHPAD}, 1, false);

    int16_t raw;
    int res = handle->read_data(&raw, 2);

    *value = (float)raw * _sens_list[CONF2RESOLUTION(handle->scratchpad[SCR_CONF])];

    return res;
}

int ds18b20_set_resolution(ds18b20_handle_t* handle, ds18b20_resolution_t value)
{
    if (!handle->inited)
    {
        return 1;
    }

    handle->scratchpad[SCR_CONF] = RESOLUTION2CONF(value);
    handle->convertion_period = _conv_time_list[CONF2RESOLUTION(handle->scratchpad[SCR_CONF])];

    _preambule(handle);
    return _write_scratchpad(handle);
}

int ds18b20_get_resolution(ds18b20_handle_t* handle, ds18b20_resolution_t* value)
{
    if (!handle->inited)
    {
        return 1;
    }

    *value = (ds18b20_resolution_t)CONF2RESOLUTION(handle->scratchpad[SCR_CONF]);
    return 0;
}

int ds18b20_set_alarm_range(ds18b20_handle_t* handle, float min, float max)
{
    if (!handle->inited)
    {
        return 1;
    }

    handle->scratchpad[SCR_TL] = (int8_t)min;
    handle->scratchpad[SCR_TH] = (int8_t)max;

    _preambule(handle);
    return _write_scratchpad(handle);
}

int ds18b20_get_alarm_range(ds18b20_handle_t* handle, float* min, float* max)
{
    if (!handle->inited)
    {
        return 1;
    }

    *min = (float)handle->scratchpad[SCR_TL];
    *max = (float)handle->scratchpad[SCR_TH];
    return 0;
}

int ds18b20_save_configs(ds18b20_handle_t* handle)
{
    if (!handle->inited)
    {
        return 1;
    }

    _preambule(handle);
    return handle->write_data((uint8_t[1]){DS18B20_CMD_COPY_SCRATCHPAD}, 1, true);
}

int ds18b20_restore_configs(ds18b20_handle_t* handle)
{
    if (!handle->inited)
    {
        return 1;
    }

    _preambule(handle);
    handle->write_data((uint8_t[1]){DS18B20_CMD_RECALL}, 1, false);
    return _read_scratchpad(handle);
}

static int _preambule(ds18b20_handle_t* handle)
{
    int res;

    res = handle->reset();

    if (res != 0)
    {
        return res;
    }

    if (handle->use_id)
    {
        handle->write_data((uint8_t[1]){CMD_MATCH_ROM}, 1, false);
        res = handle->write_data(&handle->dev_id, 8, false);
    }
    else
    {
        res = handle->write_data((uint8_t[1]){CMD_SKIP_ROM}, 1, false);
    }

    return res;
}

static int _read_scratchpad(ds18b20_handle_t* handle)
{
    uint8_t d[5];

    handle->write_data((uint8_t[1]){DS18B20_CMD_READ_SCRATCHPAD}, 1, false);
    int res = handle->read_data(d, 5);

    memcpy(handle->scratchpad, &d[2], 3);

    return res;
}

static int _write_scratchpad(ds18b20_handle_t* handle)
{
    uint8_t d[4];

    d[0] = DS18B20_CMD_WRITE_SCRATCHPAD;
    memcpy(&d[1], handle->scratchpad, 3);

    return handle->write_data(d, 4, false);
}