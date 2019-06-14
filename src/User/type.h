#ifndef __TYPE_H__
#define __TYPE_H__

extern int   AllLose;
extern int BlackEndL;
extern int BlackEndM;
extern int BlackEndR;
extern  int   RightEdge[60+1];
extern  int   LeftEdge[60+1];
extern  int   MiddleLine[60+1];
extern int   LastLine;
extern int   Width[60+1];
extern int   LeftLose;
extern int   RightLose ;



//十字标志
extern unsigned char  CrossFlag ;
//入口标志
extern unsigned char Loop_Count_Start;//入环延时标志
extern unsigned char Loop_Test;//入口测试标志
extern int Loop_Delay;
//出口标志
extern unsigned char L_Loop_Out;
extern unsigned char R_Loop_Out;

extern int Loop_Out_Delay;
extern int Loop_In_Delay;
//环岛标志
extern unsigned char R_Loop;
extern unsigned char L_Loop;
//断路标志
extern unsigned char break_flag;


void Set_Init_Val();
extern void Search_Center_Line(void);
extern void Get_Black_End();
extern void Get_Loop_In();
extern void Get_Loop_Out();
extern void Get_Cross(void);
extern void Get_Break();
#endif