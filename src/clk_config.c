#include "clk_config.h"

void clk_init(HSE_SEL SEL, uint8_t Q, uint8_t N){
	CLK_CNTR->KEY = _KEY_;
	switch(SEL){
		case HSE0: 
			CLK_CNTR->HSE0_CLK = (CLKCTRL_HSE0_CLK_BYP | CLKCTRL_HSE0_CLK_ON);
			while(HSE0_CHECK_STATUS() != SUCCESS){}
			CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, Q, N);//PLLn, SRC, Q, N
			while(CLK_CHECK_STATUS(PLL0_STAT) != 1){}
			CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_HSE0div1);
			break;
				
		case HSE1: 
			CLK_CNTR->MAX_CLK = CLKCTRL_MAX_CLK_HSE1div1;
			CLK_CNTR->HSE1_CLK = (CLKCTRL_HSE1_CLK_BYP | CLKCTRL_HSE1_CLK_ON);
			while(HSE0_CHECK_STATUS() != SUCCESS){}
			CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, Q, N);//PLLn, SRC, Q, N
			while(CLK_CHECK_STATUS(PLL0_STAT) != 1){}
			break;
				
		default: break;
	};
}

