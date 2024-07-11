#ifndef AMOLED_DRIVER_H
#define AMOLED_DRIVER_H

#include "main.h"
#include "quadspi.h"
#include "stdio.h"
#include "delay.h"

#define COL	368           
#define ROW	448

#define MAX_CHAR_POSX  COL-8
#define MAX_CHAR_POSY  ROW-8

extern uint32_t PEN_COLOR;	//画笔颜色
extern uint32_t PAPER_COLOR;  //画板颜色（背景色）

#define RED 				0xFF0000	// 纯红
#define GREEN 				0x00FF00	// 纯绿
#define BLUE	 			0x0000FF	// 纯蓝
#define WHITE	 			0xFFFFFF	// 纯白
#define BLACK	 			0x000000	// 纯黑

void qspi_amoled_Init(void);
void AMOLED_Clear(long int color);
void AMOLED_Block_Write(int Xstart, int Xend, int Ystart, int Yend);
void qspi_111_cmd_write(unsigned char Cmd, unsigned char* Data, long int Size);
void qspi_114_cmd_write(unsigned char Cmd, unsigned char* Data, long int Size);
void qspi_114_cmd_write(unsigned char Cmd, unsigned char*Data, long int Size);


void AMOLED_Fill(int xsta,int ysta,int xend,int yend,long int color);
void AMOLED_Disp_Str(int x,int y,int size,unsigned char* str);	
void AMOLED_Draw_Circle(int x0,int y0,int r);


void Color_bar(void);						//彩色条
void Gray_bar_H16(void);				//灰阶横条16
void Gray_bar_V16(void);				//灰阶竖条16\

void AMOLED_DrawPoint(int x,int y,long int frontcolor);						//画点-----PEN_COLOR:此点的颜色
void AMOLED_Draw_2x2point(int x,int y,long int frontcolor);				//画一个大点(2*2的点)
//void AMOLED_Draw_3x3point(int x0,int y0,long int frontcolor);
void AMOLED_DrawLine(int x1, int y1, int x2, int y2);							//画线---x1,y1:起点坐标---x2,y2:终点坐标  
void AMOLED_Draw_hline(int x0,int y0,int len,long int color);

void AMOLED_Fill_ellipse(int x0,int y0,int rx,int ry,long int color);
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void AMOLED_Draw_Circle(int x0,int y0,int r); 

//画一个触摸点
//用来校准用的
void AMOLED_Draw_Touch_Point(int x,int y);


//在指定位置画一个指定大小的实心圆
//(x,y):中心点
//r    :半径
void AMOLED_Draw_Donut(int x0,int y0,int r);

void AMOLED_Fill_Circle(int x0, int y0, int r);

void AMOLED_Fill_Ellipse(int x0, int y0, int rx, int ry);
void AMOLED_Draw_Ellipse(int x0, int y0, int rx, int ry);
void AMOLED_Show4Num(int x,int y,int num,int size,int mode);
void AMOLED_ShowNum(int x,int y,int num,int size,int mode);

void AMOLED_Disp_SCALE2X2_Str(int x,int y,int size,unsigned char* str);
void AMOLED_Disp_SCALE4X4_Str(int x,int y,int size,unsigned char* str);
void AMOLED_Show4x4Num(int x,int y,int num,int size,int mode);

#endif /* amoled_driver! */
