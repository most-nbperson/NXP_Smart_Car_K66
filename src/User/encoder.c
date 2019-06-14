#include "include.h"
#include "encoder.h"

void Encoder_Init(int time) //time：计时时间
{
  PIT_Init(PIT0,time);
  
  NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,1));
  NVIC_EnableIRQ(PIT0_IRQn);
   
  DMA_Count_Init(DMA_CH2,PTA12,65535,DMA_rising_down);
  DMA_Count_Init(DMA_CH3,PTB18,65535,DMA_rising_down);
  
  GPIO_PinInit(PTA13,GPO,0);
  GPIO_PinInit(PTA19,GPO,0);
}