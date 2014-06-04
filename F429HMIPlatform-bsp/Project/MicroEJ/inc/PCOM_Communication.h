/********************************************************************************
 *  FONCTION    : Couche utilisateur.
 *********************************************************************************
 *  FICHIER		: PCOM_Communication.h
 *  PROJET		:
 *  DEV         : JFA
 *  VERSION     : 1.1
 *  OUTIL		: 
 *	MODIF		: Modification noms des fonctions, ajout PC au début de la fonction	
 *					suppression volatile dans la structure COM_FrameInfo pr union union TR_FrameInfo_t Transport;
 *					Modif fct declare slave select et declare slave talk.
 ********************************************************************************/
#ifndef PCOM_COMMUNICATION_H
#define PCOM_COMMUNICATION_H

//===== INCLUDEs ===============================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
//===== DEFINEs  ===============================================================
#define TR_ACQ_WAIT         0
#define TR_ACQ_REC          1
#define TR_ACQ_FREE         2
#define TR_ACQ_ERROR        3
#define TR_ACQ_REQ          4
#define TR_ACQ_OK           5

#define TR_TYPE_FREE        0
#define TR_TYPE_PENDING     1

#define TR_DATA             0
#define TR_ACQ              1
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
extern uint8_t StateOfFrameTr;
extern uint8_t AcqOfFrameTr;
extern BOOLEAN NotAcqFrame;
extern BOOLEAN RepeatFrame;
extern uint8_t SrceTrameAcq;
extern BOOLEAN FreeSending;
//===== STRUCTUREs ==============================================================
enum COM_TypOfItf{SERIAL,SPI_MASTER,SPI_SLAVE};

typedef union
{
    unsigned IDENT:8;
    struct
    {
    	unsigned Src:4;
    	unsigned Dest:4;
    }Bit;
}NET_FrameInfo_t;

typedef union
{
	unsigned PTI:8;
    struct
    {
		unsigned Port:5;
		unsigned Type:2;
		unsigned Id:1;
    }Bit;
}TR_FrameInfo_t;


struct SPI_SlaveIO_t
{
	SIZE_ADRESS_PORT_IO *pPortSS;
	SIZE_PIN			 BitSS;
    uint8_t Enable;
};

struct COM_FrameInfo
{
    NET_FrameInfo_t Network;
    TR_FrameInfo_t Transport;

#if SPI_MASTER_ITF == ENABLE
    struct SPI_SlaveIO_t SlaveSelect;
    struct SPI_SlaveIO_t SlaveTalk;
#elif SPI_SLAVE_ITF == ENABLE
    struct SPI_SlaveIO_t SlaveTalk;
#endif
    enum COM_TypOfItf TypOfItf;

};
extern volatile struct COM_FrameInfo COM_Em[NUMBER_EQP];

#if ENABLE_REPEAT_FRAME == ENABLE
extern struct PCTIME_Tempo WaitAcqFrameTime;
#endif


//===== PROTOTYPEs ==============================================================

/******************************************************************************/
/**
 * PCCOM_CommunicationInit :
 * Initialisation des couches du protocole, doit être utilisé au démarrage de la
 * fonction principale.
 * ******************************************************************************/
void PCCOM_CommunicationInit(void);
/******************************************************************************/
/**
 * PCCOM_Cycle :
 * Gestion acquittement des trames et des répetitions. Cette fonction est donc
 * obligatoire si il y a demande d'acuittement

//This function manage the frame repetition and verifiy if the id acq frame
//received is the same of the id data frame sended
//output: return 1 if error aquittement of the last frame sended and block the
//the sending frame as long as the frame is not free with COM_FreeFrame();
//Ex:
//if(COM_Cycle()) //If error Acq then
//{
//  COM_FreeFrame(); //Free the frame
//}
*******************************************************************************/
uint8_t PCCOM_Cycle(uint8_t * pIdent);

/******************************************************************************/
/**
 * PCCOM_DeclareEqp :
 * Déclarations des équipements du réseau
 * @param   NbOfEqp: Identification d'un équipement par un numéro, doit commencer
 *  par 0 et s'incrémente de 1 à cahque nouvel déclaration.
 * @param   dest: Identifiant de l'équipement
 * @param   eTypOfItf: Déclaration du type d'interface à utiliser pour
 * communiquer avec cette équipement
 * ******************************************************************************/
void PCCOM_DeclareEqp(uint8_t NbOfEqp,uint8_t dest,enum COM_TypOfItf eTypOfItf);
//-------------------------------------------------------------------------------------------------------------
//void COM_DeclareSlaveSelect(   uint8_t NbOfEqp,
//                               uint8_t *Port,
//                               uint8_t Bit,
//                               uint8_t EnableSS);
//This function is used only if this board uses the SPI with a slave select pin 
//and if this board is the master 
//Input1:uint8_t NbOfEqp : Number of the equipment.
//Input2:uint8_t *Port: Select the *port of slave select
//Input3: uint8_t Bit:Select the bit of slave select
//Input4:uint8_t EnableSS: Enable or disable the use slave select
//EX: 
////This eqp is a spi slave and the number of the strucure is 0,the ident of the slave is 0x02
//NET_DeclareEqp(0,0x02,0,SPI_SLAVE);
////The slave select pin of the eqp declared above is LATCbits.LAT2  
//COM_DeclareSlaveSelect(0,&LATC,2,1);
//-------------------------------------------------------------------------------------------------------------
void PCCOM_DeclareSlaveSelect(uint8_t NbOfEqp,SIZE_ADRESS_PORT_IO *Port,SIZE_PIN Bit,uint8_t EnableSS);
//-------------------------------------------------------------------------------
//void COM_SetSlaveSelect(struct NET_FrameInfo *pNetFrameInfo,uint8_t stateSS );
// This funtion Active or disable the slave select pin
//Ex:
//COM_SetSlaveSelect(0,0);  //Activethe slave selct at 0
//SPI_Cycle();                      //Send frame
//COM_SetSlaveSelect(0,1); //Disable slave select at 1
//-------------------------------------------------------------------------------
void PCCOM_SetSlaveSelect(uint8_t NbOfEqp,uint8_t stateSS );
//-------------------------------------------------------------------------------------------------------------
//void COM_DeclareSlaveSelect(   struct NET_FrameInfo *pNetFrameInfo,
//                               uint8_t *Port,
//                               uint8_t Bit,
//                               uint8_t EnableSS);
//This function is used only if this board uses the SPI with a slave talk pin

//Input1:struct NET_FrameInfo *pNetFrameInfo :
//      After to declare the equipement
//      with the following funtion NET_DeclareEqp()in the network layer and
//      select the number of the structure "NetEM[NUMBER_EQP]".
//      Declare the same number of the structure that you want to declare the slave talk.
//Input2:uint8_t *Port: select the *port of slave talk
//Input3: uint8_t Bit:select the bit of slave talk
//Input4:uint8_t EnableSS: Enable or disable the use slave talk
//EX:
////This eqp is a spi slave and the number of the strucure is 0,the ident of the slave is 0x02
//NET_DeclareEqp(0,0x02,0,SPI_SLAVE);
////The slave select pin of the eqp declared above is LATCbits.LAT2
//COM_DeclareSlaveTalk(0,&LATC,2,1);
//-------------------------------------------------------------------------------------------------------------

void PCCOM_DeclareSlaveTalk(uint8_t NbOfEqp,SIZE_ADRESS_PORT_IO *Port,SIZE_PIN Bit,uint8_t EnableSS);
//-------------------------------------------------------------------------------
//void COM_SendFrame( uint8_t * pframe,
//                    uint8_t sizeOfFrame,
//                    struct NET_FrameInfo *pNetFrame);
//This function send a frame at the eqp selected.
//Input1: uint8_t * pframe : put the adress of data to sending
//I2:uint8_t sizeOfFrame : The size of data sending with sizeof()
//I3:struct NET_FrameInfo *pNetFrame: The stucture of equipement selected
//-------------------------------------------------------------------------------
void PCCOM_SendFrame(uint8_t NbOfEqp,uint8_t *pData,uint8_t sizeOfData);
//-------------------------------------------------------------------------------
//BOOLEAN COM_ReceiveFrame(uint8_t * pframe,uint8_t *sizeOfFrame, uint8_t *pSourceEqp);
//When you receive the frame, the function return 1 and store in the adress of
//pframe.
//Input1: uint8_t * pframe : Recovery the data who is store to pframe adress
//I2:uint8_t *sizeOfFrame : Recovery the size who is store to sizeOfFrame adress
//Ouput:Return 1 if you receive a frame 
//-------------------------------------------------------------------------------
BOOLEAN PCCOM_ReceiveFrame(uint8_t * pframe,uint8_t *sizeOfFrame,uint8_t *pSourceEqp);

//-------------------------------------------------------------------------------
//BOOLEAN COM_FreeFrame(void);
//This function free the frame when if not ack.
//Ex:
//if(COM_Cycle()) //If error Acq then
//{
//  COM_FreeFrame(); //Free the frame
//}
//-------------------------------------------------------------------------------
void PCCOM_FreeFrame(void);

uint8_t PCOM_GetSlaveSelect(uint8_t NbOfEqp);
void PCCOM_SetSlaveTalk(uint8_t NbOfEqp,uint8_t stateSS );
BOOLEAN PCCOM_GetSlaveTalk(uint8_t NbOfEqp);

#endif
