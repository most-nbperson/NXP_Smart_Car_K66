//�ж����ȼ�˵��
//PIT0������ ��0��1��

//�ⲿ�жϣ�0��0��

//����ͷ�жϣ�0��0��

//PIT1������

//PIT2�����ٶȣ�0��2��
//PIT3��������ͷʶ��·�ϣ�0��2��

#include "include.h" 


void main(void)
{
  int i;
  char t[20]="";
  PLL_Init(PLL230);  //����Ϊ220/5=44M 
  
  NVIC_SetPriorityGrouping(2);
  
  LED_Init();
  KEY_Init();
  KEY_Read();
  OLED_Init();
  OLED_CLS();
  Encoder_Init(30);
  Motor_Init();
  Servo_Init();
  Camera_Init();
  XBEE_Init();
  PID_ParamInit();
  
  PIT_Init(PIT3,25);
  NVIC_SetPriority(PIT3_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
  NVIC_EnableIRQ(PIT3_IRQn);
  
  PIT_Init(PIT2,5);
  NVIC_SetPriority(PIT2_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
  NVIC_EnableIRQ(PIT2_IRQn);

  Get_Connect_Main();
  
  while (1)
  { 
    
    if(Field_Over_Flag==1)
    {
      NVIC_DisableIRQ(PORTD_IRQn); //�ر�����ͷ�ж�
      Get_Finally_Image();
      //////////////////////////////////ʶ��///////////////////////////////////
      Get_Black_End();
      Search_Center_Line();
      
      Get_Cross();
      if(R_Loop_Out!=1&&L_Loop_Out!=1)
      {
        Get_Loop_In();
        Loop_Control();
      }
      Get_Loop_Out();
//      Get_Loop_In();
      
      
      
      sprintf(t,"%d",Loop_Count_Start);
      OLED_P6x8Str(0,1,t);
      
      sprintf(t,"%d",Loop_Test);
      OLED_P6x8Str(40,1,t);
      
      sprintf(t,"%d",R_Loop);
      OLED_P6x8Str(80,1,t);
      //////////////////////////////////����////////////////////////////////////
      Calculate_Error();
      Steer_Control();
//      Show_SPID();
      Draw_Road();
      Field_Over_Flag=0;
      NVIC_EnableIRQ(PORTD_IRQn);//������ͷ�ж�
    }
    
    
  }
}



void PIT2_IRQHandler()  //���ڵ����ٶ�
{
  PIT_Flag_Clear(PIT2); 
  
  if(ready)
    control_speed(80);
  else 
    control_speed(0);
    
    if(break_flag)
    {
      //���
    }
  
}

void PIT3_IRQHandler()  // 25ms ����ʶ���ϰ��Ͷ�·
{
  PIT_Flag_Clear(PIT3);       //���жϱ�־λ
//  Break_Send(); //����ʹ��
//  Get_Break();
  ///////////////////////////////////��������///////////////////////////////////
  if(R_Loop_Out)//��⵽����
  {
    Loop_Out_Delay++;
    if(Loop_Out_Delay>2) //�����ӳ�
    {
      R_Loop=0;
    }
    
    if(Loop_Out_Delay>500) //��ȫ����
    {
      R_Loop_Out=0;
      Loop_Out_Delay=0;
    }
  }

  ////////////////////////////////////����뻷//////////////////////////////////
  if(Loop_Count_Start)
  {
    Loop_Delay++;
    if(Loop_Delay>25)
    {
      Loop_Test=1;
    }
    
    if(Loop_Delay>100)
    {
      Loop_Delay=0;
        =0;
      Loop_Test=0;
    }
  }
  

}

