#include "include.h"  

float timevar_1;
int timevar_2;

void HC_SR04_Init()
{
  GPIO_PinInit(TRIG,GPO,0);
  GPIO_PinInit(ECHO,GPI,0);
  PIT_TimeStart(PIT1);
}

int Get_Distance()
{
  GPIO_PinWrite(TRIG,0);
  LPTMR_delayus(20);//延时(us)
  //产生一个20us的高电平脉冲
  GPIO_PinReverse(TRIG);
  PIT_TimeStart(PIT1);
  while(GPIO_PinRead(ECHO) == 0);             //等待电平变高，低电平一直等待
  {
     timevar_1 = PIT_TimeGet(PIT1);
  }
  
  while(GPIO_PinRead(ECHO) == 1)              //等待电平变低，高电平一直等待
  {       
  };

  //停止计时，获取计时时间
  timevar_2 = (timevar_1 * (34/1000))/2;
  if(timevar_2>=100)
    timevar_2=100;
  return  timevar_2;
     
} 
