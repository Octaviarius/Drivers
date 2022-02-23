#ifndef __SI7006_H__
#define __SI7006_H__

/* ===== INCLUDES =========================================================== */

#include <stdbool.h>
#include <stdint.h>

/* ===== DEFINITIONS ======================================================== */

#define SI7006_I2C_ADDRESS (0x40)

#define SI7006_RH_RAW2PHYS(raw)   ((raw)*1.90734e-3f - 6.0f)
#define SI7006_TEMP_RAW2PHYS(raw) ((raw)*2.68127e-3f - 46.85f)

#define SI7006_RH_RAW2PHYS_Q7p9(raw)   (((uint16_t)(raw)*125 >> 7) - 3072)
#define SI7006_TEMP_RAW2PHYS_Q8p8(raw) ((int16_t)(((int32_t)(raw)*22492 - 393006285) / 32768))

#define SI7006_TCONV_RH8  (3.1e-3f)
#define SI7006_TCONV_RH10 (4.5e-3f)
#define SI7006_TCONV_RH11 (7.0e-3f)
#define SI7006_TCONV_RH12 (12.0e-3f)

#define SI7006_TCONV_T11 (2.4e-3f)
#define SI7006_TCONV_T12 (3.8e-3f)
#define SI7006_TCONV_T13 (6.2e-3f)
#define SI7006_TCONV_T14 (10.8e-3f)

#define SI7006_POWERUP_TIME (80e-3f)

/* ===== TYPES ============================================================== */

typedef enum
{
    SI7006_DEVICE_ID_ENGINEER0 = 0x00,

    SI7006_DEVICE_ID_SI7006 = 0x06,
    SI7006_DEVICE_ID_SI7013 = 0x0D,
    SI7006_DEVICE_ID_SI7020 = 0x14,
    SI7006_DEVICE_ID_SI7021 = 0x15,

    SI7006_DEVICE_ID_ENGINEER1 = 0xFF,
} si7006_device_id_t;

typedef int (*si7006_read_t)(void* data, uint8_t size, bool is_last);
typedef int (*si7006_write_t)(const void* data, uint8_t size, bool is_last);
typedef void (*si7006_wait_ms_t)(uint16_t value);

typedef struct
{
    si7006_read_t read;
    si7006_write_t write;
    si7006_wait_ms_t wait;

    si7006_setup_resolution_t res;
    bool heater_en : 1;
    bool vdd_ok    : 1;
} si7006_driver_t;

/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== GLOBAL FUNCTIONS PROTOTYPES ======================================== */

typedef enum
{
    SI7006_SETUP_RESOLUTION_RH12_T14,
    SI7006_SETUP_RESOLUTION_RH8_T12,
    SI7006_SETUP_RESOLUTION_RH10_T13,
    SI7006_SETUP_RESOLUTION_RH11_T11,
} si7006_setup_resolution_t;

int si7006_write_setup(si7006_driver_t* self, si7006_setup_resolution_t resolution, bool heater_en);
int si7006_read_setup(si7006_driver_t* self, si7006_setup_resolution_t* resolution, bool* heater_en);

typedef enum
{
    SI7006_REG_HEATER_CTRL_CURRENT_3p09mA  = 0,
    SI7006_REG_HEATER_CTRL_CURRENT_9p18mA  = 1,
    SI7006_REG_HEATER_CTRL_CURRENT_15p24mA = 2,
    SI7006_REG_HEATER_CTRL_CURRENT_27p39mA = 4,
    SI7006_REG_HEATER_CTRL_CURRENT_51p69mA = 8,
    SI7006_REG_HEATER_CTRL_CURRENT_94p20mA = 15,
} si7006_reg_heater_ctrl_current_t;

typedef struct
{
    uint8_t current : 4;
    uint8_t         : 4;
} si7006_reg_heater_ctrl_t;

int si7006_read_heater_ctrl(si7006_driver_t* self, si7006_reg_heater_ctrl_t* value);
int si7006_write_heater_ctrl(si7006_driver_t* self, si7006_reg_heater_ctrl_t value);

typedef union
{
    uint8_t id[8];
    struct
    {
        uint8_t : 8;
        uint8_t : 8;
        uint8_t : 8;
        uint8_t device_id;
    };
} si7006_reg_eid_t;

int si7006_read_eid(si7006_driver_t* self, si7006_reg_eid_t* value);

typedef union
{
    uint8_t full;
    struct
    {
        uint8_t minor : 4;
        uint8_t major : 4;
    };
} si7006_reg_revision_t;

int si7006_read_fw_revision(si7006_driver_t* self, si7006_reg_revision_t* value);

int si7006_measure_temp(si7006_driver_t* self, uint16_t* value);
int si7006_nohold_measure_temp(si7006_driver_t* self, uint16_t* value);

int si7006_measure_rh(si7006_driver_t* self, uint16_t* value);
int si7006_nohold_measure_rh(si7006_driver_t* self, uint16_t* value);

#endif /* __SI7006_H__ */
