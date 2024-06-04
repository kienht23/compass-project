
#include "MKL46Z4.h"
#include "lcd.h"



//lcd frontplane pin and backplane pin
const static uint8_t LCD_Frp_Pin[8] = {37u, 17u, 7u, 8u, 53u, 38u, 10u, 11u};
const static uint8_t LCD_Bap_Pin[4] = {40u, 52u, 19u, 18u};

/******************************************************************/
//						Function for Initialization of LCD									//
/******************************************************************/

void SegLCD_Init(void){ //Initializes all components of SLCD on the FRDM-KL46Z
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK; 
	//Enable Clock to ports B, C, D and E, and SegLCD Peripheral
	
	// Enable Multiple Purpose CLock for LCD Alternative clock source
	MCG->C1 |= MCG_C1_IRCLKEN_MASK;		//MCG->C1 |= 0x02;
	

	
	//Configure LCD Registers
	//Configure LCD_GCR - General Control Register, controls most options in LCD Peripheral
	LCD->GCR =	LCD_GCR_SOURCE_MASK |						//Set LCD_GCR_LCLK to 111, LCD Clock prescaler where LCD controller frame frequency = LCD clock/((DUTY  |  1) x 8 x (4 | LCLK[2:0]) x Y), where Y = 2, 2, 3, 3, 4, 5, 8, 16 chosen by module duty cycle config
							LCD_GCR_DUTY(0x03);  						//Set LCD_GCR_DUTY to 011, Have 4 backplane pins, so need a 1/4 duty cycle.
	
	//Clear ben and bpen
	LCD->PEN[0] = 0x00;
	LCD->PEN[1] = 0x00;
	LCD->BPEN[0] = 0x00;
	LCD->BPEN[1] = 0x00;
	
	
	//Configure LCD_PENn - Pin Enable Register, controls which of the possible LCD pins are used
	//LCD->PEN[0] contains bits 0-31, while LCD->PEN[1] contains bits 32-63.  For pins >= 32, set in LCD->PEN[1] as LCD_PEN_PEN(Pin#-32)
	LCD->PEN[0] =  0x80   |				//LCD_P7
								 0x100  |				//LCD_P8
								 0x400  |				//LCD_P10
								 0x800  |				//LCD_P11
								 //0x20000|				//LCD_P17
								 0x40000|				//LCD_P18
								 0x80000;				//LCD_P19
	LCD->PEN[1] =  //0x20000 |				//LCD_P37
								 0x40    |				//LCD_P38
								 0x100   |				//LCD_P40
								 0x100000|				//LCD_P52
								 0x200000;				//LCD_P53
	
	//Configure LCD_SEG_BPENn - Back Plane Enable Register, controls which pins in LCD->PEN are Back Plane (commons)
	//LCD->BPEN[0] contains bits 0-31, while LCD->BPEN[1] contains bits 32-63.  For pins >= 32, set in LCD->BPEN[1] as LCD_BPEN_BPEN(Pin#-32)
	LCD->BPEN[0] = 0x40000|			//LCD_P18  COM3
								 0x80000;			//LCD_P19, COM2
	LCD->BPEN[1] = 0x100 |			//LCD_P40, COM0
								 0x100000;		//LCD_P52, COM1
	
	//Configure LCD_WFyTOx - Configures 4 Waveform signals, LCD_WF[z] is defined such that x = (z*4) and y = 3 | (z*4)
	//Where x is the n index value of WFn on the least significant byte (bits 7-0) and y is the n index value of WFn on the most significant byte (bits 31-24)

	//Note that "Disabled" is used for pins that are not set as LCD pins, where "Off" is used for pins that are set as LCD, but are just inactive.
	
	
	LCD->WF[4]  = LCD_WF_WF18(0x88)|						//WF Pin 18 (COM3) is enabled on Phases D and H
								LCD_WF_WF19(0x44);						//WF Pin 19 (COM2) is enabled on Phases C and G
	LCD->WF[10] = LCD_WF_WF40(0x11);				    //WF Pin 40 (COM0) is enabled on Phases A and E
	LCD->WF[13] = LCD_WF_WF52(0x22);					  //WF Pin 52 (COM1) is enabled on Phases B and F

								
	LCD->GCR |= LCD_GCR_LCDEN_MASK;							//Set LCDEN to enable operation of LCD
}//End SegLCD_Init


void SegLCD_Set(uint8_t Value,uint8_t Digit){//Sets a value from 0-F to a specified Digit, with 1 being the leftmost, 4 being the rightmost.  Will not display error is Value is outside of 0-F, but display will not update
	int k;
	if(Digit > 4){
		SegLCD_DisplayError(0x01);
	}		//Display "Err" if trying to access a digit that does not exist
	else if (Digit == 1 )
	{
		LCD->WF8B[0] = LCD_CLEAR;
		LCD->WF8B[1] = LCD_CLEAR;
	}
	else{
		if(Value==0x00)			{LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E |LCD_SEG_F); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x01){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_CLEAR); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x02){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B);}
		else if(Value==0x03){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x04){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x05){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
		else if(Value==0x06){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
		else if(Value==0x07){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_CLEAR); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x08){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x09){LCD->WF8B[LCD_Frp_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frp_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);
		}
	}
}//End SegLCD_Set

void SegLCD_DisplayDecimal(uint16_t Value){//Displays a 4 Digit number in decimal
	if(Value > 9999){
		SegLCD_DisplayError(0x10); //Display "Err" if value is greater than 4 digits
	}
	else{
		SegLCD_Set(Value/100,2);
		SegLCD_Set((Value - (Value/100)*100)/10,3);
		SegLCD_Set(Value - (Value/10)*10,4);
	}
}//End SegLCD_Display4Digit


void SegLCD_DisplayError(uint8_t ErrorNum){//Displays Err# on screen, where # is a value 0-F.  If ErrorNum is outside of that range, just displays Err
	LCD->WF8B[2] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G);
	LCD->WF8B[3] = (LCD_SEG_A);
	LCD->WF8B[4] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[5] = (LCD_CLEAR);
	LCD->WF8B[6] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[7] = (LCD_CLEAR);
	if(ErrorNum < 0x10){
		SegLCD_Set(ErrorNum,4); //Display ErrorNum in digit 1 if within valid range.  If not, leave blank.
	}
	else{
		LCD->WF8B[0] = (LCD_CLEAR);
		LCD->WF8B[1] = (LCD_CLEAR);
	}
}//End SegLCD_DisplayError

