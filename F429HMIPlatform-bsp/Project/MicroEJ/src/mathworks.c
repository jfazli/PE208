

#include <RTL.h>
#include "sp.h"

#define ID 0
#define COMMANDE 0
#define MEASURE 1
#define TEMPO 2

typedef signed long int jint ;
#include <stm32f4xx.h>
typedef unsigned char jchar ;

static void write_SP(ShieldedPlug* database, int ID_num,jint value ) {


	SP_write(database,ID_num,&value); 
	
	while (!SP_isPending(database,ID_num));
}





void _mathworks_task(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
int val;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3,  GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
 

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
	
	ShieldedPlug* DataBase = SP_getDatabase(ID);	

	
	  os_itv_set (5);
		   /* Start ADC3 Software Conversion */ 
		ADC_SoftwareStartConv(ADC3);
		
 for (;;)
 {

	   /* Start ADC3 Software Conversion */ 
	/*	ADC_SoftwareStartConv(ADC3);
		while (!ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC) )
		{
			
			
		}*/
		val= ADC_GetConversionValue(ADC3)*2/10;
		
	 write_SP(DataBase,COMMANDE,val);		//To write in Database

	 
	   /* Start ADC3 Software Conversion */ 
 /* ADC_SoftwareStartConv(ADC3);

		while (!ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC))
		{
			
			
		}*/
		int val= ADC_GetConversionValue(ADC3)/10;
	 write_SP(DataBase,MEASURE,val);			 
	 
	 os_itv_wait ();  

 }
}

