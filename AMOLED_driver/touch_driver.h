#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include "main.h"
#include "delay.h"
#include "ft3x68.h"


#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define TP_MAX_TOUCH  5    //电容屏支持的点数,固定为5点
#define Drawdots     0 //0-触摸画点
#define Drawlines    1 //1-触摸画线

typedef struct
{
	uint8_t (*init)(void);			//初始化触摸屏控制器
	uint8_t (*scan)(uint8_t);		//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);		//触摸屏校准 
	int x[TP_MAX_TOUCH]; 		//当前坐标
	int y[TP_MAX_TOUCH];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
													//x[4],y[4]存储第一次按下时的坐标. 
	unsigned char  sta;			//笔的状态 
								//b7:按下1/松开0; 
	              //b6:0,没有按键按下;1,有按键按下. 
								//b5:保留
								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
/////////////////////触摸屏校准参数(电容屏不需要校准)//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
//   1,横屏(适合左右为Y坐标,上下为X坐标的TP) 
//b1~6:保留.
//b7:0,电阻屏
//   1,电容屏 
	unsigned char touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

uint8_t TP_Init(void);
uint8_t TP_Scan(uint8_t tp);
void TP_Paint(void);

#endif /* touch_driver! */