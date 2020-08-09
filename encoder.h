#ifndef __ENCODER_H_
#define __ENCODER_H_
/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

typedef struct
{
    uint8_t aMaskInMode;
    uint8_t bMaskInMode;
    uint8_t countNum;
    uint8_t errCount;   
}EncoderStruct;

void HalEncoderInit(void);
void FTM2irqTask(void);
void GetEncodeCount(uint8_t *encoderCount,uint8_t *errCount);
void SetEncodeCount(void);

#endif 

