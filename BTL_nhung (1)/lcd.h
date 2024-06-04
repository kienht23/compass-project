#include "MKL46Z4.h"

//Function Declarations
void SegLCD_Init(void);
void SegLCD_Set(uint8_t Value,uint8_t Digit);
void SegLCD_DisplayDecimal(uint16_t Value);
void SegLCD_DisplayError(uint8_t ErrorNum);


//Create macros for segments
#define LCD_SEG_D 0x01
#define LCD_SEG_E 0x02
#define LCD_SEG_G 0x04
#define LCD_SEG_F 0x08
#define LCD_SEG_C 0x02
#define LCD_SEG_B 0x04
#define LCD_SEG_A 0x08
#define LCD_CLEAR 0x00


