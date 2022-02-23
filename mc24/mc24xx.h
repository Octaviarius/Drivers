#ifndef __MC24XX_H__
#define __MC24XX_H__

/* ===== INCLUDES =========================================================== */

#include <stdint.h>

/* ===== DEFINITIONS ======================================================== */

#define MC24XX_I2C_ADDRESS_0 0xA0
#define MC24XX_I2C_ADDRESS_1 0xA1
#define MC24XX_I2C_ADDRESS_2 0xA2
#define MC24XX_I2C_ADDRESS_3 0xA3
#define MC24XX_I2C_ADDRESS_4 0xA4
#define MC24XX_I2C_ADDRESS_5 0xA5
#define MC24XX_I2C_ADDRESS_6 0xA6
#define MC24XX_I2C_ADDRESS_7 0xA7

#define MC24XX_PAGE_SIZE 128

/* ===== TYPES ============================================================== */

typedef int (*mc24xx_read_t)(void* data, size_t size, bool is_last);
typedef int (*mc24xx_write_t)(const void* data, size_t size, bool is_last);

typedef struct
{
    mc24xx_read_t read;
    mc24xx_write_t write;
} mc24xx_driver_t;

/* ===== GLOBALS AND EXTERNS ================================================ */
/* ===== GLOBAL FUNCTIONS PROTOTYPES ======================================== */

int mc24xx_read_data(mc24xx_driver_t* self, uint16_t address, void* data, uint16_t size);
int mc24xx_read_data_seq(mc24xx_driver_t* self, void* data, uint16_t size);
int mc24xx_write_data(mc24xx_driver_t* self, uint16_t address, const void* data, uint16_t size);

#endif /* __MC24XX_H__ */
