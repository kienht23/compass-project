#include "MKL46Z4.h"
//#include "switch.h"

void Init_Systick();
void Shutdown();
void SysTick_Handler (void);
void Delay (uint32_t TICK);
void init_LED(void);
void reset_LED(void);
void init_SW(void);
void init_ITR_SW(void);
void PORTC_PORTD_IRQHandler(void);
void SW2_RESET(void);
void toggle_LED(void);