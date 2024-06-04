#include "MKL46Z4.h"
#include "I2C.h"

void I2C_init(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //enable clock port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; 	//enable clock I2C0
	
	//chon che do hoat dong PTE24 PTE25
	PORTE->PCR[25] |= PORT_PCR_MUX(5);	
	PORTE->PCR[24] |= PORT_PCR_MUX(5);	
	
	//pull up 
	PORTE->PCR[25] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTE->PCR[24] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	
	//enable I2C0
	I2C0->C1 |= I2C_C1_IICEN_MASK;
	
}

void I2C_write(uint8_t data, uint8_t register_address){
	I2C0->C1 |= I2C_C1_TX_MASK;		//dat I2C o che do truyen
	I2C0->C1 |= I2C_C1_MST_MASK;	//bat dau truyen bat MST len 1
	
	I2C0->D = 0x1C;					//0x0E=>0001110 W(0) => 0x1C
	while(!(I2C0->S & I2C_S_IICIF_MASK));	//cho AK
	I2C0->S |= I2C_S_IICIF_MASK; 	//Clear interupt
	
	I2C0->D = register_address;		//gui dia chi thanh ghi
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	I2C0->D = data;					//gui data
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	I2C0->C1 &= ~I2C_C1_MST_MASK;	//gui tin hieu ket thuc MST tu 1->0
}

uint8_t I2C_read( uint8_t register_address){
	uint8_t data;
	I2C0->C1 |= I2C_C1_TX_MASK;		//dat I2C o che do truyen
	I2C0->C1 |= I2C_C1_MST_MASK;	//bat dau truyen bat MST len 1
	
	I2C0->D = 0x1C;					//0x0E=>0001110 W(0) => 0x1C
	while(!(I2C0->S & I2C_S_IICIF_MASK));	//cho AK
	I2C0->S |= I2C_S_IICIF_MASK; 	//Clear interupt
	
	I2C0->D = register_address;		//gui dia chi thanh ghi
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	I2C0->C1 |= I2C_C1_RSTA_MASK;	//gui lai tin hieu bat dau 
	
	I2C0->D = 0x1D;					//0x0E=>0001110 R(1) => 0x1D
	while(!(I2C0->S & I2C_S_IICIF_MASK));	
	I2C0->S |= I2C_S_IICIF_MASK; 	
	
	I2C0->C1 &= ~I2C_C1_TX_MASK;	//chuyen I2C sang che do nhan TX=0
	I2C0->C1 |= I2C_C1_TXAK_MASK;	//gui tin hieu NAK
	data = I2C0->D;					//read to receive
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	data = I2C0->D;					//doc lai du lieu 
	while(!(I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
	
	
	I2C0->C1 &= ~I2C_C1_MST_MASK;	//gui tin hieu ket thuc MST tu 1->0
	return data;
	
}
