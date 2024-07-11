#include "touch_driver.h"
#include "amoled_driver.h"

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};


//触摸屏初始化  		    
//返回值:0,没有进行校准
//       1,进行过校准
uint8_t TP_Init(void)
{
	FT3X68_Init();
	tp_dev.scan = FT3X68_Scan;		//扫描函数指向FT5X06触摸屏扫描		
	
	return 0;
}

//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t TP_Scan(uint8_t tp)
{
	unsigned char i=0;

	if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
	{
		tp_dev.sta&=~(1<<7);//标记按键松开	
	}else//之前就没有被按下
	{
		tp_dev.x[4]=0;
		tp_dev.y[4]=0;
		tp_dev.x[0]=0xffff;
		tp_dev.y[0]=0xffff;
	}	    

	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}

void CTP_Load_Drow_Dialog(void)
{
	AMOLED_Clear(WHITE);//GREEN);//清屏   
 	PEN_COLOR=RED;//设置字体为
  //AMOLED_Fill(COL-46,0,COL,23,GREEN);//显示清屏区域
	
	AMOLED_Disp_Str(100,176,48,"CTP TEST");//
	PEN_COLOR=GREEN;//设置字体颜色
	AMOLED_Disp_Str(134,10,48,"RESET");
	
	PEN_COLOR=BLUE;//设置字体颜色
	AMOLED_Disp_Str(50,420,24,"Press K2 to next page");	
}

void AMOLED_Drawbigpoint_3x3(unsigned int x,unsigned int y,long int frontcolor)//画一个大点(3*3的点)
{
  PEN_COLOR=frontcolor;
	
	AMOLED_DrawPoint(x-1,y-1,PEN_COLOR);
	AMOLED_DrawPoint(x-1,y,PEN_COLOR);	
	AMOLED_DrawPoint(x,y-1,PEN_COLOR);
	
	AMOLED_DrawPoint(x,y,PEN_COLOR);//中心点 

	AMOLED_DrawPoint(x+1,y-1,PEN_COLOR);
	AMOLED_DrawPoint(x-1,y+1,PEN_COLOR);	
	
	AMOLED_DrawPoint(x+1,y,PEN_COLOR);
	AMOLED_DrawPoint(x,y+1,PEN_COLOR);	
	AMOLED_DrawPoint(x+1,y+1,PEN_COLOR);	
	
}

void AMOLED_Drawbigpoint_2x2(unsigned int x,unsigned int y,long int frontcolor)//画一个大点(2*2的点)
{
  AMOLED_DrawPoint(x,y,PEN_COLOR);//中心点 	
	AMOLED_DrawPoint(x+1,y,PEN_COLOR);
	AMOLED_DrawPoint(x,y+1,PEN_COLOR);
	AMOLED_DrawPoint(x+1,y+1,PEN_COLOR);	
  	
}

//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(int x0,int y0,int len,int color)
{
	if(len==0)return;
	AMOLED_Fill(x0,y0,x0+len-1,y0,color);	
}

////////////////////////////////////////////////////////////////////////////////
//10个触控点的颜色(电容触摸屏用)												 
const int POINT_COLOR_TBL[3]={RED,GREEN,BLUE};  

//电容触摸屏小画板
void TP_Paint(void)
{
	unsigned char key;
  int	t=0; 
	unsigned char maxp=5;
	int i=0;
 	int lastpos[10][2];		//最后一次的数据 
	
 	tp_dev.init();
	HAL_Delay(10);
	CTP_Load_Drow_Dialog();

	while(1)
	{
		tp_dev.scan(1); //TP_Scan	
		
		for(t=0;t<maxp;t++)
		{		
			if((tp_dev.sta)&(1<<t))//if(tp_dev.sta & TP_PRES_DOWN)			//触摸屏被按下
			{	
				if( ((tp_dev.x[0] < COL) && (tp_dev.y[0]<ROW)) && ((tp_dev.x[0] >0) || tp_dev.y[0]>0))
				{
					if(tp_dev.x[0]>134 && tp_dev.x[0]<254 && tp_dev.y[0]<58 ){CTP_Load_Drow_Dialog();}//清除	
					//else if((( tp_dev.x[0]>146 && tp_dev.x[0]<(146+24*4) ) && tp_dev.y[0]>(448-48) && tp_dev.y[0]<(ROW-1) )){break;}	
					else if( (tp_dev.x[0] < COL) && (tp_dev.y[0]<ROW) && ((tp_dev.x[0] >0) || tp_dev.y[0]>0))											
					{
					PEN_COLOR=RED;//POINT_COLOR_TBL[t]; 																		
					AMOLED_Drawbigpoint_3x3(tp_dev.x[0],tp_dev.y[0],PEN_COLOR);	//画点 
					}											
					if(lastpos[t][0]==0XFFFF || lastpos[t][0]==0XFFF)
						{
							lastpos[t][0] = tp_dev.x[t];
							lastpos[t][1] = tp_dev.y[t];
						}
				 }	
			} else lastpos[t][0]=0XFFFF;//else HAL_Delay(10);	//没有按键按下的时候
				
//			else if((  ( tp_dev.x[0]>146 && tp_dev.x[0]<242 )  && (tp_dev.y[0]>(448-48) && tp_dev.y[0]<(ROW-1)) ))	//NEXT按下,则显示后面的测试画面
//			{
//			break;
//			}					
		 }
		}
	AMOLED_Clear(WHITE);//GREEN);//清屏   
 	PEN_COLOR=RED;//设置字体为
	
	AMOLED_Disp_Str(50,100,48,"CTP TEST END");//
	HAL_Delay(200);
}	