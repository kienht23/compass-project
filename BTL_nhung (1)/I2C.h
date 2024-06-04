#include "MKL46Z4.h"

void I2C_init(void);
void I2C_write(uint8_t data, uint8_t register_address);
uint8_t I2C_read( uint8_t register_address);
