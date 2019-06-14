#ifndef INCLUDE_H_
#define INCLUDE_H_

/*
 * ����Cortex-M�ں˵�ͨ��ͷ�ļ�
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc
#include "math.h"
#include "common.h" 
#include "MK66F18.h"   /* �Ĵ���ӳ��ͷ�ļ� */
#include "core_cm4.h"         /* �ں��ļ����������ж����ȼ� */

/*----------------------------------------------------------------
                   �ײ������ļ�
------------------------------------------------------------------*/
#include "MK60_ADC.h"              /* ����ADCģ�� */
#include "MK60_GPIO.h"             /* ����GPIOģ�� */
#include "MK60_GPIO_Cfg.h"         /* ����GPIOģ�� ��51��IO���� */
#include "MK60_PLL.h"              /* ����ʱ��Ƶ������ */
#include "MK60_UART.h"             /* ���ڴ���ģ�� */
#include "MK60_PIT.h"              /* ����PIT��ʱ��ģ�� */
#include "MK60_FTM.h"              /* ����FTM��ʱ��ģ�� */
#include "MK60_CMT.h"              /* ����CMT��ʱ��ģ�� */
#include "MK60_IIC.h"              /* ����IICģ�� */
#include "MK60_DMA.h"              /* ����DMAģ�� */
#include "MK60_LPTMR.h"            /* ����LPTMR��ʱ��ģ�� */
#include "MK60_WDOG.h"             /* ���ڿ��Ź� */
#include "MK60_SYSTICK.h"          /* systick �ں˶�ʱ�� */
#include "MK60_FLASH.h"            /* Flash ��д */


#include "LED.h"       
#include "KEY.h"       
#include "OLED.h"       
#include "CAMERA.h"
#include "encoder.h"  
#include "pid.h"
#include "control.h"

#include "HC_SR04.h"
#include "speed.h"
#include "type.h"
#include "connect.h"


//#define ABS(x) (((x) > 0) ? (x) : (-(x)))
//#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
//#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif