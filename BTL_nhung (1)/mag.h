#include "MKL46Z4.h"

typedef struct  {
	int16_t x;
	int16_t y;
	int16_t z;
} mag3110;
void mag_Init();
mag3110 mag_read();