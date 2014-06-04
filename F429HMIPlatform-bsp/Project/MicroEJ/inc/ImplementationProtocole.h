/**********************************************************************       
* @file    ImplementationProtocole.h
* @brief   Integration Protocole V1.2.1 => manque les commentaires de cette version
* @version  1.0
* @date     26-07-2013
* @author   J.Fazli
**********************************************************************/
#ifndef  IMPLEMENTATIONPROTOCOLE_H
#define  IMPLEMENTATIONPROTOCOLE_H
//===== INCLUDEs ===============================================================
#include <stdint.h>
//===== DEFINEs  ===============================================================
#define FOYER_MARCHE_ARRET 		1
#define FOYER_PUISSANCE				0


//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
extern uint8_t ReceptionTrameUart;

//===== STRUCTUREs ==============================================================
typedef struct{
	uint8_t task_Eqp;	//non utilisé
	uint8_t task_SizeofData;
	uint8_t task_DataSending[2]; //Marche/arret & Puissance
}t_PCOM_Send;

typedef struct{
	uint8_t task_getResultatAcq;
	uint8_t task_IdentCycle;
	uint8_t task_Error;
	uint8_t task_IdentError;
}t_PCOM_Acq;


/*Trame d'échange avec la poele*/
typedef union{
	uint8_t Rcv_Data;
	struct{
		uint8_t Bouton1:1;	//LSB
		uint8_t Bouton2:1;
		uint8_t ChampMagnetique:1; 
		uint8_t IdentUstensile:5; //MSB
	}Bit;
}u_PCOM_DataReceveidFirstOctet;

typedef struct{
	uint16_t TemperatureMesure;
	u_PCOM_DataReceveidFirstOctet BoutonsEtChampMagnetique;
}t_FramePoeleRecu; //reception trame toutes les 200ms


/* structure des trames d'échange avec la tablette*/
typedef union{
	uint8_t data;
	struct{
		uint8_t PresencePoele:2;
		uint8_t MarcheAret:1;
		uint8_t NumeroEtape:4;
		uint8_t :1;
	}Bit;
}t_InfosRecette;


typedef struct{
	uint8_t PuissanceTable; //sur 7bits
	t_InfosRecette recette;
	uint8_t TempsEnCours;
	uint16_t Temperature;
}t_FrameTabletteEnvoie; //IHM qui envoie à la tablette


typedef struct{
	uint8_t NumeroEtape;
	uint8_t TempEtape;
	uint8_t PuissanceConsigne; //sur 7bits
	uint16_t TemperatureConsigne; //la reception de la temperature est recu en 2 octet de 8 bits
	uint8_t CoeffKp;
	uint8_t CoeffKi;
}t_FrameTabletteRecu;

typedef struct{
	uint16_t PuissanceMesure;
}t_FrameFoyerRecu;

extern t_FramePoeleRecu FramePoeleRecu;
extern t_FrameTabletteRecu FrameTabletteRecu;
extern t_FrameTabletteEnvoie FrameTabletteEnvoie;
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
void IP_Initialisation(void);
void IP_SpiLoopSending(void);
void IP_ReceptionTrame(void);
void IP_EmissionTrameFoyer(void);
void IP_EmissionTrameTablette(void);
void IP_AcquittementTrame(void);
void IP_setDataSpiToSending(uint8_t eqp,uint8_t marcheArret,uint8_t puissanceFoyer);



#endif
