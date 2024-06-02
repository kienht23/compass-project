#include  <stdio.h>
#include "LCD.h"
#include "LCD.c"



int main()
{
    SegLCD_Init();
    while(1)
    {   
        SegLCD_DisplayHex(15);
    }
     
}
