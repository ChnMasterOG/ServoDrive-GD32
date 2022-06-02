#include "flash.h"

// return 0 if ok else err
uint8_t fmc_write(int _offset, void* _p, uint32_t size)
{
	uint32_t* p32 = _p;
	
	uint32_t EraseCounter = 0x00;
	uint32_t NbrOfPage = 0x00;
	uint32_t Address = FLASH_START_ADDR;
	fmc_state_enum FLASHStatus = FMC_READY;
	if (size % 4 == 0)
	{
		fmc_unlock();
		NbrOfPage = size / FLASH_PAGE_SIZE + 1;
		// erase pages
		for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FMC_READY); EraseCounter++)
		{
			FLASHStatus = fmc_page_erase(FLASH_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
		}
		while ((Address < FLASH_START_ADDR + size) && (FLASHStatus == FMC_READY))
		{
			FLASHStatus = fmc_word_program(Address, *p32++);
			Address = Address + 4;
		}
		fmc_lock();
		if (FLASHStatus != FMC_READY) return 1;
		else return 0;
	}
	else return 1;
}

void fmc_read(int _offset, void* _p, uint32_t size)
{
	uint32_t* p32 = _p;
	uint32_t Address;
	
	for (Address = FLASH_START_ADDR; Address < FLASH_START_ADDR + size; Address += 4)
	{
		*p32++ = REG32(Address);
	}
}
