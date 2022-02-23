
/* ===== INCLUDES =========================================================== */

#include "si7006.h"

/* ===== DEFINITIONS ======================================================== */

#define RETURN_CONDITIONAL(res, desired) \
    if ((res) != (desired))              \
    {                                    \
        return (res);                    \
    }

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
#define FROM_LE16(val) (IS_LITTLE_ENDIAN ? (val) : (((uint16_t)(val) >> 8) | ((uint16_t)(val) << 8)))

#define SAFE_ASSIGN(ptr, val) \
    if ((ptr) != NULL)        \
    {                         \
        *(ptr) = val;         \
    }

/* ===== TYPES ============================================================== */

typedef enum
{
    SI7006_CMD_MEASURE_RH          = 0xE5,
    SI7006_CMD_NOHOLD_MEASURE_RH   = 0xF5,
    SI7006_CMD_MEASURE_TEMP        = 0xE3,
    SI7006_CMD_NOHOLD_MEASURE_TEMP = 0xF3,
    SI7006_CMD_READ_TEMP           = 0xE0,
    SI7006_CMD_RESET               = 0xFE,
    SI7006_CMD_WRITE_RHT           = 0xE6,
    SI7006_CMD_READ_RHT            = 0xE7,
    SI7006_CMD_WRITE_HEATER_CTRL   = 0x51,
    SI7006_CMD_READ_HEATER_CTRL    = 0x11,
    SI7006_CMD_READ_EID0_0         = 0xFA,
    SI7006_CMD_READ_EID0_1         = 0x0F,
    SI7006_CMD_READ_EID1_0         = 0xFC,
    SI7006_CMD_READ_EID1_1         = 0xC9,
    SI7006_CMD_READ_FW_REVISION_0  = 0x84,
    SI7006_CMD_READ_FW_REVISION_1  = 0xB8,
} si7006_cmd_t;

typedef union
{
    uint8_t value;

    struct
    {
        uint8_t res0       : 1;
        uint8_t            : 1;
        uint8_t heater_en  : 1;
        uint8_t            : 3;
        uint8_t vdd_status : 1;
        uint8_t res1       : 1;
    };
} si7006_reg_user_t;

/* ===== LOCAL FUNCTIONS PROTOTYPES ========================================= */
/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== LOCAL VARIABLES ==================================================== */

static uint16_t const _tconv_rht[] = {
    [SI7006_SETUP_RESOLUTION_RH12_T14] = (uint16_t)((SI7006_TCONV_RH12 + SI7006_TCONV_T14) * 1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH8_T12]  = (uint16_t)((SI7006_TCONV_RH8 + SI7006_TCONV_T12) * 1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH10_T13] = (uint16_t)((SI7006_TCONV_RH10 + SI7006_TCONV_T13) * 1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH11_T11] = (uint16_t)((SI7006_TCONV_RH11 + SI7006_TCONV_T11) * 1e3f + 0.5f),
};

static uint16_t const _tconv_t[] = {
    [SI7006_SETUP_RESOLUTION_RH12_T14] = (uint16_t)((SI7006_TCONV_T14)*1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH8_T12]  = (uint16_t)((SI7006_TCONV_T12)*1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH10_T13] = (uint16_t)((SI7006_TCONV_T13)*1e3f + 0.5f),
    [SI7006_SETUP_RESOLUTION_RH11_T11] = (uint16_t)((SI7006_TCONV_T11)*1e3f + 0.5f),
};

/* ===== GLOBAL FUNCTIONS IMPLEMENTATION ==================================== */

int si7006_write_setup(si7006_driver_t* self, si7006_setup_resolution_t resolution, bool heater_en)
{
    uint8_t out_data[] = {
        SI7006_CMD_WRITE_RHT,
        (si7006_reg_user_t){
            .heater_en = heater_en,
            .res0      = resolution & 1,
            .res1      = (resolution >> 1) & 1,
        }
            .value,
    };

    int res;

    res = self->write(&out_data, sizeof(out_data), true);
    RETURN_CONDITIONAL(res, 0);

    self->heater_en  = heater_en;
    self->resolution = resolution;

    return res;
}

int si7006_read_setup(si7006_driver_t* self, si7006_setup_resolution_t* resolution, bool* heater_en, bool* vdd_ok)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_READ_RHT,
    };

    si7006_reg_user_t in_data;

    int res;
    self->heater_en  = heater_en;
    self->resolution = resolution;

    self->write(&out_data, sizeof(out_data), false);
    res = self->write(&in_data, sizeof(in_data), true);

    self->heater_en  = in_data.heater_en;
    self->resolution = in_data.res0 | (in_data.res0 << 1);
    self->vdd_ok     = (in_data.vdd_status == 0);

    SAFE_ASSIGN(resolution, self->resolution);
    SAFE_ASSIGN(heater_en, self->heater_en);
    SAFE_ASSIGN(vdd_ok, self->vdd_ok);

    return res;
}

int si7006_read_heater_ctrl(si7006_driver_t* self, si7006_reg_heater_ctrl_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_READ_HEATER_CTRL,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    res = self->read(value, 1, true);

    return res;
}
int si7006_write_heater_ctrl(si7006_driver_t* self, si7006_reg_heater_ctrl_t value)
{
    uint8_t out_data[] = {
        SI7006_CMD_WRITE_HEATER_CTRL,
        (uint8_t)value,
    };

    int res;

    res = self->write(&out_data, sizeof(out_data), false);

    return res;
}

int si7006_read_eid(si7006_driver_t* self, si7006_reg_eid_t* value)
{
    const uint8_t const out_data[2][2] = {
        {
            SI7006_CMD_READ_EID0_0,
            SI7006_CMD_READ_EID0_1,
        },
        {
            SI7006_CMD_READ_EID1_0,
            SI7006_CMD_READ_EID1_1,
        },
    };

    int res;
    uint8_t in_data[8];

    self->write(&out_data[0], sizeof(out_data[0]), false);
    res = self->read(&in_data, 8, true);
    RETURN_CONDITIONAL(res, 0);

    value->id[4] = in_data[6];
    value->id[5] = in_data[4];
    value->id[6] = in_data[2];
    value->id[7] = in_data[0];

    res = self->write(&out_data[1], sizeof(out_data[1]), false);
    res |= self->read(&in_data, 6, true);
    RETURN_CONDITIONAL(res, 0);

    value->id[0] = in_data[4];
    value->id[1] = in_data[3];
    value->id[2] = in_data[1];
    value->id[3] = in_data[0];

    return res;
}

int si7006_read_fw_revision(si7006_driver_t* self, si7006_reg_revision_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_READ_FW_REVISION_0,
        SI7006_CMD_READ_FW_REVISION_1,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    res = self->read(value, 1, true);

    return res;
}

int si7006_measure_temp(si7006_driver_t* self, uint16_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_MEASURE_TEMP,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    self->wait(_tconv_t[self->resolution]);
    res = self->read(value, 2, true);

    *value = FROM_BE16(*value);

    return res;
}

int si7006_nohold_measure_temp(si7006_driver_t* self, uint16_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_NOHOLD_MEASURE_TEMP,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    self->wait(_tconv_t[self->resolution]);
    res = self->read(value, 2, true);

    *value = FROM_BE16(*value);

    return res;
}

int si7006_measure_rh(si7006_driver_t* self, uint16_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_MEASURE_RH,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    self->wait(_tconv_rht[self->resolution]);
    res = self->read(value, 2, true);

    *value = FROM_BE16(*value);

    return res;
}

int si7006_nohold_measure_rh(si7006_driver_t* self, uint16_t* value)
{
    const uint8_t const out_data[] = {
        SI7006_CMD_NOHOLD_MEASURE_RH,
    };

    int res;

    self->write(&out_data, sizeof(out_data), false);
    self->wait(_tconv_rht[self->resolution]);
    res = self->read(value, 2, true);

    *value = FROM_BE16(*value);

    return res;
}

/* ===== LOCAL FUNCTIONS IMPLEMENTATION ===================================== */
