/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for New Device 256kB Flash                            */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.h"        // FlashOS Structures
#include "1636RR52.h"

#define HSI_CHECK_STATUS() 	(CLK_CNTR->HSI_STAT  & 0x00100000) == 0 ? 0 : 1

void delay(uint32_t count){
	uint32_t tick = count;
	while(tick != 0){
		tick--;
	}
}


/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#define HSE0_CHECK_STATUS() (CLK_CNTR->HSE0_STAT & 0x00100000) == 0 ? 0 : 1

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

	__disable_irq();
	
	BKP->REG_63_TMR0 |= (1<<22);
  BKP->REG_63_TMR1 |= (1<<22);
  BKP->REG_63_TMR2 |= (1<<22);

  /* Reset MAX_CLK bits, MAX_CLK = HSI */
  CLK_CNTR->MAX_CLK = 0x00000000;

  /* Reset PLLn_CONTROL bits */
  CLK_CNTR->PLL0_CLK = 0x00000000;
	CLK_CNTR->PLL1_CLK = 0x00000000;
	CLK_CNTR->PLL2_CLK = 0x00000000;
	
  /* Reset HSEn_CONTROL bits */	
	CLK_CNTR->HSE0_CLK = 0x00000000;
	CLK_CNTR->HSE1_CLK = 0x00000000;
	
  /* Reset PERn_CLK bits except CLKCTRL and BKPCTRL bits */	
	CLK_CNTR->PER0_CLK = 0x00000003;
	CLK_CNTR->PER1_CLK = 0x00000000;
	
	CLK_CNTR->KEY = 0x8555AAA1;
	CLK_CNTR->MAX_CLK = 0x00000000;
	while(HSI_CHECK_STATUS() != 1){}
		
	//CLK_CNTR->HSE0_CLK = (((uint32_t)0x10000000) | ((uint32_t)0x08000000));
	//while(HSE0_CHECK_STATUS() != SUCCESS){}
	//CLK_CNTR->MAX_CLK = ((uint32_t)0x00000002);

	ic_chip_init();
		
  /* Add your Code */
  return (0);                                  // Finished without Errors
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  /* Add your Code */
	// ======================== настройка портов ===========================================
	PORTA->SOE	    = 0x00000000; //PA9 as OUT
	PORTA->SANALOG  = 0x00000000; 
	PORTA->SPULLUP  = 0x00000000;
	PORTA->SPWR[0]  = 0x00000000;
	PORTA->SFUNC[0] = 0x00000000;
	PORTA->SFUNC[1] = 0x00000000;
	PORTA->KEY 			= 0x00000000;
	
	CLK_CNTR->PER0_CLK 	= 0x00000000; // разрешение тактирования PORTA
	
	//======================= настройка SSP ===================================================
	MDR_SSP0->CR1 = 0x00000000;
	ssp_deinit();
	
	CLK_CNTR->PER1_CLK |= 0x00000000;	// разрешение тактирования SSP0
	CLK_CNTR->SSP0_CLK |= 0x00000000;
  return (0);                                  // Finished without Errors
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
	while((ic_read_status() & 0x01) == 1){}
	
  /* Add your Code */
	ic_chip_erase();
	
	while((ic_read_status() & 0x02) == 1){}
	while((ic_read_status() & 0x01) == 1){}
	
	if((ic_read_status() & 0x20) == 1){
		return (1);
	}
	
	
  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
	
	while((ic_read_status() & 0x01) == 1){}
		
  /* Add your Code */
	ic_sector_erase(adr);
	
	while((ic_read_status() & 0x02) == 1){}
	while((ic_read_status() & 0x01) == 1){}
	
	if((ic_read_status() & 0x20) == 1){
		return (1);
	}
	
  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
	
  /* Add your Code */
	uint32_t i = 0;
	
	while((ic_read_status() & 0x01) == 1){}
	for(i = 0; i < sz; i++){
		ic_byte_program((uint32_t) adr + i, (uint8_t) buf[i]);
		
		while((ic_read_status() & 0x02) == 1){}
		while((ic_read_status() & 0x01) == 1){}
			
		if((ic_read_status() & 0x20) == 1){
			return (1);
		}
			
		//delay(1000);
	}
  return (0);                                  // Finished without Errors
}


unsigned long Verify	(unsigned long adr, unsigned long sz, unsigned char *buf){	
	uint32_t i = 0;

	while((ic_read_status() & 0x01) == 1){}

	for(i = 0; i < sz; i++){
		if (ic_read_byte(adr + i) != buf[i]) return (adr + i);
		
		while((ic_read_status() & 0x01) == 1){}
	}
	
	return (0);
}
	

