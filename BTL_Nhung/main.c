#include "MKL46Z4.h"
#include "I2C.h"
#include "delay.h"
#include "uart.h"
#include "mag.h"
#include "lcd.h"
#include<math.h>

#define true 1
#define false 0

int main() {
	
	uint8_t volatile state = false;
	
	UART_Init();
	Init_Systick();
	I2C_init();
	SegLCD_Init();
	mag_Init();
	init_LED();
	init_SW();
	init_ITR_SW();
	Delay(1);
	while(1){
		
		SW2_RESET();
		if (state == false) {
		mag3110 data = mag_read();
		float d = fabs((float)data.x);
		float theta = atan2(data.y-1000.0 , data.x-45); //tinh goc
		float theta_deg = theta * (180.0 / 3.14159) + 180.0 ;
		int thetaa = round(theta_deg);
		
		
		UART_SendString("   ___x:  ");
		UART_SendFloat(data.x);
		UART_SendString("   ___y:  ");
		UART_SendFloat((data.y));
		UART_SendString("   _goc:  ");
		UART_SendFloat(thetaa);
		UART_SendString("\n");
		SegLCD_DisplayDecimal(thetaa);
		}
		toggle_LED();
		
	}
	return 0;
}