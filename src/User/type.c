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
int   LeftLoseStart  = 0;//记录左边丢线的开始行
int   RightLoseStart = 0;//记录右边边丢线的开始行
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


unsigned char  CrossFlag  = 0;//十字标志

unsigned char Loop_Count_Start=0;//入环延时标志
unsigned char Loop_Test=0;//入口测试标志
int sumR=0;
int sumL=0;

int Loop_Delay=0;

unsigned char L_Loop_Out=0;//出口标志
unsigned char R_Loop_Out=0;

int Loop_Out_Delay=0;
int Loop_In_Delay=0;

unsigned char R_Loop=0;//环岛标志
unsigned char L_Loop=0;

unsigned char break_flag=0;
                       
void Get_Black_End()//获取黑线截止行
{
  
  unsigned char LEndFlag  = 0;//左边结束
  unsigned char MEndFlag  = 0;//中间结束
  unsigned char REndFlag  = 0;//右边结束
  unsigned char MREndFlag = 0;//中间偏右结束
  unsigned char MLEndFlag = 0;//中间偏左结束
  unsigned char LLEndFlag = 0;//左边偏左结束
  unsigned char RREndFlag = 0;//右边偏右结束

  int i=0;

  BlackEndMR   = 0;     //中心偏右黑线结束标志
  BlackEndML   = 0;     //中心偏左黑线结束标志
  BlackEndLL   = 0;     //左边偏左黑线结束标志
  BlackEndRR   = 0;     //右边偏右黑线结束标志
  BlackEndL    = 0;     //左边黑线结束标志
  BlackEndM    = 0;     //中间黑线结束标志
  BlackEndR    = 0;     //右边黑线结束标志
  
  for (i = 60-1; i >= 3 ; i--)          
  {
    ///////////////////////////////查找中线////////////////////////////////////                      //  |||||||||||||||||||||||||||||||||||||||||||||||
    if(Pixel[i][40] == 1 && !MEndFlag )                                                                //  |||||||||||||||||||||||||||||||||||||||||||||||
    {                                                                                                //  |||||LL|||L||||ML|||M||||MR|||R||||RR||||||||||
      BlackEndM++;//中黑线截至行                                                                     //  ||||10|||20|||30|||40|||50|||60|||70|||80||||||
    }
    else if(i > 1 && Pixel[i-1][40] == 0 && Pixel[i-2][40] == 0)//连续两行是黑色        
    {
      MEndFlag = 1;    
    }
    ///////////////////////////////查找左边线////////////////////////////////////
    if(Pixel[i][20] == 1 && !LEndFlag )
    {
      BlackEndL++;//左黑线截至行
    }
    else if(i > 1 && Pixel[i-1][20] == 0 && Pixel[i-2][20] == 0)
    {
      LEndFlag = 1;
    }
    //////////////////////////////查找右边线////////////////////////////////////
    if(Pixel[i][60] == 1 && !REndFlag )
    {
      BlackEndR++;//右黑线截至行
    }
    else if(i > 1 && Pixel[i-1][60] == 0 && Pixel[i-2][60] == 0)
    {
      REndFlag = 1;
    }
    ////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////查找中间偏左/////////////////////////////////
    if(Pixel[i][30] == 1 && !MLEndFlag )
    {
      BlackEndML++;
    }
    else if(i > 1 && Pixel[i-1][30] == 0 && Pixel[i-2][30] == 0)
    {
      MLEndFlag = 1;
    }
    ///////////////////////////////查找中间偏右/////////////////////////////////
    if(Pixel[i][50] == 1 && !MREndFlag )
    {
      BlackEndMR++;
    }
    else if(i > 1 && Pixel[i-1][50] == 0 && Pixel[i-2][50] == 0)
    {
      MREndFlag = 1;
    }
    ////////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////查找左边偏左//////////////////////////////////
    if(Pixel[i][10] == 1 && !LLEndFlag )
    {
      BlackEndLL++;
    }
    else if(i > 1 && Pixel[i-1][10] == 0 && Pixel[i-2][10] == 0)
    {
      LLEndFlag = 1;
    }
    //////////////////////////////查找右边偏右//////////////////////////////////
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
  DropRow=60-BlackEndMaxMax;//封顶的行数      
}
 

void Set_Init_Val() //初始化赛道宽度
{
  int i;
  for(i=0;i<60;i++)          
  { 
    RightEdge[i]  = 80;
    LeftEdge[i]   = 0;
    MiddleLine[i] = 80/2;   
  }
  
} 


void Search_Center_Line(void) //寻找中线
{

  int16 i         = 0;
  int16 j         = 0;
  uint8 jj        = 0;
  uint8 WhiteNum  = 0; 
  
  LeftLose        = 0;//变量清零
  RightLose       = 0;
  AllLose         = 0;
  WhiteNum        = 0;

  MiddleLine[60]  = 80/2;//第60行
  LeftEdge[60]    = 0;
  RightEdge[60]   = 80;
  Width[60]       = 80;
  
  Set_Init_Val();
  /********************************前十行全行扫描******************************/
  for(i=60-1;i>=50;i--)
  {
    ////////////////////////////寻找左边界/////////////////////////////////////
    if(i ==60-1)//首行就以图像中心作为扫描起点
    {
       j = MidPri;
    } 
    else
    {
      j = MiddleLine[i+1];//否则就以上一行中点的位置作为本行扫描起点
    }
    
    if(j <= 2)
    {
        j = 2;
    } 
    
    while(j >= 3)//j>=3有效范围内进行搜寻 
    {
      if(Pixel[i][j-2]==0&& Pixel[i][j-1]==0 && Pixel[i][j]==1)  //左->右 黑黑白
      {
        LeftEdge[i] =j;//找到左边界    
        break;//跳出本行寻线
      }
             j--;//列数往左移动
    }
    //////////////////////////////寻找右边界///////////////////////////////////
    if(i==60-1) //再找右边界
    {
      j = MidPri;//如果首行，从图像中心开始搜寻
    }          
    else
    {
      j = MiddleLine[i+1];//否则从上一行中心位置开始搜寻
    }
    if(j >=80-3)//j >=ColumnMax-2有效范围内搜寻右线
    {
      j = 80-3;
    }
    while(j <= 80-3)
    {  
      if(Pixel[i][j]==1 && Pixel[i][j+1]==0 && Pixel[i][j+2]==0)//右->左 白黑黑
      {
        RightEdge[i] = j;//找到右边界
        break;//跳出本行寻线
      }
               j++;//列数往右移动
    }
    ////////////////////////////////////////////////////////////////////////////
    
    ///////////////////////////////判断中线/////////////////////////////////////
    if(LeftEdge[i]!=0 && RightEdge[i]!=80)
    {
      MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;  //中线位置
    }
    else if(LeftEdge[i]!=0 && RightEdge[i]==80)//丢了右线
    {
      RightLose++;

      if( (RightEdge[i]-LeftEdge[i]) >=(RightEdge[i+1]-LeftEdge[i+1]+1))//突变
      {
        MiddleLine[i] = MiddleLine[i+1];//用上一行的中点    
      }
      else 
      {
        MiddleLine[i] = LeftEdge[i] + Width[i]/2;//正常的话就用半宽补
      }
    }
    else if(LeftEdge[i]==0 && RightEdge[i]!=80)//丢了左线
    {     
      LeftLose++;
      if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1))//突变      
      {
        MiddleLine[i] = MiddleLine[i+1];//用上一行   
      } 
      else 
      {
        MiddleLine[i] = RightEdge[i] - Width[i]/2;//线宽
      }
    }
    else if(LeftEdge[i]==0 && RightEdge[i]==80)//两边都丢了的话  
    {
      AllLose++;
      if(i ==60-1)//如果是首行就以图像中心作为中点
      {
        MiddleLine[i] = MidPri;
      }       
      else 
      {
        MiddleLine[i] = MiddleLine[i+1];//如果不是首行就用上一行的中线作为本行中点
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
      MidPri = MiddleLine[60-1];//记录本帧图像第59行的中线值，作为下一幅图像的59行扫描起始点
    }
  }   
  /***************************************************************************/
  
  /*****************************后50行扫描*************************************/
  for(i=49; i>0; i--)
  {   
    if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=80) //上一行两边都找到 启用边沿扫描     
    {
      //////////////////////////////查找左边界//////////////////////////////////////
      j = ((LeftEdge[i+1]+10) >= 80-2)? 80-2:(LeftEdge[i+1]+10);//先找左边界    
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
      ///////////////////////////////查找右边界/////////////////////////////////////
      j = ((RightEdge[i+1]-10) <= 1)? 1:(RightEdge[i+1]-10); //在找右边界   
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
    
    /////////////////////////////上一行丢右边界///////////////////////////////
    else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==80) 
    {      
      j  = ((LeftEdge[i+1]+10) >=80-2)? 80-2:(LeftEdge[i+1]+10);//左边界用边沿扫描   
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
      j = MiddleLine[i+1];//上一行丢了右边界用全行扫描 
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
    /////////////////////////////上一行丢左边界////////////////////////////////////
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=80) //上一行只找到右边界      
    {
      j = ((RightEdge[i+1]-10) <= 1)?1:(RightEdge[i+1]-10);//边缘追踪找右边界 
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
      j = MiddleLine[i+1]; //全行扫描找左边界
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
    
    /////////////////////////////////没有边界//////////////////////////////////
    else if(LeftEdge[i+1]==0 && RightEdge[i+1]==80)  //上一行没找到边界，可能是十字或者环形  
    {
      j = MiddleLine[i+1];   //找全行找左边界
      while(j >= 1)  
      {
        if(Pixel[i][j]==1 && Pixel[i][j-1]==0)     
        {
          LeftEdge[i] = j;
          break;
        }
        j--;
      }
      j = MiddleLine[i+1];   //全行找右边界   
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
    
    ///////////////////////////////矫正图像畸变再找边界/////////////////////////////////////
    if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+1) )//不满足畸变 
    {      
      MiddleLine[i] = MiddleLine[i+1];//用上一行
    }
    else
    {
      //////////////////////////都没丢线////////////////////////////
      if(LeftEdge[i]!=0 && RightEdge[i]!=80)   
      {
        MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
        //对斜出十字进行纠正
        if( MiddleLine[i]-MiddleLine[i+1]>8&&((ABS(LeftEdge[i]-LeftEdge[i+1]>3)||ABS(RightEdge[i]-RightEdge[i+1]>3)) )&& i>=30)//中线向右突变
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
      
      /////////////////////////右边丢线///////////////////////////
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
      
      ////////////////////////左边丢线///////////////////////////
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
      
      //////////////////////两边丢线/////////////////////////////////
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
    uint16 m = MiddleLine[i];  //将中线所在列付给m，用于查找每行的上一行的情况
    if(m < 5)
    { 
      m = 5;
    }
    if(m > 75)
    {
      m = 75;
    }
    if( (LeftEdge[i]!=0 && LeftEdge[i]>=65) || (RightEdge[i]!=80 && RightEdge[i]<15) || (i>=1)&&(Pixel[i-1][m]== 0)) //最后一行              
    {
      LastLine = i;//最后一行，动态前瞻
      AvaliableLines = 60 - i;//有效行数
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
  CrossFlag=0;//清零
  if((AllLose>=10))//左右同时丢线 
  {
    CrossFlag=1;//标记十字  
  }
  if(CrossFlag)     
  {   
    if((BlackEndM>=BlackEndL)&&(BlackEndM>=BlackEndR))//中间白线最多 
    {
      for(i=58;i>20;i--)     
      {
        MiddleLine[i]=40;//直走    
      }   
    }            
    else if((BlackEndL>BlackEndM)&&(BlackEndL>BlackEndR))//看到左边白的最多，车体偏右了，往左跑  
    {
      for(i=58;i>20;i--)    
      {
        MiddleLine[i]=30; 
      }  
    }          
    else if((BlackEndR>BlackEndM)&&(BlackEndR>BlackEndL))//看到右边白的最多，车体偏左了，往右跑
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