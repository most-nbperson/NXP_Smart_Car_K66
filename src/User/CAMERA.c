#include "include.h"

uint8_t Image_Data[IMAGEH][IMAGEW];      //ͼ��ԭʼ���ݴ��
uint8_t Image_Use[LCDH][LCDW];           //ѹ����֮�����ڴ����Ļ��ʾ����
uint8_t Pixel[LCDH][LCDW];               //��ֵ���������
uint8_t edge[60][80]={0};
uint8_t Line_Cont=0;                    //�м���
uint8_t Field_Over_Flag=0;              //����ʶ
uint8 imgbuff[CAMERA_SIZE];
uint8_t range;

u16 all;
uint8_t disyuzhi;



void LQMT9V034_Init(uint8_t fps)
{     
	uint16_t data = 0;  
	
	//����ͷ�Ĵ�������
	SCCB_Init();                     
	
	if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)  //��ȡ����ͷ�汾�Ĵ��� 
	{     
//		if(data != MT9V034_CHIP_ID)                                        //оƬID����ȷ��˵��û����ȷ��ȡ�����ݣ��ȴ�      
//		{ 
//
//			OLED_P6x8Str(2,1,(u8*)"V034 NG!!");                       
//
//			while(1); 
//		} 
//		else                                                               //оƬID��ȷ
//		{
//
//			OLED_P6x8Str(2,1,(u8*)"V034 OK");                              
//		}
//	} 
//	else 
//	{ 
//		OLED_P6x8Str(2,1,(u8*)"V034 NG!!");                      
//
//		while(1);                                                         //����ͷʶ��ʧ�ܣ�ֹͣ����
	}  
	/* �ָ�Ĭ������ */
	MT9V034_Default_Settings();
	
	/* ��������ͷͼ��4*4��Ƶ���PCLK 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� */
	MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW, fps); 
	
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //�ο���ѹ����   1.4v 
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //����ʴ
    
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 10);             //0xAB  ���ģ������     64
    
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 0xB0);//0xB0  ����AEC/AGCֱ��ͼ������Ŀ,���44000   IMAGEH*IMAGEW  
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);     //0x1C  here is the way to regulate darkness :)    
    ////  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.  
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,0x03c7);//0x01C7�ԱȶȲ���ף�0x03C7�Աȶ���� Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.  
    
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_SHUTTER_WIDTH_CONTROL,0x0164); //0x0A Coarse Shutter IMAGEW Control 
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A  
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_AGC_AEC_DESIRED_BIN_REG, 20);  //0xA5  ͼ������  60  1-64
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_ANALOG_GAIN,0x8010);  
    
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_RESET, 0x03);          //0x0c  ��λ 
  
 
}

 


static void MT9V034_SetFrameResolution(uint16_t height,uint16_t width, uint8_t fps)
{
    uint16_t data = 0;
    uint16_t frameRate = 0;
    if((height*4) <= MAX_IMAGE_HEIGHT)   //�ж����Ƿ�4��Ƶ
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
        if(fps > 193) fps = 193;         //���֡�� 193
        if(fps < 1) fps = 1;             //��С֡��  1
        
        if(fps > 132)                    //fps ���ߣ����ʺ�һ����
        {
            frameRate = (uint16_t)(-2.0 * fps + 504);  //����ģ�����ȷ
        }
        else
        {
            frameRate = (uint16_t)(132.0 / fps * 240);
        }
    }
    else if((height*2)<=MAX_IMAGE_HEIGHT )  //�ж����Ƿ�2��Ƶ
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
        
        if(fps > 112) fps = 112;    //���֡�� 112
        if(fps < 1) fps = 1;        //��С֡��  1
        
        if(fps > 66)  //fps ���ߣ����ʺ�һ����
        {
            frameRate = (uint16_t)(-5.2 * fps + 822);
        }
        else
        {
            frameRate = (uint16_t)(66.0 / fps * 480);
        }
    }
    
    else 
    {
        if(fps > 60) fps = 60;    //���֡�� 60
        if(fps < 1) fps = 1;        //��С֡��  1
        frameRate = (uint16_t)(60.0 / fps * 480);
        
    }
    if((width*4)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�4��Ƶ   
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�2��Ƶ
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }
    //         ˮƽ��ת                     ��ֱ��ת
    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);  //��Ҫ��ת�Ŀ��Դ�ע�ͣ����ߺ���PXPת��ʱ��תҲ����  
    
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //д�Ĵ����������з�Ƶ
    
    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH,  width);  //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�
    
    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //�п�ʼ
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //�п�ʼ
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,frameRate);   //0x0B �ع�ʱ�� Խ��֡��ԽС
}



void MT9V034_SetAutoExposure(char enable)
{
  uint16_t reg =0;
  SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
  if(1 == enable)
  {
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
  }
  else
  {
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
  }
}


void MT9V034_Default_Settings(void)   
{
    MTV_IICWriteReg16(0x01, 0x0001);   //COL_WINDOW_START_CONTEXTA_REG
    MTV_IICWriteReg16(0x02, 0x0004);   //ROW_WINDOW_START_CONTEXTA_REG
    MTV_IICWriteReg16(0x03, 0x01E0);   //ROW_WINDOW_SIZE_CONTEXTA_REG
    MTV_IICWriteReg16(0x04, 0x02F0);   //COL_WINDOW_SIZE_CONTEXTA_REG
    MTV_IICWriteReg16(0x05, 0x005E);   //HORZ_BLANK_CONTEXTA_REG
    MTV_IICWriteReg16(0x06, 0x002D);   //VERT_BLANK_CONTEXTA_REG
    MTV_IICWriteReg16(0x07, 0x0188);   //CONTROL_MODE_REG
    MTV_IICWriteReg16(0x08, 0x01BB);   //COARSE_SHUTTER_WIDTH_1_CONTEXTA
    MTV_IICWriteReg16(0x09, 0x01D9);   //COARSE_SHUTTER_WIDTH_2_CONTEXTA
    MTV_IICWriteReg16(0x0A, 0x0164);   //SHUTTER_WIDTH_CONTROL_CONTEXTA
    MTV_IICWriteReg16(0x0B, 0x0000);   //COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
    MTV_IICWriteReg16(0x0C, 0x0000);   //RESET_REG
    MTV_IICWriteReg16(0x0D, 0x0300);   //READ_MODE_REG
    MTV_IICWriteReg16(0x0E, 0x0000);   //READ_MODE2_REG
    MTV_IICWriteReg16(0x0F, 0x0100);   //PIXEL_OPERATION_MODE **************0x0000
    MTV_IICWriteReg16(0x10, 0x0040);   //RAMP_START_DELAY
    MTV_IICWriteReg16(0x11, 0x8042);   //OFFSET_CONTROL
    MTV_IICWriteReg16(0x12, 0x0022);   //AMP_RESET_BAR_CONTROL
    MTV_IICWriteReg16(0x13, 0x2D2E);   //5T_PIXEL_RESET_CONTROL
    MTV_IICWriteReg16(0x14, 0x0E02);   //4T_PIXEL_RESET_CONTROL
    MTV_IICWriteReg16(0x15, 0x0E32);   //TX_CONTROL
    MTV_IICWriteReg16(0x16, 0x2802);   //5T_PIXEL_SHS_CONTROL
    MTV_IICWriteReg16(0x17, 0x3E38);   //4T_PIXEL_SHS_CONTROL
    MTV_IICWriteReg16(0x18, 0x3E38);   //5T_PIXEL_SHR_CONTROL
    MTV_IICWriteReg16(0x19, 0x2802);   //4T_PIXEL_SHR_CONTROL
    MTV_IICWriteReg16(0x1A, 0x0428);   //COMPARATOR_RESET_CONTROL
    MTV_IICWriteReg16(0x1B, 0x0000);   //LED_OUT_CONTROL
    MTV_IICWriteReg16(0x1C, 0x0302);   //DATA_COMPRESSION
    MTV_IICWriteReg16(0x1D, 0x0040);   //ANALOG_TEST_CONTROL
    MTV_IICWriteReg16(0x1E, 0x0000);   //SRAM_TEST_DATA_ODD
    MTV_IICWriteReg16(0x1F, 0x0000);   //SRAM_TEST_DATA_EVEN
    MTV_IICWriteReg16(0x20, 0x03C7);   //BOOST_ROW_EN
    MTV_IICWriteReg16(0x21, 0x0020);   //I_VLN_CONTROL
    MTV_IICWriteReg16(0x22, 0x0020);   //I_VLN_AMP_CONTROL
    MTV_IICWriteReg16(0x23, 0x0010);   //I_VLN_CMP_CONTROL
    MTV_IICWriteReg16(0x24, 0x001B);   //I_OFFSET_CONTROL
//    MTV_IICWriteReg1G=0x25, 0x0000); // I_BANDGAP_CONTROL - TRIMMED PER DIE
    MTV_IICWriteReg16(0x26, 0x0004);   //I_VLN_VREF_ADC_CONTROL
    MTV_IICWriteReg16(0x27, 0x000C);   //I_VLN_STEP_CONTROL
    MTV_IICWriteReg16(0x28, 0x0010);   //I_VLN_BUF_CONTROL
    MTV_IICWriteReg16(0x29, 0x0010);   //I_MASTER_CONTROL
    MTV_IICWriteReg16(0x2A, 0x0020);   //I_VLN_AMP_60MHZ_CONTROL
    MTV_IICWriteReg16(0x2B, 0x0004);   //VREF_AMP_CONTROL
    MTV_IICWriteReg16(0x2C, 0x0004);   //VREF_ADC_CONTROL
    MTV_IICWriteReg16(0x2D, 0x0004);   //VBOOST_CONTROL
    MTV_IICWriteReg16(0x2E, 0x0007);   //V_HI_CONTROL
    MTV_IICWriteReg16(0x2F, 0x0003);   //V_LO_CONTROL
    MTV_IICWriteReg16(0x30, 0x0003);   //V_AMP_CAS_CONTROL
    MTV_IICWriteReg16(0x31, 0x0027);   //V1_CONTROL_CONTEXTA
    MTV_IICWriteReg16(0x32, 0x001A);   //V2_CONTROL_CONTEXTA
    MTV_IICWriteReg16(0x33, 0x0005);   //V3_CONTROL_CONTEXTA
    MTV_IICWriteReg16(0x34, 0x0003);   //V4_CONTROL_CONTEXTA
    MTV_IICWriteReg16(0x35, 0x0010);   //GLOBAL_GAIN_CONTEXTA_REG
    MTV_IICWriteReg16(0x36, 0x8010);   //GLOBAL_GAIN_CONTEXTB_REG
    MTV_IICWriteReg16(0x37, 0x0000);   //VOLTAGE_CONTROL
    MTV_IICWriteReg16(0x38, 0x0000);   //IDAC_VOLTAGE_MONITOR
    MTV_IICWriteReg16(0x39, 0x0027);   //V1_CONTROL_CONTEXTB
    MTV_IICWriteReg16(0x3A, 0x0026);   //V2_CONTROL_CONTEXTB
    MTV_IICWriteReg16(0x3B, 0x0005);   //V3_CONTROL_CONTEXTB
    MTV_IICWriteReg16(0x3C, 0x0003);   //V4_CONTROL_CONTEXTB
    MTV_IICWriteReg16(0x40, 0x0080);   //DARK_AVG_THRESHOLDS
    MTV_IICWriteReg16(0x46, 0x231D);   //CALIB_CONTROL_REG (AUTO)
    MTV_IICWriteReg16(0x47, 0x0080);   //STEP_SIZE_AVG_MODE
    MTV_IICWriteReg16(0x48, 0x0020);   //ROW_NOISE_CONTROL
    MTV_IICWriteReg16(0x4C, 0x0002);   //NOISE_CONSTANT
    MTV_IICWriteReg16(0x60, 0x0000);   //PIXCLK_CONTROL
    MTV_IICWriteReg16(0x67, 0x0000);   //TEST_DATA
    MTV_IICWriteReg16(0x6C, 0x0000);   //TILE_X0_Y0
    MTV_IICWriteReg16(0x70, 0x0000);   //TILE_X1_Y0
    MTV_IICWriteReg16(0x71, 0x002A);   //TILE_X2_Y0
    MTV_IICWriteReg16(0x72, 0x0000);   //TILE_X3_Y0
    MTV_IICWriteReg16(0x7F, 0x0000);   //TILE_X4_Y0
    MTV_IICWriteReg16(0x99, 0x0000);   //TILE_X0_Y1
    MTV_IICWriteReg16(0x9A, 0x0096);   //TILE_X1_Y1
    MTV_IICWriteReg16(0x9B, 0x012C);   //TILE_X2_Y1
    MTV_IICWriteReg16(0x9C, 0x01C2);   //TILE_X3_Y1
    MTV_IICWriteReg16(0x9D, 0x0258);   //TILE_X4_Y1
    MTV_IICWriteReg16(0x9E, 0x02F0);   //TILE_X0_Y2
    MTV_IICWriteReg16(0x9F, 0x0000);   //TILE_X1_Y2
    MTV_IICWriteReg16(0xA0, 0x0060);   //TILE_X2_Y2
    MTV_IICWriteReg16(0xA1, 0x00C0);   //TILE_X3_Y2
    MTV_IICWriteReg16(0xA2, 0x0120);   //TILE_X4_Y2
    MTV_IICWriteReg16(0xA3, 0x0180);   //TILE_X0_Y3
    MTV_IICWriteReg16(0xA4, 0x01E0);   //TILE_X1_Y3
    MTV_IICWriteReg16(0xA5, 0x003A);   //TILE_X2_Y3
    MTV_IICWriteReg16(0xA6, 0x0002);   //TILE_X3_Y3
    MTV_IICWriteReg16(0xA8, 0x0000);   //TILE_X4_Y3
    MTV_IICWriteReg16(0xA9, 0x0002);   //TILE_X0_Y4
    MTV_IICWriteReg16(0xAA, 0x0002);   //TILE_X1_Y4
    MTV_IICWriteReg16(0xAB, 0x0040);   //TILE_X2_Y4
    MTV_IICWriteReg16(0xAC, 0x0001);   //TILE_X3_Y4
    MTV_IICWriteReg16(0xAD, 0x01E0);   //TILE_X4_Y4
    MTV_IICWriteReg16(0xAE, 0x0014);   //X0_SLASH5
    MTV_IICWriteReg16(0xAF, 0x0000);   //X1_SLASH5
    MTV_IICWriteReg16(0xB0, 0xABE0);   //X2_SLASH5
    MTV_IICWriteReg16(0xB1, 0x0002);   //X3_SLASH5
    MTV_IICWriteReg16(0xB2, 0x0010);   //X4_SLASH5
    MTV_IICWriteReg16(0xB3, 0x0010);   //X5_SLASH5
    MTV_IICWriteReg16(0xB4, 0x0000);   //Y0_SLASH5
    MTV_IICWriteReg16(0xB5, 0x0000);   //Y1_SLASH5
    MTV_IICWriteReg16(0xB6, 0x0000);   //Y2_SLASH5
    MTV_IICWriteReg16(0xB7, 0x0000);   //Y3_SLASH5
    MTV_IICWriteReg16(0xBF, 0x0016);   //Y4_SLASH5
    MTV_IICWriteReg16(0xC0, 0x000A);   //Y5_SLASH5
    MTV_IICWriteReg16(0xC2, 0x18D0);   //DESIRED_BIN
    MTV_IICWriteReg16(0xC3, 0x007F);   //EXP_SKIP_FRM_H
    MTV_IICWriteReg16(0xC4, 0x007F);   //EXP_LPF
    MTV_IICWriteReg16(0xC5, 0x007F);   //GAIN_SKIP_FRM
    MTV_IICWriteReg16(0xC6, 0x0000);   //GAIN_LPF_H
    MTV_IICWriteReg16(0xC7, 0x4416);   //MAX_GAIN
    MTV_IICWriteReg16(0xC8, 0x4421);   //MIN_COARSE_EXPOSURE
    MTV_IICWriteReg16(0xC9, 0x0001);   //MAX_COARSE_EXPOSURE
    MTV_IICWriteReg16(0xCA, 0x0004);   //BIN_DIFF_THRESHOLD
    MTV_IICWriteReg16(0xCB, 0x01E0);   //AUTO_BLOCK_CONTROL
    MTV_IICWriteReg16(0xCC, 0x02F0);   //PIXEL_COUNT
    MTV_IICWriteReg16(0xCD, 0x005E);   //LVDS_MASTER_CONTROL
    MTV_IICWriteReg16(0xCE, 0x002D);   //LVDS_SHFT_CLK_CONTROL
    MTV_IICWriteReg16(0xCF, 0x01DE);   //LVDS_DATA_CONTROL
    MTV_IICWriteReg16(0xD0, 0x01DF);   //LVDS_DATA_STREAM_LATENCY
    MTV_IICWriteReg16(0xD1, 0x0164);   //LVDS_INTERNAL_SYNC
    MTV_IICWriteReg16(0xD2, 0x0001);   //LVDS_USE_10BIT_PIXELS
    MTV_IICWriteReg16(0xD3, 0x0000);   //STEREO_ERROR_CONTROL
    MTV_IICWriteReg16(0xD4, 0x0000);   //INTERLACE_FIELD_VBLANK
    MTV_IICWriteReg16(0xD5, 0x0104);   //IMAGE_CAPTURE_NUM
    MTV_IICWriteReg16(0xD6, 0x0000);   //ANALOG_CONTROLS
    MTV_IICWriteReg16(0xD7, 0x0000);   //AB_PULSE_WIDTH_REG
    MTV_IICWriteReg16(0xD8, 0x0000);   //TX_PULLUP_PULSE_WIDTH_REG
    MTV_IICWriteReg16(0xD9, 0x0000);   //RST_PULLUP_PULSE_WIDTH_REG
    MTV_IICWriteReg16(0xF0, 0x0000);   //NTSC_FV_CONTROL
    MTV_IICWriteReg16(0xFE, 0xBEEF);   //NTSC_HBLANK
}      

void MTV_IICWriteReg16(uint8_t reg, uint16_t val)
{	
	SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}



void SCCB_Init(void)
{
  GPIO_PinInit(SCCB_SCL_PIN, GPI_UP, 1);//����ΪGPIO����

  GPIO_PinInit(SCCB_SDA_PIN, GPI_UP, 1);//����ΪGPIO����
}


void SCCB_Wait(void)
{
  uint8 i=0;
  for(i=0;i<100;i++)
  { 
    asm ("nop");
  }  
}


void SCCB_Star(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_High;
  SCL_High; 
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_Low; 
  SCCB_Wait();
}


void SCCB_Stop(void)
{
  SCL_Out;
  SDA_Out;
  SCCB_Wait();
  SDA_Low;
  SCCB_Wait();
  SCL_High; 
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();  
}


uint8 SCCB_SendByte(uint8 Data)
{
  uint8 i;
  uint8 Ack;
  SDA_Out;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80) SDA_High;
    else            SDA_Low;    
    Data <<= 1;
    SCCB_Wait();
    SCL_High;      
    SCCB_Wait();
    SCL_Low;
    SCCB_Wait();
  }
  SDA_High;
  SDA_In;
  SCCB_Wait();
  
  SCL_High;
  SCCB_Wait();
  Ack = SDA_Data;
  SCL_Low;
  SCCB_Wait();
  return Ack;
}


uint8 SCCB_ReadByte(void) 
{ 
  uint8 i; 
  uint8_t byte; 
  SCL_Out; 
  SDA_In; //ʹ������
  for( i=0; i<8; i++) 
  { 
    SCL_Low;
    SCCB_Wait(); 
    SCL_High;
    SCCB_Wait();
    byte = (byte<<1)|(SDA_Data & 1);
  }
  SCL_Low; 
  SDA_Out;
  SCCB_Wait(); 
  return byte; 
} 


static void SCCB_Ack(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SDA_Low;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 


static void SCCB_NAck(void) 
{ 
  SCL_Out; 
  SDA_Out;
  SCL_Low;
  SCCB_Wait();
  SDA_High;
  SCCB_Wait();
  SCL_High;
  SCCB_Wait();
  SCL_Low;
  SCCB_Wait();
} 


void SCCB_RegWrite(uint8 Device,uint8 Address,uint16 Data)
{
  uint8 i;
  uint8 Ack;
  
  for( i=0; i<3; i++)
  {
    SCCB_Star();
    Ack = SCCB_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = SCCB_SendByte(Address);
    if( Ack == 1 )
    {
      continue;
    }
    
    Ack = SCCB_SendByte((uint8)(Data>>8));
    Ack = SCCB_SendByte((uint8)Data);
    if( Ack == 1 )
    {
      continue;
    }
    
    SCCB_Stop();
    if( Ack == 0 ) break;
  }
}

uint8_t SCCB_RegRead(uint8_t Device,uint8_t Address,uint16_t *Data) 
{   
  uint8 Ack = 0;
  Device = Device<<1;
  SCCB_Star();
  Ack += SCCB_SendByte(Device);
  
  Ack += SCCB_SendByte(Address);
  
  SCCB_Star();
  Ack += SCCB_SendByte(Device + 1);
  
  *Data = SCCB_ReadByte();
  SCCB_Ack();
  *Data = *Data<<8;
  
  *Data += SCCB_ReadByte();
  SCCB_NAck();
  
  SCCB_Stop();
  
  return  Ack;
} 

int SCCB_Probe(uint8_t chipAddr) 
{ 
  uint8_t err;
  err = 0;
  chipAddr <<= 1;
  
  SCCB_Star();
  err = SCCB_SendByte(chipAddr);
  SCCB_Stop();
  
  return err;
}


void PORTD_IRQHandler(void)
{     
  //���ж�PTD14
  int n;    
    n=14;   //���ж�
  if((PORTD_ISFR & (1<<n) ))//���ж� (1<<14)
  {    
    PORTD_ISFR |= (1<<n);   //����жϱ�ʶ
    // �û�����            
    DMATransDataStart(DMA_CH4,(uint32_t)(&Image_Data[Line_Cont][0]));   //����DMA���� 
    if(Line_Cont > IMAGEH)  //�ɼ�����
    { 
      Line_Cont=0; 
      return ;
    } 
    ++Line_Cont;            //�м���
    return ; 
  }
  //���ж�PTD15
  n=15;  //���ж�
  if((PORTD_ISFR & (1<<n)))//(1<<15)
  {
    PORTD_ISFR |= (1<<n);  //����жϱ�ʶ
    // �û����� 
    Line_Cont = 0;         //�м�������
    Field_Over_Flag=1;     //��������ʶ
  } 
}

//����ͷ��ʼ��
void Camera_Init(void)
{
  //OLED_Show_Frame94();      //��ͼ�� LCDW*LCDH ���
  LQMT9V034_Init(50);            //����ͷ��ʼ��
    
  //����ͷ�йؽӿڳ�ʼ��
  GPIO_ExtiInit(PTD14,rising_down);   //���ж�
  GPIO_ExtiInit(PTD15,falling_up);    //���ж�  
    
  NVIC_SetPriority(PORTD_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));
  NVIC_EnableIRQ(PORTD_IRQn);
    
  //��λ���������
  GPIO_PinInit(PTD0,GPI,0);                
  GPIO_PinInit(PTD1,GPI,0);
  GPIO_PinInit(PTD2,GPI,0);
  GPIO_PinInit(PTD3,GPI,0);
  GPIO_PinInit(PTD4,GPI,0);
  GPIO_PinInit(PTD5,GPI,0);
  GPIO_PinInit(PTD6,GPI,0);
  GPIO_PinInit(PTD7,GPI,0);     //ע�ⲻҪʹ�õ�PD15
    
               //       ͨ��4        PTD0-PTD7           Ŀ�ĵ�ַ      �����ź� ÿ��һ��BYTE  ����    ���������ش���
  DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN,(void*)Image_Data, PTD13, DMA_BYTE1, (IMAGEW ), DMA_rising_down); 
}

//�ɼ�ͼ��
void Get_Finally_Image(void)
{
    Get_Use_Image();  
    Get_01_Value();
    Pixel_Filter();
    imgtrans();
    LED_Reverse(1);
}


/*---------------------------------------------------------------
����    ����Get_Use_Image
����    �ܡ���ȡ��Ҫʹ�õ�ͼ���С
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
__ramfunc void Get_Use_Image(void)
{
  int i = 0,j = 0,row = 0,line = 0;
  uint8_t div = IMAGEH/LCDH;
  for(i = 0; i  < IMAGEH; i += div)    //IMAGEH / div  = LCDH
  {
    for(j = 0; j < IMAGEW; j += div)  //IMAGEW / div  = LCDW
    {        
      Image_Use[row][line] = Image_Data[i][j];    
      line++;        
    }      
    line = 0;
    row++;      
  }  
}

/*---------------------------------------------------------------
����    ����Get_01_Value
����    �ܡ���ֵ��
����    ����mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Get_01_Value()
{
//  uint16 i = 0,j = 0,N0 = 0,N1 = 0,flag = 0 ;
//  float T0,T1,T2,T_center;
//  uint32 S0 = 0,S1 = 0;
//  T2 = disyuzhi;//BlackThres;  //��һ����ֵ
//  N0 = 0;
//  N1 = 0;
//  do
//  { 
//    for(i=0;i<60;i++)
//    {  
//      for(j=0;j<80;j++)
//      {  
//        if(Pixel[i][j] < T2)  //С����ֵ�ĵ�
//        {
//          S0 += Pixel[i][j];  //�Ҷ�ֵ�ۼ�
//          N0++;              //��ĸ���
//        }
//        else
//        { 
//          S1 += Pixel[i][j];  //�Ҷ�ֵ�ۼ�
//          N1++;              //��ĸ���
//        }
//      }
//    }
//    T0 = S0/(N0+N1);   //�Ƚϰ��ĵ��ƽ��ֵ
//    T1 = S1/(N0+N1);  // �Ƚ����ĵ��ƽ��ֵ
//    all = N0 + N1;
//    T_center = (T0+T1);//T0*N0/4800  + T1*N1/4800; //ȡƽ��ֵ
//    if(T2 < T_center)  //��һ��ͼ�����ֵ�ȵ�ǰͼ����������ֵС
//    {	 
//      if((T_center - T2)> ERROR)  //��ֵ����ERROR
//      {  
//        flag = 1;  //�����������ҵ�ǰ�����μ����������ֵ��С��ERROR����ֵ���Ǹ÷�ͼ��������ֵ
//      }
//      else
//      {  
//        flag = 0;  //�ҵ������ֵ     
//      }
//    }
//    else
//    {  
//      if((T2 - T_center) > ERROR)
//      { 
//        flag = 1;
//      }
//      else
//      {
//        flag = 0;
//      }
//    }
//    T2 = T_center;
//    disyuzhi = T2 + 0.5;
//  }
//  while(flag);
//  
//  unsigned char m,n;
//  for(m=0; m<60; m++)   
//  { 
//    for(n=0;n<80;n++)
//    { 
//      if(Pixel[m][n]>disyuzhi)
//        Pixel[m][n]=1;
//      else
//        Pixel[m][n]=0;  //��ֵ��
//    }
//  }
  
  int i = 0,j = 0;
  uint8_t Threshold;
  
  Threshold = GetOSTU(Image_Use);//�����ֵ
  Threshold = (uint8_t)(Threshold * 0.5) + 70;

  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      if(Image_Use[i][j] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixel[i][j] =1;        
      else                                        
        Pixel[i][j] =0;
    }    
  }
}

/*---------------------------------------------------------------
����    ����Draw_Road
����    �ܡ���OLED�ϻ�������ͷ��ͼ��
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Draw_Road(void)
{ 	 
  u8 i = 0, j = 0,temp=0;
  
  //����֡ͷ��־
  for(i=8;i<56;i+=8)//6*8=48�� 
  {
    OLED_Set_Pos(18,i/8+1);//��ʼλ��
    for(j=0;j<LCDW;j++)  //����
    { 
      temp=0; 
      if(Pixel[0+i][j]) temp|=1;
      if(Pixel[1+i][j]) temp|=2;
      if(Pixel[2+i][j]) temp|=4;
      if(Pixel[3+i][j]) temp|=8;
      if(Pixel[4+i][j]) temp|=0x10;
      if(Pixel[5+i][j]) temp|=0x20;
      if(Pixel[6+i][j]) temp|=0x40;
      if(Pixel[7+i][j]) temp|=0x80;
      OLED_WrDat(temp); 	  	  	  	  
    }
  }  
}


/*---------------------------------------------------------------
����    ����Pixle_Filter
����    �ܡ��������
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Pixel_Filter(void)
{  
  int nr; //��
  int nc; //��
  
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
      if((Pixel[nr][nc]==0)&&(Pixel[nr-1][nc]+Pixel[nr+1][nc]+Pixel[nr][nc+1]+Pixel[nr][nc-1]>2))         
      {
        Pixel[nr][nc]=1;
      }	
      else if((Pixel[nr][nc]==1)&&(Pixel[nr-1][nc]+Pixel[nr+1][nc]+Pixel[nr][nc+1]+Pixel[nr][nc-1]<2))         
      {
        Pixel[nr][nc]=0;
      }	
    }	  
  }  
}



/***************************************************************
* 
* �������ƣ�SendPicture 
* ����˵��������ͼ����λ�� ����ͬ����λ��ע���޸Ķ�Ӧ�����ݽ���Э��
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
***************************************************************/ 
void UARTSendPicture(uint8_t  tmImage[IMAGEH][IMAGEW]) 
{ 
  int i = 0, j = 0; 
  UART_PutChar(UART4,0x01); //����֡ͷ��־ WindowsFormsApplication1.exe
  UART_PutChar(UART4,0xfe); //����֡ͷ��־ WindowsFormsApplication1.exe
  for(i=0;i < IMAGEH; i++) 
  { 
    for(j=0;j <IMAGEW ;j++) 
    { 
      if(tmImage[i][j]==0xfe) 
      { 
        tmImage[i][j]=0xff; //��ֹ���ͱ�־λ 
      } 
      UART_PutChar(UART4,tmImage[i][j]); 
    } 
  }
  UART_PutChar(UART4,0xfe); //����֡β��־ 
  UART_PutChar(UART4,0x01); //����֡β��־ 
} 




void data2trans()   // imgbuff[]--> Pixel[][]
{ 
  uint16 m,n;
  uint8 colour[2] = {0, 1}; //0 �� 1 �ֱ��Ӧ����ɫ
  for(m=0;m<60;m++)  //
  { for(n=0;n<10;n++) //  imgbuff[80*60/8] 
    { 
      Pixel[m][8*n]   = colour[(imgbuff[m*10+n] >> 7 ) & 0x01 ];
      Pixel[m][8*n+1] = colour[(imgbuff[m*10+n] >> 6 ) & 0x01 ];
      Pixel[m][8*n+2] = colour[(imgbuff[m*10+n] >> 5 ) & 0x01 ];
      Pixel[m][8*n+3] = colour[(imgbuff[m*10+n] >> 4 ) & 0x01 ];
      Pixel[m][8*n+4] = colour[(imgbuff[m*10+n] >> 3 ) & 0x01 ];
      Pixel[m][8*n+5] = colour[(imgbuff[m*10+n] >> 2 ) & 0x01 ];
      Pixel[m][8*n+6] = colour[(imgbuff[m*10+n] >> 1 ) & 0x01 ];
      Pixel[m][8*n+7] = colour[(imgbuff[m*10+n] >> 0 ) & 0x01 ];
    }
  }
}


void imgtrans()  // Pixel[][]-->imgbuffer[]
{  
  uint8 m,n;
  uint8 cval;
  ///// д�� imgbuffer
  for(m=0;m<60;m++)
  { 
    for(n=0;n<10;n++)
    {  
      cval = Pixel[m][n*8]*128 + Pixel[m][n*8+1]*64 + Pixel[m][n*8+2]*32 + Pixel[m][n*8+3]*16 + Pixel[m][n*8+4]*8 + Pixel[m][n*8+5]*4 + Pixel[m][n*8+6]*2 + Pixel[m][n*8+7];
      imgbuff[m*10+n] = cval; //255 - cval;   //�� 1    �� 0 
    }
  }
}


uint8_t GetOSTU(uint8_t tmImage[LCDH][LCDW]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
  
  for (j = 0; j < LCDH; j++) 
  { 
    for (i = 0; i < LCDW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
  if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
  if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];   //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
} 

//GATE:������ֵ��Maybe GATE = 16
void SobelEdge(int GATE)
{
	unsigned char sobelKernel[3][3] 
				={
					{-1,-1,-1},
					{-1, 0,-1},
					{-1,-1,-1}
				  };
	int i,j;
	int tmp;
	for (i=1;i<(60-1);i++)
		for(j=1;j<(80-1);j++)
		{
			tmp = sobelKernel[0][0]*Image_Use[i-1][j-1]+sobelKernel[0][1]*Image_Use[i-1][j]+sobelKernel[0][2]*Image_Use[i-1][j+1]
				 +sobelKernel[1][0]*Image_Use[i+0][j-1]+sobelKernel[1][1]*Image_Use[i+0][j]+sobelKernel[0][2]*Image_Use[i+0][j+1]
				 +sobelKernel[2][0]*Image_Use[i+1][j-1]+sobelKernel[2][1]*Image_Use[i+1][j]+sobelKernel[0][2]*Image_Use[i+1][j+1];
                   //tmp=Image_Use[i][j];
			if(ABS(tmp)<GATE)
			{
				edge[i][j] =0; 
			}
			else
			{
				edge[i][j] =1;
			}
		}
}

//Maybe will init the edge[0][*] edge[H-1][*] edge[*][0] edge[*][W-1]
//Call this func at the Imgproc Inital
void init_edge()
{
	int i;
	for (i=0;i<(60-1);i++)
	{
		edge[i][0]=edge[i][80-1] =0;
	}
	for (i=0;i<(80-1);i++)
	{
		edge[0][i]=edge[60-1][i] =0;
	}
}