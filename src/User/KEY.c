#include "include.h"

uint8_t key_flag = 0;

void KEY_Init(void)
{
  GPIO_PinInit(PTB20, GPI_UP, 1); //k1
  GPIO_PinInit(PTB21, GPI_UP, 1); //k2
  GPIO_PinInit(PTB22, GPI_UP, 1); //k3
}

void KEY_Read(void)
{
  GPIO_ExtiInit(PTB20, falling_up);
  GPIO_ExtiInit(PTB21, falling_up);
  GPIO_ExtiInit(PTB22, falling_up);
 
  
  NVIC_SetPriority(PORTB_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));
  NVIC_EnableIRQ(PORTB_IRQn);
  
  
}

void PORTB_IRQHandler()
{
  
}
