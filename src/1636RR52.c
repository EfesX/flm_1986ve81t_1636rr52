#include "1636RR52.h"
//#include "mdr32f8_ssp.h"

uint16_t trash = 0;

void delay_mem(uint32_t count){
	uint32_t tick = count;
	while(tick != 0){
		tick--;
	}
}

void ssp_deinit(void){
  MDR_SSP0->CR0   = 0;
  MDR_SSP0->CR1   = 0;
  MDR_SSP0->CPSR  = 0;
  MDR_SSP0->IMSC  = 0;
  MDR_SSP0->DMACR = 0;
  MDR_SSP0->ICR = ((uint32_t)0x00000002) | ((uint32_t)0x00000001);
}


void clear_fifo(void){
	//uint16_t trash = 0;
	while(SSP_CHECK_RX_EMPTY() != 0){
		//trash = MDR_SSP0->DR;
		MDR_SSP0->DR;
	}
	
}

void write_enable(void){
	PORTA->CRXTX |= (1 << 9);
	MDR_SSP0->DR = 0x0006;
	while(SSP_CHECK_TX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
	PORTA->SRXTX |= (1 << 9);
	trash = MDR_SSP0->DR;
}

void unprotect_sector(uint32_t addr){
	write_enable();
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x0039;
	MDR_SSP0->DR = (addr >> 16);
	MDR_SSP0->DR = (addr >> 8);
	MDR_SSP0->DR = addr;
	
	while(SSP_CHECK_TX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
		
	PORTA->SRXTX |= (1 << 9);
		
	clear_fifo();
}

//======================================== PUBLIC ===============================================================================
void ic_chip_init(void){
	//SSP_InitTypeDef ssp0;
	// ======================== настройка портов ===========================================
	CLK_CNTR->PER0_CLK 	|= 0x00002000; // разрешение тактирования PORTA
	PORTA->KEY 			 		= 0x8555AAA1;
	
	PORTA->SOE	    |= 0x00000200; //PA9 as OUT
	PORTA->SANALOG  |= 0x00000200; 
	PORTA->SPULLUP  |= 0x00000200;
	PORTA->SPWR[0]  |= 0x000C0000;
		
		
	PORTA->SOE	    |= 0x00000180;
	PORTA->SFUNC[0] |= 0x60000000;
	PORTA->SFUNC[1] |= 0x00000606;
	PORTA->SANALOG  |= 0x00000580; 
	PORTA->SPWR[0]  |= 0x0033c000;
		
	//======================= настройка SSP ===================================================
	
	CLK_CNTR->PER1_CLK |= 0x00000004;	// разрешение тактирования SSP0
	CLK_CNTR->SSP0_CLK |= 0x00010000;
	
	ssp_deinit();

	MDR_SSP0->CR0  = 0x00000007;
	MDR_SSP0->CPSR = 0x0000004e;
	MDR_SSP0->CR1  = 0x00000002; 
	
	PORTA->SRXTX |= (1 << 9);
}


void ic_sector_erase(uint32_t addr){
	write_enable();
	
	unprotect_sector(addr);
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x00d8;
	MDR_SSP0->DR = (addr >> 16);
	MDR_SSP0->DR = (addr >> 8);
	MDR_SSP0->DR = addr;
	
	while(SSP_CHECK_TX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
		
	PORTA->CRXTX |= (1 << 9);
	
	clear_fifo();
}

void ic_chip_erase(){
	write_enable();
	unprotect_sector(0x00000000);
	unprotect_sector(0x00010000);
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x0060;

	while(SSP_CHECK_TX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
		
	PORTA->CRXTX |= (1 << 9);
	
	clear_fifo();
}

void ic_byte_program(uint32_t addr, uint8_t byte){
	write_enable();
	
	delay_mem(100);
	
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x0002;					//код операции
	MDR_SSP0->DR = (addr >> 16);    //адрес
	MDR_SSP0->DR = (addr >> 8);
	MDR_SSP0->DR = addr;
	MDR_SSP0->DR = (uint16_t) byte;
	
	while(SSP_CHECK_TX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
		
	PORTA->SRXTX |= (1 << 9);
	
	clear_fifo();
}

uint8_t  ic_read_status(){
	PORTA->CRXTX |= (1 << 9);
	//uint16_t trash = 0;
	
	MDR_SSP0->DR = 0x0005;
	MDR_SSP0->DR = 0x0000;
	
	while(SSP_CHECK_RX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
	PORTA->SRXTX |= (1 << 9);
	
	//trash = MDR_SSP0->DR;
	MDR_SSP0->DR;
	return MDR_SSP0->DR;
}

uint16_t ic_read_id(){
	uint16_t data = 0;
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x009f;
	MDR_SSP0->DR = 0x0000;
	MDR_SSP0->DR = 0x0000;
	
	while(SSP_CHECK_RX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
	PORTA->SRXTX |= (1 << 9);
	
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	data = (data << 8);
	data |= MDR_SSP0->DR;
	return data;
}

uint8_t	 ic_read_byte(uint32_t addr){
	uint16_t data = 0;
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x000b;					//код операции
	MDR_SSP0->DR = (addr >> 16);    //адрес
	MDR_SSP0->DR = (addr >> 8);
	MDR_SSP0->DR = addr;
	MDR_SSP0->DR = 0xdead;					//фиктивный байт
	MDR_SSP0->DR = 0xbaba;					//для тактирования приема
	
	
	while(SSP_CHECK_RX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
	
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	data = MDR_SSP0->DR;
	
	PORTA->SRXTX |= (1 << 9);
	
	return data;
}

void	 ic_read_array(uint32_t addr, uint8_t *array, uint16_t size){
	uint16_t i = 0;
	
	PORTA->CRXTX |= (1 << 9);
	
	MDR_SSP0->DR = 0x000b;					//код операции
	MDR_SSP0->DR = (addr >> 16);    //адрес
	MDR_SSP0->DR = (addr >> 8);
	MDR_SSP0->DR = addr;
	MDR_SSP0->DR = 0xdead;					//фиктивный байт
	
	while(SSP_CHECK_RX_EMPTY() == 0){}
	while(SSP_CHECK_BUSY() == 1){}
	
	trash = MDR_SSP0->DR;
	trash = MDR_SSP0->DR;
	trash = MDR_SSP0->DR;
	trash = MDR_SSP0->DR;
	trash = MDR_SSP0->DR;

	for(i = 0; i < size; i++){
			MDR_SSP0->DR = 0xdead;
			
			while(SSP_CHECK_RX_EMPTY() == 0){}
			while(SSP_CHECK_BUSY() == 1){}
				
			array[i] = MDR_SSP0->DR;
	}
	
	PORTA->SRXTX |= (1 << 9);
}
