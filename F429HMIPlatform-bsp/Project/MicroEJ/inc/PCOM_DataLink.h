/********************************************************************************
 *  FONCTION    : Couche de liaison entre la couche basse et la couche haute(utilisateur)
 ********************************************************************************
 *  FICHIER     : PCOM_DataLink.h
 *  PROJET		:
 *  DEV         : JFA
 *  VERSION     : 1.1
 *  OUTIL		: 
 *	MODIF		: Modification noms des fonctions, ajout PC au début de la fonction		
 ********************************************************************************/
#ifndef PCOM_DATALINK_H
#define PCOM_DATALINK_H
//===== INCLUDEs ================================================================
#include "PCOM_Communication.h"
//===== DEFINEs  ===============================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
enum E_StateOfDL{INIT,SEND_STX,SEND_CRC,SEND_PTI,SEND_NET,SEND_DATA,SEND_ETX};

union CheckOfDL_t
{
    unsigned Flags:8;
    struct
    {
        unsigned FrameOk:1;
        unsigned End:1;
        unsigned Start:1;
        unsigned Crc:1;
        unsigned Pti:1;
        unsigned Ident:1;
        unsigned Data:1;
        unsigned :1;

    }Bit;
};


struct DL_FrameInfo
{
    enum COM_TypOfItf TypOfItf;
    enum E_StateOfDL StateOfDL;
    uint8_t data[MAX_SIZE_DATA];
    uint8_t sizeOfData;
    uint8_t crc;
    uint8_t NbOfEqp;
};

struct DL_FrameInfoRcv
{
    union CheckOfDL_t Check;
    NET_FrameInfo_t Network;
    TR_FrameInfo_t Transport;
    enum E_StateOfDL StateOfDL;
    uint8_t data[MAX_SIZE_DATA];
    uint8_t sizeOfData;
    uint8_t crc;
    uint8_t StoreCrc;
    uint8_t ReceivedDle;
    uint8_t CurrentData;
};

extern volatile struct DL_FrameInfo dl_FrameInfoEm;
extern volatile struct DL_FrameInfo dl_FrameInfoRep;
extern volatile struct DL_FrameInfoRcv dl_FrameInfoRecv[NUMBER_ITF];
//===== PROTOTYPEs ==============================================================
void PCDL_InitFrame(void);
BOOLEAN PCDL_SendFrame(uint8_t * pData ,uint8_t SizeOfData);
BOOLEAN PCDL_ReceiveFrame(uint8_t Data, volatile struct DL_FrameInfoRcv *pFrameInfo);
void PCDL_StartOfFrame(void);
void PCDL_StopOfFrame(void);
void PCDL_FillBuff(void * pData,uint8_t * pSizeOfFrame,struct DL_FrameInfo *pFrameInfo);
uint8_t PCDL_CalculateCRC_EM(struct DL_FrameInfo *pFrameInfo);
uint8_t PCDL_CalculateCRC_REC(struct DL_FrameInfoRcv *pFrameInfo);
uint8_t PCDL_GestionDLE(uint8_t DataReceived,volatile struct DL_FrameInfoRcv *pFrameInfo);
uint8_t PCDL_GestionDleSending(uint8_t Data);
#endif
