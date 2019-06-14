#include "include.h"

/*------------------------------------------------------------------------------------------------------
【函    数】LED_Init
【功    能】初始化核心板和母板上的LED
【参    数】无
【返 回 值】无
【实    例】LED_Init(); //初始化LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void LED_Init(void)
{
    /* 初始化核心板上的LED */
   GPIO_PinInit(PTA17, GPO, 1);
   GPIO_PinInit(PTC0 , GPO, 1);
   GPIO_PinInit(PTD15, GPO, 1);
   GPIO_PinInit(PTE26, GPO, 1);
   
   /* 初始化母板上的LED */
   GPIO_PinInit(PTC18, GPO, 1);
   GPIO_PinInit(PTC19, GPO, 1);
}

/*------------------------------------------------------------------------------------------------------
【函    数】LED_ON
【功    能】开灯
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_ON(1); //开LED
【注意事项】
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
【函    数】LED_OFF
【功    能】关灯
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_OFF(1); //关LED
【注意事项】
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
【函    数】LED_Reverse
【功    能】翻转LED
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_Reverse(1); //翻转LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void LED_Reverse(uint8_t led)
{
    /* 记录LED状态 */
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