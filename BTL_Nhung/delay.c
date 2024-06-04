#include "MKL46Z4.h"
#include "delay.h"

#define true 1
#define false 0

volatile uint32_t msTicks = 0;
uint8_t volatile state = false;
uint32_t TICK_Green = 1000; // 1000ms = 1Hz
uint32_t TICK_Red = 500; // 500ms = 2Hz

void Init_Systick(){
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = SystemCoreClock/1000;
	NVIC_SetPriority(-1,15);
}

void Shutdown(){
	SysTick->CTRL &= ~(1<<0);
}

void SysTick_Handler (void){
	msTicks++;
	
}

void Delay (uint32_t TICK) {
	while (msTicks < TICK); // Wait 500ms
	msTicks = 0; // Reset counter
	
}

void init_LED() {
	//LED Green PortD5
	SIM->SCGC5 |= 1 << 12; //This enables the clock to PortD
	PORTD->PCR[5] |= 1 << 8; //Mux 001 | GPIO
	PTD->PDDR |= 1 << 5; //This sets PTD5 as an output
	
	//LED Red PortE29
	SIM->SCGC5 |= 1 << 13; //This enables the clock to PortE
	PORTE->PCR[29] |= 1 << 8; //Mux 001 | GPIO
	PTE->PDDR |= 1 << 29; //This sets PTE29 as an output
}

void reset_LED() {
	PTD->PSOR |= 1 << 5;
	PTE->PCOR |= 1 << 29;
}

void init_SW() {
	SIM->SCGC5 |= 1u << 11; //This enables the clock to PORTC
	
	//SW1 PORTC3
  PORTC->PCR[3] |= 1u << 8 | 1 << 1 | 1 << 0; //This sets GPIO and Enable pull-up resistor
  PTC->PDDR &= ~(1u << 3); //This sets PTC3 as an input. 
	
	//SW2 PORTC12
  PORTC->PCR[12] |= 1u << 8 | 1 << 1 | 1 << 0; //This sets GPIO and Enable pull-up resistor
  PTC->PDDR &= ~(1u << 12); //This sets PTC12 as an input. 
	
}

void init_ITR_SW(void) {
	//Select falling edge interrupts for PORTC[3].
	PORTC->PCR[3]	 |=  (0xA) << 16; //OR		PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
	PORTC->PCR[12] |=  (0xA) << 16; //OR		PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);
	
	NVIC_ClearPendingIRQ(31); // number for both PORTC and PORTD are 31
	NVIC_EnableIRQ(31);
}


//SW1 interrupt Handler
void PORTC_PORTD_IRQHandler(void) {
	//If SW1 not press => do nothing
	if ((PTC->PDIR & (1<<3)) != 0)
		return;
	
	//Toggle state System
	if (state == false) {
		PTD->PCOR |= 1 << 5;
		PTE->PSOR |= 1 << 29;
		state = true;
	}
	else {
		PTD->PSOR |= 1 << 5;
		PTE->PCOR |= 1 << 29;
		state = false;
	}

	/* Clear interrupt service flag */
	PORTC->PCR[3] |= 1 << 24; 
}

//SU2 interrupt 
void SW2_RESET(void) {
		if ((PTC->PDIR & (1<<12)) != 0)
			state = state;
		else {
				reset_LED();
		}
		/* Clear interrupt service flag */
	PORTC->PCR[12] |= 1 << 24; 
		
}



void toggle_LED() {
	if (state == false) {	
			PTE->PTOR |= 1 << 29;
			msTicks = 0;
			Delay(TICK_Red);
		} 
		else {
			PTD->PTOR |= 1 << 5;
			msTicks = 0;
			Delay(TICK_Green);
		}
}