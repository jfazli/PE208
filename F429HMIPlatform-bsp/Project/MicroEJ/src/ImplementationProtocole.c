/*!*******************************************************************************       
 * @file     ImplementationProtocole.c
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
#include "PCOM_Time.h"
#include "PCOM_Uart.h"
#include "PCOM_Spi.h"
#include "PCOM_Communication.h"
#include "PCOM_Time.h"
#include "APPLI_InteractiveCookware.h"
#include "ImplementationProtocole.h"
#include "PCOM_Gpio.h"
//===== DEFINEs  ================================================================

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
uint8_t ReceptionTrameUart;
const uint8_t SizeOfFrameTablette = 6;
//===== STRUCTUREs ==============================================================
u_PCOM_DataReceveidFirstOctet dataFO;

t_PCOM_Send FrameFoyerEnvoie[2];
t_PCOM_Acq	t_taskPcomAcq;
t_FramePoeleRecu FramePoeleRecu;
t_FrameTabletteEnvoie FrameTabletteEnvoie;
t_FrameTabletteRecu FrameTabletteRecu;
t_FrameFoyerRecu FrameFoyerRecu[2];
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
void IP_Initialisation(void)
{
	//Initialisation de la communication du protocole
	PCCOM_CommunicationInit();

	PCCOM_DeclareEqp(EQP_1,ID_EQP_SL_1,SPI_SLAVE);
	PCCOM_DeclareEqp(EQP_2,ID_EQP_SL_2,SPI_SLAVE);
	PCCOM_DeclareEqp(EQP_POELE,ID_EQP_POELE,SERIAL);
	PCCOM_DeclareEqp(EQP_TABLETTE,ID_EQP_TABLETTE,SERIAL);

	PCCOM_DeclareSlaveTalk(EQP_1,GPIOB,GPIO_Pin_5,ENABLE);
	PCCOM_DeclareSlaveSelect(EQP_1,GPIOD,GPIO_Pin_2,ENABLE);
	/*slave 2*/
	PCCOM_DeclareSlaveTalk(EQP_2,GPIOG,GPIO_Pin_14,ENABLE);
	PCCOM_DeclareSlaveSelect(EQP_2,GPIOC,GPIO_Pin_9,ENABLE);
}



void IP_SpiLoopSending(void)
{
	jchar bEnd=0;

	if(PCCOM_GetSlaveTalk(EQP_1) == 1 && 	PCOM_GetSlaveSelect(EQP_1)==1)
	{

		PCCOM_SetSlaveSelect(EQP_2,1);
		PCCOM_SetSlaveSelect(EQP_1,0);

		bEnd=PCSPI_Cycle();

//		if(bEnd ==1)
		   PCCOM_SetSlaveSelect(EQP_1,1);
	}
	if(PCCOM_GetSlaveTalk(EQP_2) == 1	&& 	PCOM_GetSlaveSelect(EQP_2)==1		)
	{
		PCCOM_SetSlaveSelect(EQP_1,1);
		PCCOM_SetSlaveSelect(EQP_2,0);

		bEnd=PCSPI_Cycle();

//		if(bEnd ==1)
			PCCOM_SetSlaveSelect(EQP_2,1);
	}
	else{
		PCCOM_SetSlaveSelect(EQP_1,1);
		PCCOM_SetSlaveSelect(EQP_2,1);
	}

}



/**
* Gestion des donnée à envoyée au génerateurs:
*auparavant géré par la couche JVM, maintenant géré içi, c'est à dire les datas :
*		- marche/arrêt foyer
*		- puissance foyer
*/
void IP_setDataSpiToSending(uint8_t eqp,uint8_t marcheArret,uint8_t puissanceFoyer)
{
	FrameFoyerEnvoie[eqp].task_DataSending[FOYER_MARCHE_ARRET] = marcheArret;
	FrameFoyerEnvoie[eqp].task_DataSending[FOYER_PUISSANCE] 		=	puissanceFoyer;
	FrameFoyerEnvoie[eqp].task_SizeofData=2;
	FrameFoyerEnvoie[eqp].task_Eqp=eqp;	//non utilisé
}	
/**
* Fonction de réception de trame
*
*/
void IP_ReceptionTrame(void)
{
	static uint8_t OnseshotStartDelay=0;
	uint8_t receiveFrame=0;
	uint8_t dataReceive[10];
	uint8_t sizeData;
	uint8_t sourceEqp;
	uint8_t increment=0;
	
	receiveFrame = PCCOM_ReceiveFrame((uint8_t*)dataReceive,(uint8_t*)&sizeData,(uint8_t*)&sourceEqp);

	/*si reception nouvelle trame*/
	if(receiveFrame== 1)
	{
		/*Si la trame reçu vient de la poele*/
		if(sourceEqp == ID_EQP_POELE)
		{
			FramePoeleRecu.BoutonsEtChampMagnetique.Rcv_Data=dataReceive[0];
			FramePoeleRecu.TemperatureMesure=(dataReceive[1]*256+dataReceive[2]) /10;
			PCTIME_InitialiseTempoStart(&t_tempsNonreceptionTrameUart);
			/*FIXME commenter sur version precedente*/
			ReceptionTrameUart=1;
			
		}
		else if(sourceEqp == ID_EQP_TABLETTE)
		{
			FrameTabletteRecu.NumeroEtape=dataReceive[0];
			FrameTabletteRecu.TempEtape= dataReceive[1];
			FrameTabletteRecu.PuissanceConsigne=dataReceive[2];
			FrameTabletteRecu.TemperatureConsigne=(dataReceive[3]*256+dataReceive[4]) /*10*/;
			FrameTabletteRecu.CoeffKp= dataReceive[5];
			FrameTabletteRecu.CoeffKi= dataReceive[6];
			
		}
		else if(sourceEqp == ID_EQP_SL_1)
		{
			FrameFoyerRecu[0].PuissanceMesure =  dataReceive[0];
		}
		else if(sourceEqp == ID_EQP_SL_2)
		{
			FrameFoyerRecu[1].PuissanceMesure =  dataReceive[0];
		}
	}
	
}

void IP_EmissionTrameFoyer(void)
{
	static uint8_t eqp=0;
	
	eqp=!eqp;
	PCCOM_SendFrame(eqp,FrameFoyerEnvoie[eqp].task_DataSending,FrameFoyerEnvoie[eqp].task_SizeofData);
}

void IP_EmissionTrameTablette(void)
{
	uint8_t bufferData[SizeOfFrameTablette];
	FrameTabletteEnvoie.recette.Bit.NumeroEtape = FrameTabletteRecu.NumeroEtape;

	bufferData[0] = FrameTabletteEnvoie.PuissanceTable;
	bufferData[1] = FrameTabletteEnvoie.recette.data;
	bufferData[2] = FrameTabletteEnvoie.TempsEnCours>>8;//MSB
	bufferData[3] = FrameTabletteEnvoie.TempsEnCours & 0x0FF;
	bufferData[4] = (uint8_t)(FrameTabletteEnvoie.Temperature>>8); //MSB
	bufferData[5] = (uint8_t) (FrameTabletteEnvoie.Temperature & 0x00FF) ;	//LSB

	
	PCCOM_SendFrame(EQP_TABLETTE,bufferData,sizeof(bufferData));
}

void IP_AcquittementTrame(void)
{	
	t_taskPcomAcq.task_getResultatAcq=PCCOM_Cycle(&t_taskPcomAcq.task_IdentCycle);
	if(t_taskPcomAcq.task_getResultatAcq==TR_ACQ_ERROR)
	{
		if(t_taskPcomAcq.task_Error == 0){
			t_taskPcomAcq.task_Error=1;
			t_taskPcomAcq.task_IdentError=t_taskPcomAcq.task_IdentCycle;
		}
			PCCOM_FreeFrame();
	}
	else if(t_taskPcomAcq.task_getResultatAcq == TR_ACQ_OK)
	{
		 PCCOM_FreeFrame();
	}
	
}
