/********************************************************************************
 *Fonction  : Couche utilisateur
 *********************************************************************************
 *	FICHIER	: PCOM_Communication.c
 *	PROJET	: 
 *	DEV		: JFA
 *  VERSION : 1.1
 *	OUTIL	: 
 * 	MODIFICATION : Modifications nom fonctions de COM_xxx à PCCOM_xxx
 * 					Modif fct declare slave select et declare slave talk.
 ********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_CouchePhysique.h"
#include "PCOM_Config.h"
#include "PCOM_Gpio.h"
#include "PCOM_DataLink.h"
#include "PCOM_Time.h"
#include "PCOM_Uart.h"
#include "PCOM_Spi.h"
#include "PCOM_Communication.h"
//===== DEFINEs  ===============================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
#if SPI_MASTER_ITF == ENABLE
uint8_t Tst_ReceivedDataSlave=0;
#endif
#if SPI_MASTER_ITF == ENABLE || SPI_SLAVE_ITF == ENABLE
uint8_t UsingSlaveSelect=0;
#endif
uint8_t StateOfFrameTr=TR_TYPE_FREE;
uint8_t AcqOfFrameTr = TR_ACQ_FREE;
BOOLEAN NotAcqFrame = 0;
BOOLEAN RepeatFrame = 0;
uint8_t SrceTrameAcq = 0;
BOOLEAN FreeSending = 0;
uint8_t StateOfSS[NUMBER_EQP];

//===== STRUCTUREs ==============================================================
#if ENABLE_REPEAT_FRAME == ENABLE
struct PCTIME_Tempo WaitAcqFrameTime;
#endif
volatile struct COM_FrameInfo COM_Em[NUMBER_EQP];

//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

void PCCOM_CommunicationInit(void)
{  
    PCDL_InitFrame();
    StateOfFrameTr=TR_TYPE_FREE;
}

void PCCOM_DeclareEqp(uint8_t NbOfEqp,uint8_t dest,enum COM_TypOfItf eTypOfItf)
{
    COM_Em[NbOfEqp].Network.Bit.Dest = dest;
    COM_Em[NbOfEqp].TypOfItf = eTypOfItf;// dest interface
    COM_Em[NbOfEqp].Network.Bit.Src =NET_SRC;
    COM_Em[NbOfEqp].Transport.Bit.Type = TR_DATA;
}


void PCCOM_DeclareSlaveSelect(uint8_t NbOfEqp,SIZE_ADRESS_PORT_IO *Port,SIZE_PIN Bit,uint8_t EnableSS)
{
#if SPI_MASTER_ITF == ENABLE
    if(EnableSS)
    {
        COM_Em[NbOfEqp].SlaveSelect.pPortSS= Port;
        COM_Em[NbOfEqp].SlaveSelect.BitSS=Bit;
        COM_Em[NbOfEqp].SlaveSelect.Enable=EnableSS;
        UsingSlaveSelect=1;
    }
#endif
}

void PCCOM_SetSlaveSelect(uint8_t NbOfEqp,uint8_t stateSS )
{
#if SPI_MASTER_ITF == ENABLE
	PCGPIO_WriteBit(COM_Em[NbOfEqp].SlaveSelect.pPortSS, COM_Em[NbOfEqp].SlaveSelect.BitSS, stateSS );
	/*etat de l'IO slave select*/
	StateOfSS[NbOfEqp]=stateSS;
#endif
}


uint8_t PCOM_GetSlaveSelect(uint8_t NbOfEqp)
{
	return StateOfSS[NbOfEqp];
}



void PCCOM_DeclareSlaveTalk(uint8_t NbOfEqp,SIZE_ADRESS_PORT_IO *Port,SIZE_PIN Bit,uint8_t EnableSS)
{
#if SPI_MASTER_ITF == ENABLE || SPI_SLAVE_ITF == ENABLE
    if(EnableSS)
    {
        COM_Em[NbOfEqp].SlaveTalk.pPortSS= Port;
        COM_Em[NbOfEqp].SlaveTalk.BitSS=Bit;
        COM_Em[NbOfEqp].SlaveTalk.Enable=EnableSS;
        UsingSlaveSelect=1;
    }
#endif
}

void PCCOM_SetSlaveTalk(uint8_t NbOfEqp,uint8_t stateSS )
{
#if SPI_MASTER_ITF == ENABLE || SPI_SLAVE_ITF == ENABLE
    	PCGPIO_WriteBit(COM_Em[NbOfEqp].SlaveSelect.pPortSS, COM_Em[NbOfEqp].SlaveSelect.BitSS, stateSS );
#endif
}

BOOLEAN PCCOM_GetSlaveTalk(uint8_t NbOfEqp)
{
#if SPI_MASTER_ITF == ENABLE || SPI_SLAVE_ITF == ENABLE
    BOOLEAN bVal;

    if(PCGPIO_ReadBit(COM_Em[NbOfEqp].SlaveTalk.pPortSS, COM_Em[NbOfEqp].SlaveTalk.BitSS))
    {
        bVal = 1;
    }
    else
        bVal = 0;

    return bVal;
#endif
}


void PCCOM_SendFrame(uint8_t NbOfEqp,uint8_t * pData,uint8_t sizeOfData)
{
    BOOLEAN bEnd=0;
    uint8_t Data=0;
#if SPI_MASTER_ITF == ENABLE
//    uint8_t WriteSingleChar=0;
#endif
    if(StateOfFrameTr==TR_TYPE_FREE)    //Si il n'y a pas de trame en cours de préparation ou d'émission
    if(FreeSending == 0 || NotAcqFrame ==1)    //si la trame précedente à été acquitter ou que la trame est une trame d'acquittement
    {
        StateOfFrameTr=TR_TYPE_PENDING;
        dl_FrameInfoEm.NbOfEqp=NbOfEqp;
        dl_FrameInfoEm.sizeOfData=sizeOfData;
        memcpy(dl_FrameInfoEm.data,pData,sizeOfData);
        dl_FrameInfoEm.crc=PCDL_CalculateCRC_EM(&dl_FrameInfoEm);
        dl_FrameInfoEm.TypOfItf =COM_Em[NbOfEqp].TypOfItf;

#if ENABLE_ACQ_FRAME == ENABLE
        if(!NotAcqFrame) //Si il y a demande de acquittement de trame
        {
            FreeSending=1;
            AcqOfFrameTr=TR_ACQ_REQ;
#if  ENABLE_REPEAT_FRAME == ENABLE
            memcpy(&dl_FrameInfoRep,&dl_FrameInfoEm,sizeof(dl_FrameInfoEm));
#endif

        }
#endif
        PCDL_StartOfFrame();

        switch(COM_Em[NbOfEqp].TypOfItf)
        {
#if SPI_MASTER_ITF == ENABLE
            case SPI_SLAVE: //If the eqp dest is spi slave
                if(1==UsingSlaveSelect)
                    PCCOM_SetSlaveSelect(NbOfEqp,0);

#if SPI_POLLING_INTERRUPT_ITF == 0    //Mode polling
                while(bEnd ==0)
                {
                    bEnd=PCDL_SendFrame((void*)&Data ,dl_FrameInfoEm.sizeOfData);     //prepare the data to send
                    Tst_ReceivedDataSlave=PCSPI_Write_Master(Data);				     //send Data & receive the data from slave
                    if(PCDL_GestionDLE(Tst_ReceivedDataSlave,&dl_FrameInfoRecv[NUMBER_ITF_SPI]))
						PCDL_ReceiveFrame(Tst_ReceivedDataSlave,&dl_FrameInfoRecv[NUMBER_ITF_SPI]);  //NUMBER_ITF_SPI_SLAVE
                };
                if(1==UsingSlaveSelect)
                    PCCOM_SetSlaveSelect(NbOfEqp,1);
                PCDL_StopOfFrame();

             #elif SPI_POLLING_INTERRUPT_ITF == 1  //Mode Interrupt
                PCDL_SendFrame((void*)(&WriteSingleChar) ,dl_FrameInfoEm.sizeOfData);
                BUFFER_SPI=WriteSingleChar;
                //set SPI1 Mode to TX
                SPI_MODE_MASTER_INT = TX;
             #endif
                break;
#endif
#if  SPI_SLAVE_ITF == ENABLE
            case SPI_MASTER:  //If the eqp dest is spi master
                IO_SLAVETALK=1;
                break;
#endif
            case SERIAL:
            #if UART_POLLING_INTERRUPT_ITF == DISABLE //Polling
                while(bEnd ==0)
                {
                    bEnd=PCDL_SendFrame((void*)&Data ,dl_FrameInfoEm.sizeOfData);     //prepare the data to send
                    PCUART_Send(Data);				     //send Data & receive the data from slav
                };
                PCDL_StopOfFrame();
            #else                               //interrupt               
                ENABLE_INT_TX=1;
            #endif
                break;
        }

        
    }
}

BOOLEAN PCCOM_ReceiveFrame(uint8_t * pframe,uint8_t *sizeOfFrame,uint8_t *pSourceEqp)
{
    BOOLEAN bEnd=0;
    BOOLEAN FrameCompletedReceived=0;
    uint8_t NumBerOfEqpDL=0;
    uint8_t i;
    uint8_t EqpFind=0;
    uint8_t GatewayFrame=0;
#if ENABLE_ACQ_FRAME == ENABLE
    uint8_t SendDataNull=0x00;
#endif

    for(i=0;i<NUMBER_ITF;i++)
    {
        if(dl_FrameInfoRecv[i].Check.Bit.FrameOk)
        {
            dl_FrameInfoRecv[i].StoreCrc=PCDL_CalculateCRC_REC(&dl_FrameInfoRecv[i]);
            if(dl_FrameInfoRecv[i].StoreCrc != dl_FrameInfoRecv[i].crc)
            {
                FrameCompletedReceived=0;
                dl_FrameInfoRecv[i].Check.Bit.FrameOk=0;
            }
            else
            {
                NumBerOfEqpDL=i;
                FrameCompletedReceived=1;
                i=NUMBER_ITF;
            }
        }
    }
    if(FrameCompletedReceived== 1)//if the frame is full receive
    {
        FrameCompletedReceived=0;

        for(i=0;i<NUMBER_EQP;i++)
        {
            
            if((dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Dest) != NET_SRC)   //If the dest is different of receiver frame source
            {
                if((dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Dest) == (COM_Em[i].Network.Bit.Dest)) //If The dest is same as dest
                {
                    EqpFind=1; //Frame gateway
                }
            }
            else if( dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Src == (COM_Em[i].Network.Bit.Dest))
            {
                EqpFind=2;
            }
            if(EqpFind>0)
            {
               GatewayFrame=i;
               i=NUMBER_EQP;
            }
        }
        if(EqpFind ==1)//passerelle
        {
#if GATEWAY == 1

            COM_Em[GatewayFrame].Network.Bit.Src= dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Src;
            COM_Em[GatewayFrame].Transport.PTI=dl_FrameInfoRecv[NumBerOfEqpDL].Transport.PTI;
            NotAcqFrame=1;

            PCCOM_SendFrame(GatewayFrame,&(dl_FrameInfoRecv[NumBerOfEqpDL].data),dl_FrameInfoRecv[NumBerOfEqpDL].sizeOfData);
            NotAcqFrame=0;
            
            COM_Em[GatewayFrame].Network.Bit.Src=NET_SRC;
            COM_Em[GatewayFrame].Transport.Bit.Type=TR_DATA;
#endif
        }
        else if(EqpFind ==2 ) //La trame reçue me concerne
        {
            memcpy(pframe,dl_FrameInfoRecv[NumBerOfEqpDL].data,dl_FrameInfoRecv[NumBerOfEqpDL].sizeOfData);
            *sizeOfFrame= dl_FrameInfoRecv[NumBerOfEqpDL].sizeOfData;
            if(dl_FrameInfoRecv[NumBerOfEqpDL].Transport.Bit.Type == TR_ACQ) //Si la trame reçu est une trame d'acquittement
            {
                SrceTrameAcq=dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Src;
                AcqOfFrameTr=TR_ACQ_REC;        //Je préviens COM_CYCLE que j'ai reçu une trame d'acquittement
            }
            else        //Sinon si la trame reçu est une trame de data
            {

                *pSourceEqp=dl_FrameInfoRecv[NumBerOfEqpDL].Network.Bit.Src;
                bEnd=1;
#if ENABLE_ACQ_FRAME == ENABLE

                COM_Em[GatewayFrame].Transport.Bit.Type=TR_ACQ;
                NotAcqFrame=1;
                PCCOM_SendFrame(GatewayFrame,&SendDataNull,sizeof(SendDataNull)); //acquittement de la trame reçue
                NotAcqFrame=0;
#endif           
            }
        }
        dl_FrameInfoRecv[NumBerOfEqpDL].Check.Flags=0;
    }

    return bEnd;
}

uint8_t PCCOM_Cycle(uint8_t * pIdent)
{
#if ENABLE_REPEAT_FRAME == ENABLE
    static uint8_t CountSendedFrame=0;
#endif
    switch(AcqOfFrameTr)
    {
        case TR_ACQ_WAIT:
#if  ENABLE_REPEAT_FRAME == ENABLE
            PCTIME_TempoStart(&WaitAcqFrameTime, TIME_ACQ_FRAME);
            if(PCTIME_TempoIsElapsed(&WaitAcqFrameTime))
            {
                
                if(CountSendedFrame<NUMB_SEND_SAME_FRAME)
                {
                    CountSendedFrame++;
                    RepeatFrame=1;
                    COM_Em[dl_FrameInfoRep.NbOfEqp].Transport.Bit.Type=TR_DATA;
                    memcpy(&dl_FrameInfoEm,&dl_FrameInfoRep,sizeof(dl_FrameInfoRep));
                    PCCOM_SendFrame(dl_FrameInfoEm.NbOfEqp,dl_FrameInfoEm.data,dl_FrameInfoEm.sizeOfData);
                }
                else
                {
                    CountSendedFrame=0;
                    *pIdent=COM_Em[dl_FrameInfoRep.NbOfEqp].Network.Bit.Dest;
                     AcqOfFrameTr=TR_ACQ_ERROR;
                }
            }
#else
            AcqOfFrameTr=TR_ACQ_WAIT;
#endif
            break;
        case TR_ACQ_REC:
             *pIdent=SrceTrameAcq;
            AcqOfFrameTr=TR_ACQ_OK;
#if ENABLE_REPEAT_FRAME == ENABLE
            CountSendedFrame=0;
            WaitAcqFrameTime.Request=0;
#endif
            break;
    }

    return AcqOfFrameTr;
}

void PCCOM_FreeFrame(void)
{
   AcqOfFrameTr=TR_ACQ_FREE;
   StateOfFrameTr=TR_TYPE_FREE;
   FreeSending=0;
}
