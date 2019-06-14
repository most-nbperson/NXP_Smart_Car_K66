#include "include.h"
#include "MK60_SYSTICK.h"
#define EACH_PER_MS    25   //ÿ�� 25 ms �ж�һ��  systick ��ʱ����24λ���¼����Ķ�ʱ��  ���װ��ֵ16777215 / 600 000 000= 0.2796 ����ʱ27ms

struct time{
	
    uint32_t fac_us;                  //us��Ƶϵ��
	uint32_t fac_ms;                  //ms��Ƶϵ��
	volatile uint32_t millisecond;   //ms
	uint64_t microsecond;             //us
	uint8_t ms_per_tick;              //1ms����systick��������
	
}timer;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵������ʼ��systick������  
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����
������ֵ����     
������  ��systime.init();                     //����systick��ʱ�� 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static void systime_init()
{
	timer.fac_us = core_clk;
	timer.fac_ms = core_clk * 1000;
	timer.ms_per_tick = EACH_PER_MS;
    timer.millisecond = 100;
	SysTick_Config(timer.fac_ms * timer.ms_per_tick );   //����systick�ж�
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵����systick  �жϷ����� 
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����
������ֵ����     
������  ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SysTick_Handler(void)
{
	timer.millisecond += timer.ms_per_tick;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵����systick  ��ȡ��ǰmsֵ 
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����ǰmsֵ
������ֵ����     
������  ��systime.get_time_ms();         //��ʱ����  �õ���ǰʱ�� ms   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static uint32_t get_current_time_ms(void)
{
    uint32_t val = SysTick->VAL;
	return timer.millisecond -  val/timer.fac_ms;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵����systick  ��ȡ��ǰmsֵ 
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����ǰmsֵ
������ֵ����     
������  ��systime.get_time_us();         //��ʱ����  �õ���ǰʱ�� us   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static uint64_t get_current_time_us(void)
{
    uint32_t val = SysTick->VAL;
	return (uint64_t)((uint64_t)(timer.millisecond * 1000) -  val / timer.fac_us);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵����systick  ��ʱus 
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����
������ֵ����     
������  ��systime.delay_us(1000000);         //��ʱ����  ��ʱ1s
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static void delay_us( uint32_t us)     
{
    uint64_t now = systime.get_time_us();
	uint64_t end_time = now + us - 1;
	while( systime.get_time_us() < end_time)
    {
        ;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵����systick  ��ʱms
������汾��V1.0
�������¡�2019��03��12�� 
������ֵ����
������ֵ����     
������  ��systime.delay_ms(1000);         //��ʱ���� ��ʱ1s 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
static void delay_ms( uint16_t ms) 
{
	systime.delay_us(ms * 1000);
}

systime_t  systime = 
{
	systime_init,
	get_current_time_us,
	get_current_time_ms,
	delay_us,
	delay_ms
};



