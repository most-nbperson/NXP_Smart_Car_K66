#include "include.h"

float Databuff[2];//�����һ������εľ���
char obstacle_flag=0;//·�ϱ�־λ

void Find_Obstacle()
{
  Databuff[1]=Databuff[0];
  Databuff[0]=Get_Distance();
  if(Databuff[1]<=50&&Databuff[0]<=50&&Databuff[1]>=30&&Databuff[0]>=30)    //�����һ�κ���һ�ξ��붼С��70cm��ʱ��
  { 
    obstacle_flag=1;//���·�ϱ�־λ
  }
  else
  {
    obstacle_flag=0;
  }

  if(Pixel[50][10] == 1 && Pixel[50][30] == 1 &&Pixel[50][50] == 1 &&Pixel[50][70] == 1)//�����Ч�д���50 ˵�� �ճ�����
  {
    Out_line_flag=1;//�����־λ
  }
  else
  {
    Out_line_flag=0;
  }

}