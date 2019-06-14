#include "include.h"

/*------------------------------------------------------------------------------------------------------
����    ����LED_Init
����    �ܡ���ʼ�����İ��ĸ���ϵ�LED
����    ������
���� �� ֵ����
��ʵ    ����LED_Init(); //��ʼ��LED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_Init(void)
{
    /* ��ʼ�����İ��ϵ�LED */
   GPIO_PinInit(PTA17, GPO, 1);
   GPIO_PinInit(PTC0 , GPO, 1);
   GPIO_PinInit(PTD15, GPO, 1);
   GPIO_PinInit(PTE26, GPO, 1);
   
   /* ��ʼ��ĸ���ϵ�LED */
   GPIO_PinInit(PTC18, GPO, 1);
   GPIO_PinInit(PTC19, GPO, 1);
}

/*------------------------------------------------------------------------------------------------------
����    ����LED_ON
����    �ܡ�����
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_ON(1); //��LED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_ON(uint8_t led)
{
    switch(led)
    {
      case 0:
        GPIO_PinInit(PTA17, GPO, 0);
        break;
        
      case 1:
        GPIO_PinInit(PTC0 , GPO, 0);
        break;
        
      case 2:
        GPIO_PinInit(PTD15, GPO, 0);
        break;
        
      case 3:
        GPIO_PinInit(PTE26, GPO, 0);
        break;
        
      case 4:
        GPIO_PinInit(PTC18, GPO, 0);
        break;
        
      case 5:
        GPIO_PinInit(PTC19, GPO, 0);
        break;
        
      default:
        break;
    }
}


/*------------------------------------------------------------------------------------------------------
����    ����LED_OFF
����    �ܡ��ص�
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_OFF(1); //��LED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_OFF(uint8_t led)
{

    switch(led)
    {
      case 0:
        GPIO_PinInit(PTA17, GPO, 1);
        break;
        
      case 1:
        GPIO_PinInit(PTC0 , GPO, 1);
        break;
        
      case 2:
        GPIO_PinInit(PTD15, GPO, 1);
        break;
        
      case 3:
        GPIO_PinInit(PTE26, GPO, 1);
        break;
        
      case 4:
        GPIO_PinInit(PTC18, GPO, 1);
        break;
        
      case 5:
        GPIO_PinInit(PTC19, GPO, 1);
        break;
        
      default:
        break;
    }
}
/*------------------------------------------------------------------------------------------------------
����    ����LED_Reverse
����    �ܡ���תLED
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_Reverse(1); //��תLED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_Reverse(uint8_t led)
{
    /* ��¼LED״̬ */
    static uint8_t led_state[6];
    led_state[led]++;
    
    if(0 == led_state[led]%2)
    {
        LED_ON(led);
    }
    else
    {
        LED_OFF(led);
    }
}