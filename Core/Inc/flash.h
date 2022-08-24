#ifndef  __FLASH__
#define  __FLASH__

#include <stdint.h>
#include "main.h"

#define FLASH_START_ADDR  ((uint32_t)0x8003C00)	// page=15
#define FLASH_PAGE_SIZE   ((uint16_t)0x400)			//1024

uint8_t fmc_write(int _offset, void* _p, uint32_t size);
void fmc_read(int _offset, void* _p, uint32_t size);

#endif
