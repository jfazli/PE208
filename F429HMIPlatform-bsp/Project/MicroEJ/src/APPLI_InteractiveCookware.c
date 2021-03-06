/*!*******************************************************************************       
 * @file     APPLI_InteractiveCookware.c
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
#include "PCOM_Time.h"
#include "ImplementationProtocole.h"
#include "APPLI_InteractiveCookware.h"
#include "PID.h"
#include <RTL.h>
//===== DEFINEs  ================================================================

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
volatile int32_t stateAppli;
uint8_t SelectMode;
uint8_t ChoixFoyer;
uint8_t MemorisationPuissance;
uint8_t BoutonDeGrise =0;//bouton gris� lorsqu'il vaut 0 
static uint8_t startRecette;
static struct PCTIME_Tempo t_deroulementRecette;
static uint8_t old_Etape=0;

	static int16_t consignePuissance;
	static int16_t consignetest;
//===== STRUCTUREs ==============================================================
struct PCTIME_Tempo Time_StartSendingFoyer;
struct PCTIME_Tempo Time_StartSendingTablette;
struct PCTIME_Tempo t_toggleScrutation;
struct PCTIME_Tempo t_tempScrutation;
struct PCTIME_Tempo t_tempsNonreceptionTrameUart;
struct PCTIME_Tempo t_tempsEtapeRecette;
struct PID_Parameter t_pid_temperature;
struct PID_Parameter t_pid_puissance;
//===== PROTOTYPEs ==============================================================
void APPLI_DeroulementRecette(void);
void APPLI_InitPID(void);
int16_t APPLI_Regulation(void);
#ifdef SIMULATION_TABLETTE
void APPLI_SimulationFrameTablette(void);
#endif
//===== CODE ====================================================================
/*Global variable*/
static U64 ioeTaskStack_pc[(512/8)]; // that is 4096 bits
OS_TID ioeTaskStackID_pc;

__task void PcomTask (void)
{
	for (;;) 
	{ /* Infinite loop � runs while task1 runs. */ 
		IP_ReceptionTrame();
		IP_SpiLoopSending();
		IP_AcquittementTrame();
		PCTIME_TempoStart(&Time_StartSendingFoyer,2000); /*clock 10�S, envoie toute les 20ms*/
		PCTIME_TempoStart(&Time_StartSendingTablette,TIME_SEND_FRAME_TABLETTE); /*clock 10�S, envoie toute les 1000ms*/
		if(PCTIME_TempoIsElapsed(&Time_StartSendingFoyer))
		{
			IP_EmissionTrameFoyer();
		}
		if(PCTIME_TempoIsElapsed(&Time_StartSendingTablette))
		{
			IP_EmissionTrameTablette();			
		}
		
		APPLI_ReceptionTrameUART();
		APPLI_AppliLogiciel();
	}
}

void APPLI_StartTask(void)
{
	ioeTaskStackID_pc=os_tsk_create_user (PcomTask, 10,&ioeTaskStack_pc,sizeof(ioeTaskStack_pc)); /* Creates task1 with priority 1 (default)*/ 
}

void APPLI_InitPID(void)
{
//	t_pid_temperature.coefficient.a1=	3707;
//	t_pid_temperature.coefficient.a2=	4096;
//	t_pid_temperature.coefficient.b1=	4096;
//	t_pid_temperature.coefficient.b2=	4096;
//	t_pid_temperature.coefficient.c1=	6174;
//	t_pid_temperature.coefficient.c2=	66;
//	t_pid_temperature.coefficient.Kp=12;  
//	t_pid_temperature.coefficient.Ki=110; 
//	t_pid_temperature.coefficient.Kd=0; 
	PID_initialisation(&t_pid_temperature, TIME_100MS,0,100); //regulation temperature
	PID_Parameter(&t_pid_temperature, 3000,30,0); //1er
	/*200�C � vide kp = 3000 ki 30*/
	PID_initialisation(&t_pid_puissance,TIME_100MS,0,100);
	PID_Parameter(&t_pid_puissance,0,50,0);
}

int16_t APPLI_Regulation(void)
{
	static struct PCTIME_Tempo t_regulation;
	static struct PCTIME_Tempo Time_test;
	int16_t temperatureconsigne= FrameTabletteRecu.TemperatureConsigne/10;
	
	
//	if(FrameFoyerRecu[ChoixFoyer].PuissanceMesure>= FrameTabletteRecu.PuissanceConsigne)
//	{
		PID_Loop(&t_pid_puissance,FrameFoyerRecu[ChoixFoyer].PuissanceMesure,&consignetest,FrameTabletteRecu.PuissanceConsigne,1000);
//		PCTIME_TempoStart(&Time_test,5000);
//		if(PCTIME_TempoIsElapsed(&Time_test))
//		{
//			if(FrameFoyerRecu[0].PuissanceMesure< FrameTabletteRecu.PuissanceConsigne-10)
//				consignetest++;			
//			else if(FrameFoyerRecu[ChoixFoyer].PuissanceMesure> FrameTabletteRecu.PuissanceConsigne+10)
//				consignetest--;
//				
//			if(consignetest>100) consignetest=100;
//		}
//	}
//	else
//	{
		PID_Loop(&t_pid_temperature,FramePoeleRecu.TemperatureMesure,&consignePuissance,temperatureconsigne,1000);
		if(consignePuissance >= consignetest)
			consignePuissance = consignetest;
//		}
	
	return consignePuissance;
}

void APPLI_DeroulementRecette(void)
{

	
	static uint8_t countTemps=0;

	uint16_t consigneTable;
		
	/***
	* si on re�oit une nouvelle �tape on r�cuperele temp de la nouvelle �tape qui est re�u en pas de 30s.
	***/
	if(FrameTabletteRecu.NumeroEtape <= 0 && (startRecette >=1))
	{
		startRecette=0;
		FrameTabletteEnvoie.TempsEnCours=0;
		old_Etape =0;
		PCTIME_InitialiseTempoStart( &t_deroulementRecette);
		
	}
	else if(FrameTabletteRecu.NumeroEtape != old_Etape)
	{
//		FrameTabletteEnvoie.TempsEnCours = ((FrameTabletteRecu.TempEtape)*30);
		old_Etape = FrameTabletteRecu.NumeroEtape;
		startRecette=1;
		PCTIME_InitialiseTempoStart(&t_deroulementRecette);
	}
	else if(startRecette == 1)
	{
		MemorisationPuissance=APPLI_Regulation();
		
  	PCTIME_TempoStart(&t_deroulementRecette,TIME_1S);
		if(PCTIME_TempoIsElapsed(&t_deroulementRecette))
		{						
			if(FrameTabletteEnvoie.TempsEnCours <= 0)
			{
				startRecette=0;
				MemorisationPuissance=0;
				PCTIME_InitialiseTempoStart(&t_deroulementRecette);	
				FrameTabletteEnvoie.TempsEnCours=0;			
				FrameTabletteRecu.NumeroEtape=0;				
			}
			else
				FrameTabletteEnvoie.TempsEnCours--;
		}
	}
	else
	{
		startRecette=0;
		PCTIME_TempoStart(&t_deroulementRecette,TIME_2S);
		if(PCTIME_TempoIsElapsed(&t_deroulementRecette))
		{
			SelectMode =MODE_INIT;
			old_Etape=0;
			FrameTabletteRecu.NumeroEtape=0;
		}
	}
	
}


void APPLI_ToggleSelectFoyer(void)
{
		PCTIME_TempoStart(&t_toggleScrutation,TIME_TOGGLE_SCRUTATION); //clock 10�S=> 20ms
		if(PCTIME_TempoIsElapsed(&t_toggleScrutation)){	//toggle foyer
			if(ChoixFoyer==0)
			{
					ChoixFoyer=1;
			}
			else if(ChoixFoyer==1)
			{
					ChoixFoyer=0;
			}				
		}
}

void APPLI_AppliLogiciel(void)
{
	/* gestion bouton appuie sur stop ou gris�*/
#ifndef TOUCH_COOK
	if(stateAppli != STATE_APPLI_START)
	{
		SelectMode=MODE_INIT;
		FrameTabletteEnvoie.recette.Bit.MarcheAret=0;
	}
#endif
	switch(SelectMode)
	{
		case MODE_INIT :	
#ifdef SIMULATION_TABLETTE
			APPLI_SimulationFrameTablette();
#endif
			/*TODO : reinit le temps en cours et l'�tape si start recette == 1*/
			if(startRecette>0)
			{
				FrameTabletteEnvoie.TempsEnCours=0;
				startRecette=0;
				FrameTabletteRecu.NumeroEtape=0;
				FrameTabletteEnvoie.recette.Bit.NumeroEtape=0;
				old_Etape=0;
			}
			PCTIME_InitialiseTempoStart( &t_deroulementRecette);
			MemorisationPuissance=0; //Initialisation de la puissance
			IP_setDataSpiToSending(FOYER_FOYER1,0,0); // Mise � l'arr�t du foyer 1
			IP_setDataSpiToSending(FOYER_FOYER2,0,0); //Mise � l'arr�t du foyer 2
			ChoixFoyer=0;
			PCTIME_InitialiseTempoStart(&t_toggleScrutation);
			FrameTabletteEnvoie.recette.Bit.PresencePoele=0;
			//Si il y a appuie sur n'importe quelle bouton ou sur les 2 en m�me temps
			//alors d�marrage mode scrutation
//			if(((FramePoeleRecu.BoutonsEtChampMagnetique.Rcv_Data) & 3) != 0)
			if(ReceptionTrameUart>/*=*/0)
			{
				#ifndef TOUCH_COOK
				if(stateAppli ==STATE_APPLI_START)
				{
					FrameTabletteEnvoie.recette.Bit.MarcheAret=1;
					SelectMode = MODE_SCRUTATION;	
					(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1) =0;	
					(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2) =0;			
				}	
				#else	
					FrameTabletteEnvoie.recette.Bit.MarcheAret=1;
					SelectMode = MODE_SCRUTATION;	
					(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1) =0;	
					(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2) =0;
				#endif
			}
  		break;
  		case MODE_SCRUTATION:
			if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.ChampMagnetique == 1) //si il y a d�tection d'une poele
			{
				if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1 == 1)
				{
					ChoixFoyer=FOYER_FOYER1;
					IP_setDataSpiToSending(FOYER_FOYER1,1,10);
					IP_setDataSpiToSending(FOYER_FOYER2,0,10);
				}
				else if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2 == 1)
				{
					ChoixFoyer=FOYER_FOYER2;
					IP_setDataSpiToSending(FOYER_FOYER2,1,10);
					IP_setDataSpiToSending(FOYER_FOYER1,0,10);
				}
				
				FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1= 0;//r�init les appuie boutons	
				FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2= 0;//r�init les appuie boutons					
				SelectMode=MODE_FONCTIONNEMENT; 
			}
			else	//sinon si pas de d�tection de po�le au bout de 60S, repart en mode init
			{
				/* scrutation de la poele en changeant les foyer p�riodiquement avec une puissance de 10%*/
				/* utilis� par la JVM*/
				APPLI_ToggleSelectFoyer();
				/* utilis�e pour le g�n�rateur avec les valeurs associ�s*/
				/* on ne sauvegarde pas la puissance quand on est en scrutation, seulement en mode fonctionnement*/
				if(ChoixFoyer==FOYER_FOYER1)
				{
					IP_setDataSpiToSending(FOYER_FOYER1,1,10);
					IP_setDataSpiToSending(FOYER_FOYER2,0,10);
				}
				else if(ChoixFoyer==FOYER_FOYER2)
				{
					IP_setDataSpiToSending(FOYER_FOYER2,1,10);
					IP_setDataSpiToSending(FOYER_FOYER1,0,10);
				}
				/*au bout de xs on repart en init*/
//				PCTIME_TempoStart(&t_tempScrutation,TIME_2MIN );
//				if(PCTIME_TempoIsElapsed(&t_tempScrutation))
//				{
//					if(MemorisationPuissance == 0) //si la consigne de puissance vaut 0
//					{
//						MemorisationPuissance=1;		//alors gard� une consigne � 1
//					}
//					SelectMode= MODE_INIT;				//revenir au mode init
//					PCTIME_InitialiseTempoStart(&t_tempScrutation);
//				}
			}
			break;
		case MODE_FONCTIONNEMENT:
#ifdef TOUCH_COOK
/****************Fonctionnement sans la tablette juste avec la poele****************************/
			if(MemorisationPuissance==0)
				MemorisationPuissance=1;
			if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.ChampMagnetique == 0)		//Si il y'a perte du champ magnetique
			{
				SelectMode = MODE_SCRUTATION;	//repart en mode scrutation
			}
			else //si la poele est pr�sente
			{
				if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1 == 1)		//si il y appuie sur le bouton 1, incr�mentation de la consigne de puissance
				{
					#ifndef DEBUG
					MemorisationPuissance++;
					#else
					MemorisationPuissance+=33;
					#endif
					if(MemorisationPuissance >100)
						MemorisationPuissance=100;
				}	
				else if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2 == 1)	//si il y a appuie sur le bouton 2, d�cr�mentation de la consigne de puissance
				{
					MemorisationPuissance--;
					if(MemorisationPuissance<1)
						MemorisationPuissance=1;					
				}
				FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton1= 0;//r�init les appuie boutons	
				FramePoeleRecu.BoutonsEtChampMagnetique.Bit.Bouton2= 0;//r�init les appuie boutons

				IP_setDataSpiToSending(ChoixFoyer,1,MemorisationPuissance);
	
			}
/*****************************************************************************************************/
/****************Fonctionnement sans la tablette avec la tablette****************************/
#else
	#ifndef DEBUG
			if(FramePoeleRecu.BoutonsEtChampMagnetique.Bit.ChampMagnetique == 0)		//Si il y'a perte du champ magnetique
			{
				FrameTabletteEnvoie.recette.Bit.PresencePoele=0;
				SelectMode = MODE_SCRUTATION;	//repart en mode scrutation
			}
			else
			{
	#endif
				FrameTabletteEnvoie.recette.Bit.PresencePoele=(ChoixFoyer+1);
				APPLI_DeroulementRecette();
				IP_setDataSpiToSending(ChoixFoyer,1,MemorisationPuissance);	
	#ifndef DEBUG				
			}
	#endif
#endif
			break;
	}

}

void APPLI_ReceptionTrameUART(void)
{
	if(ReceptionTrameUart == 1)
	{
		PCTIME_TempoStart(&t_tempsNonreceptionTrameUart,TIME_6S); 
		/*Si plus de r�ception de trame uart*/
		if(PCTIME_TempoIsElapsed(&t_tempsNonreceptionTrameUart))
		{
			SelectMode = MODE_INIT;
			ReceptionTrameUart=0;
		}
	}
}




#ifdef SIMULATION_TABLETTE
void APPLI_SimulationFrameTablette(void)
{
		FrameTabletteRecu.NumeroEtape=1;
		FrameTabletteEnvoie.TempsEnCours= 20000;
		FrameTabletteRecu.PuissanceConsigne=2200;
		FrameTabletteRecu.TemperatureConsigne=2000;
		FrameTabletteRecu.CoeffKp= 1;
		FrameTabletteRecu.CoeffKi= 0;	
}
#endif