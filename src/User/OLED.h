#ifndef __OLED_H__
#define __OLED_H__

/*  母板上使用OLED请将 OLED_PIN 定义为1   核心板上使用请将OLED_PIN 定义为0 */
#define OLED_PIN    1

void OLED_Init(void);
void OLED_CLS(void);
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_PutPixel(unsigned char x,unsigned char y);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
void OLED_Show_Road(void);
void OLED_Show_Frame80(void);
void OLED_Show_Frame94(void);
void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Fill(unsigned char bmp_data);
void OLED_DLY_ms(unsigned int ms);
void Draw_Mypoint(uint8 y,uint8 x);
#endif

