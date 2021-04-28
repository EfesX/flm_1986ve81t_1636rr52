#ifndef __1636RR52_H
#define __1636RR52_H

#include "MDR1986VE8T.h"

#define SSP_CHECK_BUSY() 			(MDR_SSP0->SR  & 0x00000010) == 0 ? 0 : 1
#define SSP_CHECK_RX_FULL() 	(MDR_SSP0->SR  & 0x00000008) == 0 ? 0 : 1 // 1 - заполнен
#define SSP_CHECK_RX_EMPTY() 	(MDR_SSP0->SR  & 0x00000004) == 0 ? 0 : 1 // 0 - пуст
#define SSP_CHECK_TX_FULL() 	(MDR_SSP0->SR  & 0x00000002) == 0 ? 0 : 1 // 0 - заполнен
#define SSP_CHECK_TX_EMPTY() 	(MDR_SSP0->SR  & 0x00000001) == 0 ? 0 : 1 // 1 - пуст

void ic_chip_init(void);
void ic_sector_erase(uint32_t addr);
void ic_chip_erase(void);
void ic_byte_program(uint32_t addr, uint8_t byte);
uint8_t  ic_read_status(void);
uint16_t ic_read_id(void);
uint8_t	 ic_read_byte(uint32_t addr);
void	 ic_read_array(uint32_t addr, uint8_t *array, uint16_t size);

void flm_ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf);
void flm_sector_erase(unsigned long adr);
void ssp_deinit(void);

#endif /* __1636RR52_H */

