#pragma once

#include <stdbool.h>
#include <stdint.h>

#define DS18B20_FAMILY 0x28

#define DS18B20_CMD_CONVERT          0x44
#define DS18B20_CMD_COPY_SCRATCHPAD  0x48
#define DS18B20_CMD_WRITE_SCRATCHPAD 0x4E
#define DS18B20_CMD_RECALL           0xB8
#define DS18B20_CMD_READ_SCRATCHPAD  0xBE

#define DS18B20_RAW2PHYS_12BIT(val) (float)(val) * (1.0 / (float)(1UL << 4))
#define DS18B20_RAW2PHYS_11BIT(val) (float)(val) * (1.0 / (float)(1UL << 3))
#define DS18B20_RAW2PHYS_10BIT(val) (float)(val) * (1.0 / (float)(1UL << 2))
#define DS18B20_RAW2PHYS_9BIT(val)  (float)(val) * (1.0 / (float)(1UL << 1))

#define DS18B20_MAX_CONV_TIME_MS (750)

typedef struct
{
    int (*read_data)(void* data, uint32_t size);
    int (*write_data)(void* data, uint32_t size, bool is_strong);
    int (*reset)(void);
    uint64_t dev_id;
    uint16_t convertion_period;
    uint8_t scratchpad[3];
    uint8_t inited : 1;
    uint8_t use_id : 1;
} ds18b20_handle_t;

int ds18b20_init(ds18b20_handle_t* handle, uint64_t device_id);

int ds18b20_set_id_usable(ds18b20_handle_t* handle, bool is_use);

int ds18b20_start_convertion(ds18b20_handle_t* handle);
int ds18b20_start_convertion_all(ds18b20_handle_t* handle);

int ds18b20_get_temperature(ds18b20_handle_t* handle, float* value);

typedef enum
{
    DS18B20_RESOLUTION_9BIT,
    DS18B20_RESOLUTION_10BIT,
    DS18B20_RESOLUTION_11BIT,
    DS18B20_RESOLUTION_12BIT,
} ds18b20_resolution_t;

int ds18b20_set_resolution(ds18b20_handle_t* handle, ds18b20_resolution_t value);
int ds18b20_get_resolution(ds18b20_handle_t* handle, ds18b20_resolution_t* value);

int ds18b20_set_alarm_range(ds18b20_handle_t* handle, float min, float max);
int ds18b20_get_alarm_range(ds18b20_handle_t* handle, float* min, float* max);

int ds18b20_save_configs(ds18b20_handle_t* handle);
int ds18b20_restore_configs(ds18b20_handle_t* handle);
