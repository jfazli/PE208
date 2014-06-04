/********************************************************************************
 * Fonction : Liaison entre la couche utilisateur et la couche physique 
 *********************************************************************************
 *	FICHIER	: PCOM_Datalink.c
 *	PROJET	: 
 *	DEV		: JFA
 *  VERSION : 1.1
 *	OUTIL	: 
 * 	MODIFICATION : Modification noms des fonctions DL_xxx en PCDL_xxx	
 ********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Communication.h"
#include "PCOM_Uart.h"
#include "PCOM_Time.h"
#include "PCOM_DataLink.h"
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
volatile struct DL_FrameInfo dl_FrameInfoEm;
volatile struct DL_FrameInfo dl_FrameInfoRep;
volatile struct DL_FrameInfoRcv dl_FrameInfoRecv[NUMBER_ITF];
//===== PROTOTYPEs ==============================================================

//===== CODE ====================================================================
void PCDL_InitFrame(void)
{
    dl_FrameInfoEm.StateOfDL = INIT; //Initialisation au démarrage de la carte de l'émission
    for(uint8_t i=0;i<NUMBER_ITF;i++)
        dl_FrameInfoRecv[i].Check.Flags=0;  //Initialisation du check des octets reçus
}

uint8_t PCDL_CalculateCRC_EM(struct DL_FrameInfo *pFrameInfo)
{
    uint16_t CalculateCrc=0;
    uint8_t i;

    CalculateCrc += STX;

    for(i=0; i<(pFrameInfo->sizeOfData);i++)
    {
       CalculateCrc+=(pFrameInfo->data[i]);
    }

    CalculateCrc+=COM_Em[pFrameInfo->NbOfEqp].Transport.PTI;
    CalculateCrc+=COM_Em[pFrameInfo->NbOfEqp].Network.IDENT;

    CalculateCrc += ETX;


    CalculateCrc %= 0xFF;

    return CalculateCrc;
}

uint8_t PCDL_CalculateCRC_REC(struct DL_FrameInfoRcv *pFrameInfo)
{
    uint16_t CalculateCrc=0;
    uint8_t i;

    CalculateCrc += STX;


    for(i=0; i<(pFrameInfo->sizeOfData);i++)
    {
       CalculateCrc+=(pFrameInfo->data[i]);
    }

    CalculateCrc+=pFrameInfo->Transport.PTI;
    CalculateCrc+=pFrameInfo->Network.IDENT;

    CalculateCrc += ETX;


    CalculateCrc %= 0xFF;

    return CalculateCrc;
}

void PCDL_StartOfFrame(void)
{
    if(dl_FrameInfoEm.StateOfDL==INIT) //On s'assure que l'on ne redémarre pas une trame au millieu d'une autre
    {
        dl_FrameInfoEm.StateOfDL = SEND_STX;
    }
	
}

void PCDL_StopOfFrame(void)
{
    StateOfFrameTr=TR_TYPE_FREE;//permet de pouvoir relancer une nouvelle trame à la fin de l'émission de ts les octets
    if(AcqOfFrameTr==TR_ACQ_REQ) //si
    {
        AcqOfFrameTr=TR_ACQ_WAIT;
#if ENABLE_REPEAT_FRAME == ENABLE
        if(RepeatFrame == 0)
           WaitAcqFrameTime.Request=0;
#endif
    }
    dl_FrameInfoEm.StateOfDL = INIT;
}


BOOLEAN PCDL_SendFrame(uint8_t * pData ,uint8_t SizeOfData)
{
    static uint8_t currentData=0;
    static uint8_t bDLEsending=0;
    BOOLEAN bEnd=0;
    uint8_t *pFrame;


    pFrame = (uint8_t *)pData;

    switch(dl_FrameInfoEm.StateOfDL)
    {
        case INIT :
//                        *pFrame = 0;
                        bDLEsending=0;
                        currentData=0;
                        dl_FrameInfoEm.sizeOfData=0;
                        bEnd=1;
                        break;
        case SEND_STX:
                        *pFrame = STX;
                        dl_FrameInfoEm.StateOfDL = SEND_CRC;
                        dl_FrameInfoEm.sizeOfData= SizeOfData;
                        break;
        case SEND_CRC:                      
                        if(PCDL_GestionDleSending(dl_FrameInfoEm.crc))
                        {
                            *pFrame=DLE;
                        }
                        else
                        {
                            *pFrame=dl_FrameInfoEm.crc;
                            dl_FrameInfoEm.StateOfDL = SEND_PTI;
                        }
                        break;
        case SEND_PTI:
                        if(PCDL_GestionDleSending(COM_Em[dl_FrameInfoEm.NbOfEqp].Transport.PTI))
                        {
                            *pFrame=DLE;
                        }
                        else
                        {
                             *pFrame=COM_Em[dl_FrameInfoEm.NbOfEqp].Transport.PTI;
                             COM_Em[dl_FrameInfoEm.NbOfEqp].Transport.Bit.Type=TR_DATA;
                             dl_FrameInfoEm.StateOfDL = SEND_NET;
                        }
                            
                        break;
        case SEND_NET:
                        if(PCDL_GestionDleSending(COM_Em[dl_FrameInfoEm.NbOfEqp].Network.IDENT))
                        {
                            *pFrame=DLE;
                        }
                        else
                        {
                           *pFrame=COM_Em[dl_FrameInfoEm.NbOfEqp].Network.IDENT;
                            dl_FrameInfoEm.StateOfDL = SEND_DATA;
                        }                
                        break;
        case SEND_DATA:

                        if(PCDL_GestionDleSending(dl_FrameInfoEm.data[currentData]))
                        {
                            *pFrame=DLE;
                        }
                        else
                        {
                            *pFrame=dl_FrameInfoEm.data[currentData];
                             currentData++;
                        }
                        if(currentData >= dl_FrameInfoEm.sizeOfData)
                        {
                            dl_FrameInfoEm.StateOfDL = SEND_ETX;
                            currentData=0;
                        }
                        break;
        
        case SEND_ETX:

                        *pFrame = ETX;
                        bEnd=1;
                        dl_FrameInfoEm.StateOfDL=SEND_STX;
                        dl_FrameInfoEm.sizeOfData=0;
                        break;
        default:
                        *pFrame = 0;
                        bEnd=1;
                        break;
    }
    return bEnd;
}

BOOLEAN PCDL_ReceiveFrame(uint8_t Data,volatile struct DL_FrameInfoRcv *pFrameInfo)
{
    BOOLEAN bEnd=0;

	
 //   if((dl_FrameInfoRecv[NUMBER_ITF_SPI].Check.Bit.FrameOk)==0)
	/*modifié à la volé mais non validé*/
	if((pFrameInfo->Check.Bit.FrameOk)==0)
	{
        if(pFrameInfo->ReceivedDle == 0)
        {
            if(Data == STX)
            {
                pFrameInfo->Check.Flags=0;
                pFrameInfo->StateOfDL=SEND_STX;
            }
            else if(Data == ETX)
            {

                pFrameInfo->StateOfDL=SEND_ETX;
            }
        }
        else
            (pFrameInfo->ReceivedDle) =0;
        

        switch(pFrameInfo->StateOfDL)
        {

            case INIT:
                            break;
            case SEND_STX:
                            pFrameInfo->StateOfDL= SEND_CRC;
                            pFrameInfo->CurrentData=0;
                            pFrameInfo->Check.Bit.Start=1;

                            break;
            case SEND_CRC:
                            pFrameInfo->crc=Data;
                            pFrameInfo->StateOfDL= SEND_PTI;
                            pFrameInfo->Check.Bit.Crc=1;
                            break;
            case SEND_PTI:
                            pFrameInfo->StateOfDL= SEND_NET;
                            pFrameInfo->Transport.PTI=Data;
                            pFrameInfo->Check.Bit.Pti=1;

                            break;
            case SEND_NET:
                            pFrameInfo->StateOfDL= SEND_DATA;
                            pFrameInfo->Network.IDENT=Data;
                            pFrameInfo->Check.Bit.Ident=1;

                            break;
            case SEND_DATA:

                            pFrameInfo->data[pFrameInfo->CurrentData]=Data;
                            pFrameInfo->CurrentData++;
                            pFrameInfo->Check.Bit.Data=1;

                            break;
            case SEND_ETX:
                            pFrameInfo->sizeOfData = pFrameInfo->CurrentData;
                            pFrameInfo->Check.Bit.End=1;

                            if((pFrameInfo->Check.Flags) >= 62)
                            {
                                pFrameInfo->Check.Flags=0;
                                bEnd=1;
                                pFrameInfo->Check.Bit.FrameOk=1;
                            }
                            pFrameInfo->StateOfDL=INIT;

                            break;
        }
    }
    return bEnd;
}

uint8_t PCDL_GestionDLE(uint8_t DataReceived,volatile struct DL_FrameInfoRcv *pFrameInfo)
{
    uint8_t bEnd=0;

    if(pFrameInfo->ReceivedDle==1)
    {
    	bEnd=1;
    }
    else if(DataReceived== DLE)
    {
    	pFrameInfo->ReceivedDle+=1;
    }
    else
    	bEnd=1;

    return bEnd;
}

uint8_t PCDL_GestionDleSending(uint8_t Data)
{
    uint8_t bEnd=0;
    static uint8_t DleSended=0;

    if(DleSended == 1)
    {
        bEnd=0;
        DleSended=0;

    }
    else if( (Data == STX) || (Data == DLE) || (Data == ETX))
    {
        DleSended=1;
        bEnd=1;
    }
    
    return bEnd;
}
