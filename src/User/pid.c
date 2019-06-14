#include "pid.h"

static MPID sPID;
static MPID *sptr = &sPID;

void PID_ParamInit(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=M_KP;      //�������� Proportional Const
    sptr->Integral=M_KI;        //���ֳ���  Integral Const
    sptr->Derivative=M_KD;      //΢�ֳ��� Derivative Const
    sptr->SetPoint=100;           //�趨Ŀ��Desired Value
}

u32 Speed_Adjust(u32 NextPoint,u32 Set_Speed)
{
  int iError,iIncpid;                             
  sptr->SetPoint = Set_Speed;
  iError=sptr->SetPoint-NextPoint;                    //��������
  iIncpid=(sptr->Proportion * iError)                 //E[k]��
              -(sptr->Integral * sptr->LastError)     //E[k-1]��
              +(sptr->Derivative * sptr->PrevError);  //E[k-2]��
              
  sptr->PrevError=sptr->LastError;                    //�洢�������´μ���
  sptr->LastError=iError;

  return iIncpid;
}



float  SKP=0.0;//����������ϵ����Ӱ�����Ĵ�Ƿ�Χ
float  SKD=7.4;//10//7.5//�������΢��ϵ��,Ӱ�����Ĵ�Ƿ�Ӧ
float  SteerPwmAdd=0.0;//���pwm����
float  Error;//ƫ��ֵ
float  SLastError;//�ϴε�ƫ��
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
float  J=0.0290;//����p��ƫ��Ĺ�ϵ��Խ������Խǿ
float  BasicP=3.0; //������Pֵ
uint32 SteerPwm=0,LastSteerSwm=0;//�����pwmֵ���ϴζ����pwmֵ

//��Ȩƽ����Ȩֵ��ѡȡ
 #if 1
float Weight[60]={ 
                    0,0,0,0,0,0,0,0,0,0,     //0-9�У������ò���
                    0,0,0,0,0,0,2,2,2,2,  //0-19�У������ò���
                   
                   1.8,1.8,1.8,1.8,1.8,1.3,1.3,1.3,1.3,1.3,//20-29��

                   //1.05,1.05,1.05,1.05,1.05,2,2,2,2,2,
                    
                   2,2,2,2,2,2.0,2.0,2.5,2.5,2.5,//30-39��
            
                   2.1,2.1,2.1,2.1,2.1,2,1,2,1,2,//40-49��
                  1,0,0,0,0,};//���ʮ��*///��ѹ�ߣ��������
#endif

#if 0

float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,              //0-9�У������ò���
                    
                     0,0,0,0,0,1.5,1,1.5,1,             //0-19�У������ò���
                  
                     2.5,1,2.2,1,2.5,1,2,2,1,2,            //20-29��
                    
                     1,3,2,2,1,2,1,2,2,1,              //30-39��

                     3,1,2,3,3,2,3,1,1,             //40-49��
                   
                     1,1,1,1,1,2,1,1,0,0,};          //���ʮ��




#endif


void Calculate_Error(void)
{
  int i;
  CenterSum=0;
  CenterMeanValue=0;
  WeightSum=0;   
  for(i=57;i>LastLine;i--)
  {      
    CenterSum+=MiddleLine[i]*Weight[i];
    WeightSum+=Weight[i];       
  }
  if(WeightSum!=0)
  {
    CenterMeanValue=(CenterSum/WeightSum);//�����Ȩƽ�������ߵ�ֵ
  }
  SLastError=Error;
  Error=(45-CenterMeanValue);// һ��ͼ��ƫ��ֵ 
  if(Error>=30.0)//ƫ���޷�
  Error=30.0;
  if(Error<=-30.0)
    Error=-30.0; 
  SKP=BasicP+(Error* Error)*J;//��̬����pģ��
  if(SKP>=11) 
    SKP=11;//pֵ�޷�    
}
                                    
void Steer_Control(void)
{
  Calculate_Error(); 
  SteerPwmAdd=(SKP*Error)+SKD*(Error-SLastError);//�����pd������
  if(SteerPwmAdd>=120)
    SteerPwmAdd=120;
  if(SteerPwmAdd<=-120)
    SteerPwmAdd=-120;
  SteerPwm=(SteerPwmAdd+SteerMidle);
  if(SteerPwm>=Servo_left_max)//�޷�
    SteerPwm=Servo_left_max;
  if(SteerPwm<=Servo_right_max)
    SteerPwm=Servo_right_max;
  Turn(SteerPwm);//���pwm����
  LastSteerSwm=SteerPwm;//��¼pwmֵ
                    
}

void Show_SPID()
{
  char a[20]="";
  sprintf(a,"Middle:%f",CenterMeanValue);
  OLED_P6x8Str(0,1,a);
  sprintf(a,"Error:%f",Error);
  OLED_P6x8Str(0,2,a);
  sprintf(a,"PwmAdd:%f",SteerPwmAdd);
  OLED_P6x8Str(0,3,a);
  sprintf(a,"SteerPwm:%d",SteerPwm);
  OLED_P6x8Str(0,4,a);
}


