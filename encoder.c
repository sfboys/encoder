#include "encoder.h"
#include "ftm.h"

#define AMASKENCODER FTM_CHANNEL_CHANNEL2
#define BMASKENCODER FTM_CHANNEL_CHANNEL3
#define AMASKPORT PTB
#define BMASKPORT PTB
#define AMASKPIN PTB3
#define BMASKPIN PTB4

EncoderStruct g_encodeStruct={ FTM_INPUTCAPTURE_FALLINGEDGE,FTM_INPUTCAPTURE_FALLINGEDGE,128,0};


void HalEncoderInit(void)
{
   FTM_InputCaptureInit(FTM2,AMASKENCODER,FTM_INPUTCAPTURE_FALLINGEDGE); 
   FTM_InputCaptureInit(FTM2,BMASKENCODER,FTM_INPUTCAPTURE_FALLINGEDGE); 
}

void FTM_InputCaptureExchange(FTM_Type *pFTM, uint8_t u8FTM_Channel, uint8_t u8CaptureMode)
{
     pFTM->CONTROLS[u8FTM_Channel].CnSC &=0xC0 ;
    if(FTM_INPUTCAPTURE_RISINGEDGE == u8CaptureMode)        /* enable interrupt, Capture on rising edge */
    {
        pFTM->CONTROLS[u8FTM_Channel].CnSC = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSA_MASK;
    }
    else if(FTM_INPUTCAPTURE_FALLINGEDGE == u8CaptureMode)  /* Capture on falling edge */
    {
        pFTM->CONTROLS[u8FTM_Channel].CnSC = FTM_CnSC_CHIE_MASK | FTM_CnSC_ELSB_MASK;
    }
}

void FTM2irqTask(void)
{
    if(FTM_GetChannelFlag(FTM2,AMASKENCODER))
    {
        FTM_ClrChannelFlag(FTM2,AMASKENCODER);
        if((uint8_t)FTM_INPUTCAPTURE_RISINGEDGE == g_encodeStruct.aMaskInMode)
        {
            if(0 != READ_INPUT(BMASKPORT,BMASKPIN))
            {
                g_encodeStruct.countNum -- ;
            }else
            {
                g_encodeStruct.countNum ++ ;
            }
            FTM_InputCaptureExchange(FTM2,AMASKENCODER,FTM_INPUTCAPTURE_FALLINGEDGE);
            g_encodeStruct.aMaskInMode = FTM_INPUTCAPTURE_FALLINGEDGE;
            
        }else if((uint8_t)FTM_INPUTCAPTURE_FALLINGEDGE == g_encodeStruct.aMaskInMode )
        {
            if(0 != READ_INPUT(BMASKPORT,BMASKPIN))
            {
                g_encodeStruct.countNum ++ ;
            }else
            {
                g_encodeStruct.countNum -- ;
            }
            FTM_InputCaptureExchange(FTM2,AMASKENCODER,FTM_INPUTCAPTURE_RISINGEDGE);
            g_encodeStruct.aMaskInMode = FTM_INPUTCAPTURE_RISINGEDGE;
        }
        g_encodeStruct.errCount ++ ;
    }

    if(FTM_GetChannelFlag(FTM2,BMASKENCODER))
    {
         FTM_ClrChannelFlag(FTM2,BMASKENCODER);
        if((uint8_t)FTM_INPUTCAPTURE_RISINGEDGE == g_encodeStruct.bMaskInMode)
        {
            if(0 != READ_INPUT(AMASKPORT,AMASKPIN))
            {
                g_encodeStruct.countNum ++ ;
            }else
            {
                g_encodeStruct.countNum -- ;
            }
            FTM_InputCaptureExchange(FTM2,BMASKENCODER,FTM_INPUTCAPTURE_FALLINGEDGE);
            g_encodeStruct.bMaskInMode = FTM_INPUTCAPTURE_FALLINGEDGE;
        }else if((uint8_t)FTM_INPUTCAPTURE_FALLINGEDGE == g_encodeStruct.bMaskInMode )
        {
            if(0 != READ_INPUT(AMASKPORT,AMASKPIN))
            {
                g_encodeStruct.countNum -- ;
            }else
            {
                g_encodeStruct.countNum ++ ;
            }
            FTM_InputCaptureExchange(FTM2,BMASKENCODER,FTM_INPUTCAPTURE_RISINGEDGE);
            g_encodeStruct.bMaskInMode = FTM_INPUTCAPTURE_RISINGEDGE;
        }
    }
}

void GetEncodeCount(uint8_t *encoderCount,uint8_t *errCount)
{
    *encoderCount = g_encodeStruct.countNum;
    *errCount = g_encodeStruct.errCount;
}

void SetEncodeCount(void)
{
    g_encodeStruct.countNum = 128;
    g_encodeStruct.errCount = 0;
}
