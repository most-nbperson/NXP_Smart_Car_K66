#ifndef __CONTROL_H__
#define __CONTROL_H__
 
#define FRONT 0
#define BACK 1
#define STOP 2

#define SteerMidle 460  //6200  //6380 //�������ֵ   5000  7400   5350   5960 4740
#define Steer_DIFF 90  //1200  //1410                    //�����������ֵ�Ĳ�ֵ 

#define Servo_left_max  SteerMidle+Steer_DIFF   //����������ֵ
#define Servo_right_max SteerMidle-Steer_DIFF   //����������ֵ

#define SteerMax Servo_left_max-SteerMidle
#define SteerMin Servo_right_max-SteerMidle


extern void Motor_Init();
extern void Servo_Init();

extern void Motor_Left(int dir,int speed);
extern void Motor_Right(int dir,int speed);
extern void Go_Front(int L_speed,int R_speed);
extern void Go_Back(int L_speed,int R_speed);
extern void Turn(uint32 duty);

#endif