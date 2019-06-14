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
  LPTMR_delayus(20);//��ʱ(us)
  //����һ��20us�ĸߵ�ƽ����
  GPIO_PinReverse(TRIG);
  PIT_TimeStart(PIT1);
  while(GPIO_PinRead(ECHO) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
  {
     timevar_1 = PIT_TimeGet(PIT1);
  }
  
  while(GPIO_PinRead(ECHO) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
  {       
  };

  //ֹͣ��ʱ����ȡ��ʱʱ��
  timevar_2 = (timevar_1 * (34/1000))/2;
  if(timevar_2>=100)
    timevar_2=100;
  return  timevar_2;
     
} 
