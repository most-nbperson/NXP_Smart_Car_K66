#include "include.h"  

unsigned char ready=0;

void XBEE_Init()
{
  UART_Init(UART3,9600);
}

void Get_Connect_Main()//主机
{
  char a;
  while(1)
  {
      //发出信号

    UART_PutChar(UART3,'a');
    
      //等待接收
    a=UART_GetChar(UART3);
    if(a=='b')
    {
      ready=1;
      break;
    }
  }
  
}

void Get_Connect_Slave()//从机
{
  char b;
  while(1)
  {
    b=UART_GetChar(UART3);
    //已接收到信号，进行回应
    if(b=='a')
    {
      UART_PutChar(UART3,'b');
      break;
    }
  }
}

void Break_Send() 
{
  char c;
  if(break_flag)
  {
    ready=0;
    //发出信号
    UART_PutChar(UART3,'c');
    //等待接收
    c=UART_GetChar(UART3);
    if(c=='d')
    {
      ready=1;
    }
  }
}

void Break_Get()
{
  char d;
  if(break_flag)
  {
    d=UART_GetChar(UART3);
    if(d=='c')
    {
      UART_PutChar(UART3,d);
      ready=1;
    }
  }
}