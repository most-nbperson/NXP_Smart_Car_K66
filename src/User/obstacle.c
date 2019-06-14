#include "include.h"

float Databuff[2];//存放上一次与这次的距离
char obstacle_flag=0;//路障标志位

void Find_Obstacle()
{
  Databuff[1]=Databuff[0];
  Databuff[0]=Get_Distance();
  if(Databuff[1]<=50&&Databuff[0]<=50&&Databuff[1]>=30&&Databuff[0]>=30)    //如果上一次和这一次距离都小于70cm的时候
  { 
    obstacle_flag=1;//输出路障标志位
  }
  else
  {
    obstacle_flag=0;
  }

  if(Pixel[50][10] == 1 && Pixel[50][30] == 1 &&Pixel[50][50] == 1 &&Pixel[50][70] == 1)//如果无效行大于50 说明 拐出界了
  {
    Out_line_flag=1;//输出标志位
  }
  else
  {
    Out_line_flag=0;
  }

}