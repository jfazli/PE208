/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <stdio.h>
#include "sni.h"
#include "LLMJVM.h"
#include <RTL.h>
#include "sram_driver.h"

#include "stm32f4xx.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"
#include "stm32f4xx_fmc.h"
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_lcd.h"
#include "stm324x9i_eval_fmc_sdram.h"

#define MICROJVM_STACK_SIZE 2048

static U64 javaTaskStack[(MICROJVM_STACK_SIZE)/8];
__task void javaTask (void);
void* vm;

__task void javaTask (void) {
	int32_t err;
	int32_t exitcode;

	printf("VM START\n");
	err = SNI_startVM(vm, 0, NULL);
	if(err < 0){
		//Error occurred
		if(err == LLMJVM_E_EVAL_LIMIT){
			printf("Evaluation limits reached.\n");
		}
		else{
			printf("VM execution error (err = %d).\n", err);
		}
	}
	else{
		//VM execution ends normally
		exitcode = SNI_getExitCode(vm);
		printf("VM END (exit code = %d)\n", exitcode);
	}
	
	SNI_destroyVM(vm);
		
	os_tsk_delete_self();
} 

void board_init(void)
{
		/* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
  SDRAM_Init(); //TODO: move in LLDISPLAY init
	
	// Clean LCD frame buffer data in SDRAM
	uint16_t data = LCD_COLOR_BLACK;
	for(int i = 0; i < (LCD_PIXEL_WIDTH*LCD_PIXEL_HEIGHT*16/8); i += 2)
	{
		SDRAM_WriteBuffer(&data,i,1);
	}

	// NOR_Init();
}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void DEBUG_IO_init(){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_9;	/*IO_DEBUG*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}


int main(void)
{
	printf("START\n");

	board_init();
	MICRO_TIMER_Init();
	MICRO_SPI_Init();
	MICRO_SERIAL_Init();
	DEBUG_IO_init();
	vm = SNI_createVM();

	if(vm == NULL){
		printf("VM initialization error.\n");
	}
	else {
			os_sys_init_user(javaTask, 10, &javaTaskStack, sizeof(javaTaskStack));
	}
	
	printf("END\n");
}
