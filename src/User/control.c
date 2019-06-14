#include "include.h"
#include "control.h"

//电机初始化 频率为20khz
// 总线(44M)/（2^3）/250=22K
void Motor_Init()
{
  //左
  FTM_PwmInit(FTM0, FTM_CH0, 12000, 0); //c1
  FTM_PwmInit(FTM0, FTM_CH1, 12000, 0); //c2
  //右
  FTM_PwmInit(FTM0, FTM_CH2, 12000, 0); //c3
  FTM_PwmInit(FTM0, FTM_CH3, 12000, 0); //c4
}

void Servo_Init()
{
  FTM_PwmInit(FTM3, FTM_CH7,50 , SteerMidle);

}

void Motor_Right(int dir,int speed)
{
  if(dir==FRONT)
  {
    FTM_PwmDuty(FTM0, FTM_CH0,0);
    FTM_PwmDuty(FTM0, FTM_CH1,speed);
  }
  else if(dir==BACK)
  {
    FTM_PwmDuty(FTM0, FTM_CH0,speed);
    FTM_PwmDuty(FTM0, FTM_CH1,0);
  }
  else if(dir==STOP)
  {
    FTM_PwmDuty(FTM0, FTM_CH0,0);
    FTM_PwmDuty(FTM0, FTM_CH1,0);
  }
}

void Motor_Left(int dir,int speed)
{
  if(dir==FRONT)
  {
    FTM_PwmDuty(FTM0, FTM_CH2,speed);
    FTM_PwmDuty(FTM0, FTM_CH3,0);
  }
  else if(dir==BACK)
  {
    FTM_PwmDuty(FTM0, FTM_CH2,0);
    FTM_PwmDuty(FTM0, FTM_CH3,speed);
  }
  else if(dir==STOP)
  {
    FTM_PwmDuty(FTM0, FTM_CH2,0);
    FTM_PwmDuty(FTM0, FTM_CH3,0);
  }
}

void Go_Front(int L_speed,int R_speed)
{
  Motor_Left(FRONT,L_speed);
  Motor_Right(FRONT,R_speed);
}

void Go_Back(int L_speed,int R_speed)
{
  Motor_Left(BACK,L_speed);
  Motor_Right(BACK,R_speed);
}

void Turn(uint32 duty)
{    
  FTM_PwmDuty(FTM3,FTM_CH7,duty);
}