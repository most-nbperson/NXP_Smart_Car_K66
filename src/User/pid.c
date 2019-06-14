#include "pid.h"

static MPID sPID;
static MPID *sptr = &sPID;

void PID_ParamInit(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=M_KP;      //比例常数 Proportional Const
    sptr->Integral=M_KI;        //积分常数  Integral Const
    sptr->Derivative=M_KD;      //微分常数 Derivative Const
    sptr->SetPoint=100;           //设定目标Desired Value
}

u32 Speed_Adjust(u32 NextPoint,u32 Set_Speed)
{
  int iError,iIncpid;                             
  sptr->SetPoint = Set_Speed;
  iError=sptr->SetPoint-NextPoint;                    //增量计算
  iIncpid=(sptr->Proportion * iError)                 //E[k]项
              -(sptr->Integral * sptr->LastError)     //E[k-1]项
              +(sptr->Derivative * sptr->PrevError);  //E[k-2]项
              
  sptr->PrevError=sptr->LastError;                    //存储误差，用于下次计算
  sptr->LastError=iError;

  return iIncpid;
}



float  SKP=0.0;//舵机方向比例系数，影响舵机的打角范围
float  SKD=7.4;//10//7.5//舵机方向微分系数,影响舵机的打角反应
float  SteerPwmAdd=0.0;//舵机pwm增量
float  Error;//偏差值
float  SLastError;//上次的偏差
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
float  J=0.0290;//调节p和偏差的关系，越大，作用越强
float  BasicP=3.0; //基本的P值
uint32 SteerPwm=0,LastSteerSwm=0;//舵机的pwm值和上次舵机的pwm值

//加权平均，权值的选取
 #if 1
float Weight[60]={ 
                    0,0,0,0,0,0,0,0,0,0,     //0-9行，基本用不到
                    0,0,0,0,0,0,2,2,2,2,  //0-19行，基本用不到
                   
                   1.8,1.8,1.8,1.8,1.8,1.3,1.3,1.3,1.3,1.3,//20-29行

                   //1.05,1.05,1.05,1.05,1.05,2,2,2,2,2,
                    
                   2,2,2,2,2,2.0,2.0,2.5,2.5,2.5,//30-39行
            
                   2.1,2.1,2.1,2.1,2.1,2,1,2,1,2,//40-49行
                  1,0,0,0,0,};//最近十行*///不压线，用于弯道
#endif

#if 0

float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,              //0-9行，基本用不到
                    
                     0,0,0,0,0,1.5,1,1.5,1,             //0-19行，基本用不到
                  
                     2.5,1,2.2,1,2.5,1,2,2,1,2,            //20-29行
                    
                     1,3,2,2,1,2,1,2,2,1,              //30-39行

                     3,1,2,3,3,2,3,1,1,             //40-49行
                   
                     1,1,1,1,1,2,1,1,0,0,};          //最近十行




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
    CenterMeanValue=(CenterSum/WeightSum);//算出加权平均后中线的值
  }
  SLastError=Error;
  Error=(45-CenterMeanValue);// 一场图像偏差值 
  if(Error>=30.0)//偏差限幅
  Error=30.0;
  if(Error<=-30.0)
    Error=-30.0; 
  SKP=BasicP+(Error* Error)*J;//动态二次p模型
  if(SKP>=11) 
    SKP=11;//p值限幅    
}
                                    
void Steer_Control(void)
{
  Calculate_Error(); 
  SteerPwmAdd=(SKP*Error)+SKD*(Error-SLastError);//舵机的pd控制器
  if(SteerPwmAdd>=120)
    SteerPwmAdd=120;
  if(SteerPwmAdd<=-120)
    SteerPwmAdd=-120;
  SteerPwm=(SteerPwmAdd+SteerMidle);
  if(SteerPwm>=Servo_left_max)//限幅
    SteerPwm=Servo_left_max;
  if(SteerPwm<=Servo_right_max)
    SteerPwm=Servo_right_max;
  Turn(SteerPwm);//舵机pwm更新
  LastSteerSwm=SteerPwm;//记录pwm值
                    
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


