#include "include.h"

int MiddleLine[60+1];
int RightEdge[60+1];
int LeftEdge[60+1];
int Width[60+1]={2,3,3,3,4,4,5,5,6,6,
                       8,8,10,10,12,13,14,14,16,17,
                       18,18,20,20,22,22,24,24,26,26,
                       28,30,31,32,32,34,36,36,38,39,
                       41,41,43,43,45,45,47,47,49,50,
                       50,51,52,54,55,56,57,58,59,60,61};;

int   MidPri         = 40;
int   LastLine       = 0;
float AvaliableLines = 0;
int   LeftLose       = 0;
int   RightLose      = 0; 
int   AllLose        = 0;
int   LeftLoseStart  = 0;//��¼��߶��ߵĿ�ʼ��
int   RightLoseStart = 0;//��¼�ұ߱߶��ߵĿ�ʼ��
int   WhiteStart     = 0;                       
                       

int BlackEndMR      = 0;
int BlackEndML      = 0;
int BlackEndLL      = 0;
int BlackEndRR      = 0;
int BlackEndL       = 0;
int BlackEndM       = 0;
int BlackEndR       = 0;
int BlackEndMaxMax  = 0;
int BlackEndMax     = 0;
int DropRow         = 0;


unsigned char  CrossFlag  = 0;//ʮ�ֱ�־

unsigned char Loop_Count_Start=0;//�뻷��ʱ��־
unsigned char Loop_Test=0;//��ڲ��Ա�־
int sumR=0;
int sumL=0;

int Loop_Delay=0;

unsigned char L_Loop_Out=0;//���ڱ�־
unsigned char R_Loop_Out=0;

int Loop_Out_Delay=0;
int Loop_In_Delay=0;

unsigned char R_Loop=0;//������־
unsigned char L_Loop=0;

unsigned char break_flag=0;
                       
void Get_Black_End()//��ȡ���߽�ֹ��
{
  
  unsigned char LEndFlag  = 0;//��߽���
  unsigned char MEndFlag  = 0;//�м����
  unsigned char REndFlag  = 0;//�ұ߽���
  unsigned char MREndFlag = 0;//�м�ƫ�ҽ���
  unsigned char MLEndFlag = 0;//�м�ƫ�����
  unsigned char LLEndFlag = 0;//���ƫ�����
  unsigned char RREndFlag = 0;//�ұ�ƫ�ҽ���

  int i=0;

  BlackEndMR   = 0;     //����ƫ�Һ��߽�����־
  BlackEndML   = 0;     //����ƫ����߽�����־
  BlackEndLL   = 0;     //���ƫ����߽�����־
  BlackEndRR   = 0;     //�ұ�ƫ�Һ��߽�����־
  BlackEndL    = 0;     //��ߺ��߽�����־
  BlackEndM    = 0;     //�м���߽�����־
  BlackEndR    = 0;     //�ұߺ��߽�����־
  
  for (i = 60-1; i >= 3 ; i--)          
  {
    ///////////////////////////////��������////////////////////////////////////                      //  |||||||||||||||||||||||||||||||||||||||||||||||
    if(Pixel[i][40] == 1 && !MEndFlag )                                                                //  |||||||||||||||||||||||||||||||||||||||||||||||
    {                                                                                                //  |||||LL|||L||||ML|||M||||MR|||R||||RR||||||||||
      BlackEndM++;//�к��߽�����                                                                     //  ||||10|||20|||30|||40|||50|||60|||70|||80||||||
    }
    else if(i > 1 && Pixel[i-1][40] == 0 && Pixel[i-2][40] == 0)//���������Ǻ�ɫ        
    {
      MEndFlag = 1;    
    }
    ///////////////////////////////���������////////////////////////////////////
    if(Pixel[i][20] == 1 && !LEndFlag )
    {
      BlackEndL++;//����߽�����
    }
    else if(i > 1 && Pixel[i-1][20] == 0 && Pixel[i-2][20] == 0)
    {
      LEndFlag = 1;
    }
    //////////////////////////////�����ұ���////////////////////////////////////
    if(Pixel[i][60] == 1 && !REndFlag )
    {
      BlackEndR++;//�Һ��߽�����
    }
    else if(i > 1 && Pixel[i-1][60] == 0 && Pixel[i-2][60] == 0)
    {
      REndFlag = 1;
    }
    ////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////�����м�ƫ��/////////////////////////////////
    if(Pixel[i][30] == 1 && !MLEndFlag )
    {
      BlackEndML++;
    }
    else if(i > 1 && Pixel[i-1][30] == 0 && Pixel[i-2][30] == 0)
    {
      MLEndFlag = 1;
    }
    ///////////////////////////////�����м�ƫ��/////////////////////////////////
    if(Pixel[i][50] == 1 && !MREndFlag )
    {
      BlackEndMR++;
    }
    else if(i > 1 && Pixel[i-1][50] == 0 && Pixel[i-2][50] == 0)
    {
      MREndFlag = 1;
    }
    ////////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////�������ƫ��//////////////////////////////////
    if(Pixel[i][10] == 1 && !LLEndFlag )
    {
      BlackEndLL++;
    }
    else if(i > 1 && Pixel[i-1][10] == 0 && Pixel[i-2][10] == 0)
    {
      LLEndFlag = 1;
    }
    //////////////////////////////�����ұ�ƫ��//////////////////////////////////
    if(Pixel[i][70] == 1 && !RREndFlag )
    {
      BlackEndRR++;
    }
    else if(i > 1 && Pixel[i-1][70] == 0 && Pixel[i-2][70] == 0)
    {
      RREndFlag = 1;
    }
  }
        
    
  
  if(BlackEndL>BlackEndM)
     BlackEndMax=BlackEndL;
  else
    BlackEndMax=BlackEndM;

  if(BlackEndMax>BlackEndR)
     BlackEndMax=BlackEndMax;
  else
    BlackEndMax=BlackEndR;

  if(BlackEndMax>BlackEndMR)
     BlackEndMax=BlackEndMax;
  else
    BlackEndMax=BlackEndMR;

  if(BlackEndMax>BlackEndML)
     BlackEndMax=BlackEndMax;
  else
    BlackEndMax=BlackEndML;

  if(BlackEndMax>BlackEndLL)
     BlackEndMax=BlackEndMax;
  else
    BlackEndMax=BlackEndLL;

  if(BlackEndMax>BlackEndRR)
     BlackEndMax=BlackEndMax;
  else
    BlackEndMax=BlackEndRR;
  
  if(BlackEndMaxMax>=60)
  {
    BlackEndMaxMax=58;
  }
  DropRow=60-BlackEndMaxMax;//�ⶥ������      
}
 

void Set_Init_Val() //��ʼ���������
{
  int i;
  for(i=0;i<60;i++)          
  { 
    RightEdge[i]  = 80;
    LeftEdge[i]   = 0;
    MiddleLine[i] = 80/2;   
  }
  
} 


void Search_Center_Line(void) //Ѱ������
{

  int16 i         = 0;
  int16 j         = 0;
  uint8 jj        = 0;
  uint8 WhiteNum  = 0; 
  
  LeftLose        = 0;//��������
  RightLose       = 0;
  AllLose         = 0;
  WhiteNum        = 0;

  MiddleLine[60]  = 80/2;//��60��
  LeftEdge[60]    = 0;
  RightEdge[60]   = 80;
  Width[60]       = 80;
  
  Set_Init_Val();
  /********************************ǰʮ��ȫ��ɨ��******************************/
  for(i=60-1;i>=50;i--)
  {
    ////////////////////////////Ѱ����߽�/////////////////////////////////////
    if(i ==60-1)//���о���ͼ��������Ϊɨ�����
    {
       j = MidPri;
    } 
    else
    {
      j = MiddleLine[i+1];//���������һ���е��λ����Ϊ����ɨ�����
    }
    
    if(j <= 2)
    {
        j = 2;
    } 
    
    while(j >= 3)//j>=3��Ч��Χ�ڽ�����Ѱ 
    {
      if(Pixel[i][j-2]==0&& Pixel[i][j-1]==0 && Pixel[i][j]==1)  //��->�� �ںڰ�
      {
        LeftEdge[i] =j;//�ҵ���߽�    
        break;//��������Ѱ��
      }
             j--;//���������ƶ�
    }
    //////////////////////////////Ѱ���ұ߽�///////////////////////////////////
    if(i==60-1) //�����ұ߽�
    {
      j = MidPri;//������У���ͼ�����Ŀ�ʼ��Ѱ
    }          
    else
    {
      j = MiddleLine[i+1];//�������һ������λ�ÿ�ʼ��Ѱ
    }
    if(j >=80-3)//j >=ColumnMax-2��Ч��Χ����Ѱ����
    {
      j = 80-3;
    }
    while(j <= 80-3)
    {  
      if(Pixel[i][j]==1 && Pixel[i][j+1]==0 && Pixel[i][j+2]==0)//��->�� �׺ں�
      {
        RightEdge[i] = j;//�ҵ��ұ߽�
        break;//��������Ѱ��
      }
               j++;//���������ƶ�
    }
    ////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////�ж�����/////////////////////////////////////
    if(LeftEdge[i]!=0 && RightEdge[i]!=80)
    {
      MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;  //����λ��
    }
    else if(LeftEdge[i]!=0 && RightEdge[i]==80)//��������
    {
      RightLose++;

      if( (RightEdge[i]-LeftEdge[i]) >=(RightEdge[i+1]-LeftEdge[i+1]+1))//ͻ��
      {
        MiddleLine[i] = MiddleLine[i+1];//����һ�е��е�    
      }
      else 
      {
        MiddleLine[i] = LeftEdge[i] + Width[i]/2;//�����Ļ����ð��
      }
    }
    else if(LeftEdge[i]==0 && RightEdge[i]!=80)//��������
    {     
      LeftLose++;
      if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1))//ͻ��      
      {
        MiddleLine[i] = MiddleLine[i+1];//����һ��   
      } 
      else 
      {
        MiddleLine[i] = RightEdge[i] - Width[i]/2;//�߿�
      }
    }
    else if(LeftEdge[i]==0 && RightEdge[i]==80)//���߶����˵Ļ�  
    {
      AllLose++;
      if(i ==60-1)//��������о���ͼ��������Ϊ�е�
      {
        MiddleLine[i] = MidPri;
      }       
      else 
      {
        MiddleLine[i] = MiddleLine[i+1];//����������о�����һ�е�������Ϊ�����е�
      }             
    }
    if(MiddleLine[60-1] >=70)
    {
      MidPri = 70;
    }          
    else if(MiddleLine[60-1] <= 10)
    {
      MidPri = 10;
    }         
    else
    {
      MidPri = MiddleLine[60-1];//��¼��֡ͼ���59�е�����ֵ����Ϊ��һ��ͼ���59��ɨ����ʼ��
    }
  }   
  /***************************************************************************/
  
  /*****************************��50��ɨ��*************************************/
  for(i=49; i>0; i--)
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=80) //��һ�����߶��ҵ� ���ñ���ɨ��     
    {
      //////////////////////////////������߽�//////////////////////////////////////
      j = ((LeftEdge[i+1]+10) >= 80-2)? 80-2:(LeftEdge[i+1]+10);//������߽�    
      jj = ((LeftEdge[i+1]-5) <= 1)? 1:(LeftEdge[i+1]-5);        
      while(j >= jj)       
      {       
        if(Pixel[i][j]==1 && Pixel[i][j-1]==0)  
        {
          LeftEdge[i] = j;
          break;
        }   
        j--;   
      }
      ///////////////////////////////�����ұ߽�/////////////////////////////////////
      j = ((RightEdge[i+1]-10) <= 1)? 1:(RightEdge[i+1]-10); //�����ұ߽�   
      jj = ((RightEdge[i+1]+5) >= 80-2)? 80-2:(RightEdge[i+1]+5);    
      while(j <= jj)             
      {
        if(Pixel[i][j]==1&& Pixel[i][j+1]==0) 
        {
          RightEdge[i] = j;
          break;    
        }
        j++;
      }
    }
    //////////////////////////////////////////////////////////////////////////
    
    /////////////////////////////��һ�ж��ұ߽�///////////////////////////////
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==80) 
    {      
      j  = ((LeftEdge[i+1]+10) >=80-2)? 80-2:(LeftEdge[i+1]+10);//��߽��ñ���ɨ��   
      jj = ((LeftEdge[i+1]-5) <= 1)? 1:(LeftEdge[i+1]-5);              
      while(j >= jj)   
      {     
        if(Pixel[i][j]==1 && Pixel[i][j-1]==0)
        {
          LeftEdge[i] = j;
          break;
        }
        j--;   
      }     
      j = MiddleLine[i+1];//��һ�ж����ұ߽���ȫ��ɨ�� 
      if(j >= 78)
      {
        j = 78;
      }
      while(j <= 80-2)      
      {    
        if(Pixel[i][j]==1 && Pixel[i][j+1]==0)
        {
          RightEdge[i] = j;
          break;
        }
        j++;
      }
           
    }
    /////////////////////////////��һ�ж���߽�////////////////////////////////////
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=80) //��һ��ֻ�ҵ��ұ߽�      
    {
      j = ((RightEdge[i+1]-10) <= 1)?1:(RightEdge[i+1]-10);//��Ե׷�����ұ߽� 
      jj = ((RightEdge[i+1]+5) >= 80-2)? 80-2:(RightEdge[i+1]+5);            
      while(j <= jj)  
      {    
        if(Pixel[i][j]==1&&Pixel[i][j+1]==0)
        {
          RightEdge[i] = j;
          break;
        }
        j++;     
      }   
      j = MiddleLine[i+1]; //ȫ��ɨ������߽�
      if(j < 2)
      {
        j = 2;
      }  
      while(j >= 1)  
      {  
        if(Pixel[i][j]==1 && Pixel[i][j-1]==0)       
        {
          LeftEdge[i] = j;
          break;
        }
        j--;      
      }           
    } 
    //////////////////////////////////////////////////////////////////////////
    
    /////////////////////////////////û�б߽�//////////////////////////////////
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==80)  //��һ��û�ҵ��߽磬������ʮ�ֻ��߻���  
    {
      j = MiddleLine[i+1];   //��ȫ������߽�
      while(j >= 1)  
      {
        if(Pixel[i][j]==1 && Pixel[i][j-1]==0)     
        {
          LeftEdge[i] = j;
          break;
        }
        j--;
      }
      j = MiddleLine[i+1];   //ȫ�����ұ߽�   
      while(j <=80-2)       
      {   
        if(Pixel[i][j]==1&&Pixel[i][j+1]==0)
        {
          RightEdge[i] = j;  
          break;
        }
        j++;
      }         
    }
    ///////////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////����ͼ��������ұ߽�/////////////////////////////////////
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1) )//��������� 
    {      
      MiddleLine[i] = MiddleLine[i+1];//����һ��
    }
    else
    {
      //////////////////////////��û����////////////////////////////
      if(LeftEdge[i]!=0 && RightEdge[i]!=80)   
      {
        MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
        //��б��ʮ�ֽ��о���
        if( MiddleLine[i]-MiddleLine[i+1]>8&&((ABS(LeftEdge[i]-LeftEdge[i+1]>3)||ABS(RightEdge[i]-RightEdge[i+1]>3)) )&& i>=30)//��������ͻ��
        {
          uint8 ii = i;
          while(1)
          {
            MiddleLine[ii+1] = MiddleLine[ii]-1; 
            ii++;               
            if( ii>=50 || (MiddleLine[ii]-MiddleLine[ii+1]<=1) )
            {
              break;
            }          
          }
        }
        if( (MiddleLine[i+1]-MiddleLine[i]>8)&&((ABS(LeftEdge[i]-LeftEdge[i+1]>3)||ABS(RightEdge[i]-RightEdge[i+1]>3)))&& i>=30)
        {
          uint8 ii = i;  
          while(1)
          {
            MiddleLine[ii+1] = MiddleLine[ii]+1;
            ii++;
            if( ii>=50 || (MiddleLine[ii+1]-MiddleLine[ii]<=1) )
            { 
              break;
            }       
          }
        }
      }
      ///////////////////////////////////////////////////////
      
      /////////////////////////�ұ߶���///////////////////////////
      else if(LeftEdge[i]!=0 && RightEdge[i]==80)
      { 
        RightLose++;
        if(LeftEdge[i+1] != 0)
        {
          MiddleLine[i] = MiddleLine[i+1]+LeftEdge[i]-LeftEdge[i+1];
        }        
        else
        {
          MiddleLine[i]  = LeftEdge[i] + Width[i]/2;
        }
      }
      //////////////////////////////////////////////////////
      
      ////////////////////////��߶���///////////////////////////
      else if(LeftEdge[i]==0 && RightEdge[i]!=80)
      {
        LeftLose++;                
        if(RightEdge[i+1] !=80)
        {
          MiddleLine[i] = MiddleLine[i+1]+RightEdge[i]-RightEdge[i+1];
        }
        else
        {
          MiddleLine[i] = RightEdge[i] - Width[i]/2;
        }
      }
      ///////////////////////////////////////////////////////
      
      //////////////////////���߶���/////////////////////////////////
      else if(LeftEdge[i]==0 && RightEdge[i]==80)  
      {
        WhiteNum++;
        AllLose++;
        if(WhiteNum == 1)
        {   
          WhiteStart = i;
        }         
        MiddleLine[i] = MiddleLine[i+1];
      }
      //////////////////////////////////////////////////////////
    } 
    ////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////////////
    if(i == 0)             
    {            
      AvaliableLines = 60;
      LastLine  = 0;  
      break;
    }
    uint16 m = MiddleLine[i];  //�����������и���m�����ڲ���ÿ�е���һ�е����
    if(m < 5)
    { 
      m = 5;
    }
    if(m > 75)
    {
      m = 75;
    }
    if( (LeftEdge[i]!=0 && LeftEdge[i]>=65) || (RightEdge[i]!=80 && RightEdge[i]<15) || (i>=1)&&(Pixel[i-1][m]== 0)) //���һ��              
    {
      LastLine = i;//���һ�У���̬ǰհ
      AvaliableLines = 60 - i;//��Ч����
      break;
    }           
  }
  /***********************************************************************************/
}

void Get_Loop_In()
{
  char a[50]="";
  int i,j;

  int sum=0;
  for(i=25;i<35;i++)
  {
    for(j=0;j<80;j++)
    {
      sum+=Pixel[i][j];
    }
  }
  sprintf(a,"%03d",sum);
  OLED_P6x8Str(0,0,a);
  if(sum>630)
    Loop_Count_Start=1;
  
  if(Loop_Test)
  {
    sum=0;
    for(i=25;i<35;i++)
    {
      for(j=0;j<80;j++)
      {
        sum+=Pixel[i][j];
      }
    }
    if(sum>630)
      R_Loop=1;
  }
}

void Loop_Control()
{
  int i;
  
  if(R_Loop&&L_Loop)
  {
    L_Loop=0;
    R_Loop=0;
  }
  
  if(R_Loop)
  {
    for(i=58;i>15;i--)
    {
      MiddleLine[i]=RightEdge[i]-Width[i]/2;
    }
  }
  if(L_Loop)
  {
    for(i=58;i>15;i--)
    {
      MiddleLine[i]=LeftEdge[i]+Width[i]/2;
    }
  }
}
          
void Get_Loop_Out()
{
  int i;
  if(AllLose>=10)
  {
    if(R_Loop)
    {
      R_Loop_Out=1;
      for(i=58;i>15;i--)
      {
        MiddleLine[i]=RightEdge[i]-Width[i]/2;
      }
    }
    if(L_Loop)
    {
      L_Loop_Out=1;
      for(i=58;i>15;i--)
      {
        MiddleLine[i]=RightEdge[i]+Width[i]/2;
      }
    }
  }
}




void Get_Cross(void)
{       
  unsigned char i;
  CrossFlag=0;//����
  if((AllLose>=10))//����ͬʱ���� 
  {
    CrossFlag=1;//���ʮ��  
  }
  if(CrossFlag)     
  {   
    if((BlackEndM>=BlackEndL)&&(BlackEndM>=BlackEndR))//�м������� 
    {
      for(i=58;i>20;i--)     
      {
        MiddleLine[i]=40;//ֱ��    
      }   
    }            
    else if((BlackEndL>BlackEndM)&&(BlackEndL>BlackEndR))//������߰׵���࣬����ƫ���ˣ�������  
    {
      for(i=58;i>20;i--)    
      {
        MiddleLine[i]=30; 
      }  
    }          
    else if((BlackEndR>BlackEndM)&&(BlackEndR>BlackEndL))//�����ұ߰׵���࣬����ƫ���ˣ�������
    {
      for(i=58;i>20;i--)     
      {   
        MiddleLine[i]=50;    
      }  
    }
  }
}

void Get_Break()
{
  char t[20];
  int sum=0;
  int i,j;
  for(i=20;i<30;i++)
  {
    for(j=35;j>45;j++)
    {
      sum+=Pixel[i][j];
    }
  }
  if(sum==0)
  {
    break_flag=1;
  }
  sprintf(t,"%d",sum);
  OLED_P6x8Str(0,0,t);
}