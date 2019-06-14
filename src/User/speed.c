#include "include.h"

int Vspeed=0;
int more_speed=0;

int R_speed=0;
int L_speed=0;

int Para_R=0;
int Para_L=0;

int PWM_Duty_L=0;
int PWM_Duty_R=0;

int Set_R_Speed=0;//暂时在此改变速度
int Set_L_Speed=0;

void PIT0_IRQHandler()
{
  PIT_Flag_Clear(PIT0);       //清中断标志位
  
  if(GPIO_PinRead(PTA13)==1)   //顺时针旋转 
    L_speed=DMA_Count_Get(DMA_CH2);   
  else if(GPIO_PinRead(PTA13)==0)   //逆时针旋转                       
    L_speed=DMA_Count_Get(DMA_CH2);    
  DMA_Count_Reset(DMA_CH2); //清除计数

  if(GPIO_PinRead(PTB19)==1)   //顺时针旋转 
    R_speed=-DMA_Count_Get(DMA_CH3);   
  else if(GPIO_PinRead(PTB19)==0)   //逆时针旋转                       
    R_speed=DMA_Count_Get(DMA_CH3);    
  DMA_Count_Reset(DMA_CH3); //清除计数
  
  
  Para_R=Speed_Adjust(R_speed,Set_R_Speed);
  PWM_Duty_R +=Para_R;
  if(PWM_Duty_R<0)
    PWM_Duty_R=0;				
  if(PWM_Duty_R>10000)
    PWM_Duty_R=10000;
  
  Para_L=Speed_Adjust(L_speed,Set_L_Speed);
  PWM_Duty_L +=Para_L;
  if(PWM_Duty_L<0)
    PWM_Duty_L=0;				
  if(PWM_Duty_L>10000)
    PWM_Duty_L=10000;

}





void control_speed(int speed)
{
  Set_R_Speed=speed;//暂时在此改变速度
  Set_L_Speed=speed;
  Go_Front(PWM_Duty_L,PWM_Duty_R);
}
