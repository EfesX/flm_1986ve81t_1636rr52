#ifndef __CLK_CONFIG_H
#define __CLK_CONFIG_H	

//#include "main.h"

#define EXTFREQ 8000000


#define LSI_CHECK_STATUS() 	(CLK_CNTR->LSI_STAT  & 0x00100000) == 0 ? 0 : 1
#define HSI_CHECK_STATUS() 	(CLK_CNTR->HSI_STAT  & 0x00100000) == 0 ? 0 : 1
#define LSE_CHECK_STATUS() 	(CLK_CNTR->LSE_STAT  & 0x00100000) == 0 ? 0 : 1
#define HSE0_CHECK_STATUS() (CLK_CNTR->HSE0_STAT & 0x00100000) == 0 ? 0 : 1
#define HSE1_CHECK_STATUS() (CLK_CNTR->HSE1_STAT & 0x00100000) == 0 ? 0 : 1
#define PLL0_CHECK_STATUS() (CLK_CNTR->PLL0_STAT & 0x00100000) == 0 ? 0 : 1

#define CLK_CHECK_STATUS(a) ((CLK_CNTR-> a & 0x00100000) == 0 ? 0 : 1)

#define PER0_CLK_ENABLE(a) (CLK_CNTR->PER0_CLK |= a)
#define PER1_CLK_ENABLE(a) (CLK_CNTR->PER1_CLK |= a)

typedef enum{
	HSE0 = 0x00,
	HSE1 = 0x01,
	HSI  = 0x02,
	LSI  = 0x03,
	LSE  = 0x04
}HSE_SEL;

void clk_init(HSE_SEL SEL, uint8_t Q, uint8_t N);


#endif /* __CLK_CONFIG_H */
