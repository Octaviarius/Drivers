
/* ===== INCLUDES =========================================================== */

#include "mc24xx.h"

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

#define THIS_IS_LE16(val) (IS_LITTLE_ENDIAN ? (val) : (((uint16_t)(val) >> 8) | ((uint16_t)(val) << 8)))
#define THIS_IS_BE16(val) (IS_BIG_ENDIAN ? (val) : (((uint16_t)(val) >> 8) | ((uint16_t)(val) << 8)))

/* ===== TYPES ============================================================== */
/* ===== LOCAL FUNCTIONS PROTOTYPES ========================================= */
/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== LOCAL VARIABLES ==================================================== */
/* ===== GLOBAL FUNCTIONS IMPLEMENTATION ==================================== */

int mc24xx_read_data(mc24xx_driver_t* self, uint16_t address, void* data, uint16_t size)
{
    int res;

    address = THIS_IS_BE16(address);

    self->write(&address, sizeof(address), false);
    res = self->read(data, size, true);

    return res;
}

int mc24xx_read_data_seq(mc24xx_driver_t* self, void* data, uint16_t size)
{
    int res;
    res = self->read(data, size, true);
    return res;
}

int mc24xx_write_data(mc24xx_driver_t* self, uint16_t address, const void* data, uint16_t size)
{
    int res;

    address = THIS_IS_BE16(address);

    self->write(&address, sizeof(address), false);
    res = self->write(data, size, true);

    return res;
}

/* ===== LOCAL FUNCTIONS IMPLEMENTATION ===================================== */
