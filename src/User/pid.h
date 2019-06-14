#ifndef __PID_H
#define __PID_H

#include "include.h"

typedef struct MPID
{
  int SetPoint; //设定目标 Desired Value
  long SumError; //误差累计
  double Proportion; //比例常数 Proportional Const
  double Integral; //积分常数 Integral Const
  double Derivative; //微分常数 Derivative Const
  int LastError; //Error[-1]
  int PrevError; //Error[-2]
}MPID;


#define M_KP 5.0
#define M_KI 1.5
#define M_KD 0.0

extern float  SLastError;
extern float  Error;
extern float  CenterMeanValue;
extern float  SteerPwmAdd;
extern uint32  SteerPwm;

extern void PID_ParamInit();
extern u32 Speed_Adjust(u32 Now_Speed,u32 Set_Speed);
extern void Calculate_Error(void);
extern void Steer_Control(void);
extern void Show_SPID(void);


#endif