/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_fsmc_sram.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_fsmc_sram.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "rgb_lcd.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"
#include "stm32f4xx_fmc.h"
#else
#error Invalid MCU
#endif
#include "sram_driver.h"

#define SRAM_TEST

#ifdef SRAM_TEST

#define MEM_RAM 0
#define MEM_SRAM 1

#define MEM_TO_TEST MEM_SRAM

#if MEM_TO_TEST == MEM_RAM
#define MEM_MAX (128*1024)
#define MEM_START (0x20000000)
#else
#define MEM_MAX (2*1024*1024)
#define MEM_START (((uint32_t)(0x60000000 | 0x04000000)))
#endif

#define SRAM_WRITE 0
#define SRAM_READ 1

int SRAM_access(uint32_t addr, uint8_t write, uint32_t value, int cpt)
{
	if (addr >= MEM_MAX)
		return cpt;	// drop
	
	addr += MEM_START;
	//printf("addr = 0x%x\n",addr);
	if (write == SRAM_WRITE)
	{
		(*(uint32_t*)addr) = value; 
		return cpt;
	}
	else if ((*(uint32_t*)addr) != value)
	{
		printf("ERROR %d, @0x%x read 0x%x, expected 0x%x\n", cpt, addr, (*(uint32_t*)addr), value);
		while(1);
	}
	return cpt + 1;
}





void SRAM_test(void)
{
	int i,inc,cpt,j;
	cpt = 0;
	while(1)
	{
		for(inc=0x1ffff;--inc>=0;)
		{

			for(i=3;--i;i>=0)
			{
				
				/*for ( j =1000;--j;j>=0)
				{
					cpt = SRAM_access(0x5+(inc*i),0,0xaaaaaaaa,cpt);
 
					cpt = SRAM_access(0x5+(inc*i),1,0xaaaaaaaa,cpt);
					
					cpt = SRAM_access(0x132565+(inc*i),0,0x12345678,cpt);
 
					cpt = SRAM_access(0x132565+(inc*i),1,0x12345678,cpt);
				}*/
				uint8_t write = SRAM_WRITE;
				cpt = SRAM_access(0x5+(inc*i),write,0x68,cpt);
				cpt = SRAM_access(0x55+(inc*i),write,0x08,cpt);
				cpt = SRAM_access(0xaaa+(inc*i),write,0xaaaaaaaa,cpt);
				cpt = SRAM_access(0xacaa+(inc*i),write,0x12345678,cpt);
				write = SRAM_READ;
				cpt = SRAM_access(0x5+(inc*i),write,0x68,cpt);
				cpt = SRAM_access(0x55+(inc*i),write,0x08,cpt);
				cpt = SRAM_access(0xaaa+(inc*i),write,0xaaaaaaaa,cpt);
				cpt = SRAM_access(0xacaa+(inc*i),write,0x12345678,cpt);
				
				write = SRAM_WRITE;
				cpt = SRAM_access(0x5555+(inc*i),write,0x55555555,cpt);
				cpt = SRAM_access(0x985+(inc*i),write,0x98,cpt);
				cpt = SRAM_access(0x6855+(inc*i),write,0x45,cpt);
				cpt = SRAM_access(0x32565+(inc*i),write,0x89,cpt);
				write = SRAM_READ;
				
				cpt = SRAM_access(0x5555+(inc*i),write,0x55555555,cpt);
				cpt = SRAM_access(0x985+(inc*i),write,0x98,cpt);
				cpt = SRAM_access(0x6855+(inc*i),write,0x45,cpt);
				cpt = SRAM_access(0x32565+(inc*i),write,0x89,cpt);
				
				write = SRAM_WRITE;
				cpt = SRAM_access(0x132565+(inc*i),write,0x10,cpt);
				cpt = SRAM_access(0x13+(inc*i),write,0xffff,cpt);
				cpt = SRAM_access(0x130+(inc*i),write,0x0,cpt);
				cpt = SRAM_access(0x128+(inc*i),write,0xf0f0f0f0,cpt);
				cpt = SRAM_access(0x1000+(inc*i),write,0x0f0f0f0f,cpt);
				cpt = SRAM_access(0x1280+(inc*i),write,0xffffffff,cpt);
				write = SRAM_READ;
				cpt = SRAM_access(0x132565+(inc*i),write,0x10,cpt);
				cpt = SRAM_access(0x13+(inc*i),write,0xffff,cpt);
				cpt = SRAM_access(0x130+(inc*i),write,0x0,cpt);
				cpt = SRAM_access(0x128+(inc*i),write,0xf0f0f0f0,cpt);
				cpt = SRAM_access(0x1000+(inc*i),write,0x0f0f0f0f,cpt);
				cpt = SRAM_access(0x1280+(inc*i),write,0xffffffff,cpt);			
			}
		}
	}
}
#endif

void SRAM_initialize(void)
{
  /* Configure FSMC Bank 1 NOR/SRAM2  */
 SRAM_Init();

//  SDRAM_Init();

	
	
	#ifdef SRAM_TEST
	//SRAM_test();
	#endif
	
}

