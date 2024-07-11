#include "amoled_driver.h"
#include "amoled_font.h"

__IO uint8_t CmdCplt = 0, TxCplt = 0;

#define Buffer_Size (COL * ROW)
static uint8_t OLED_Buffer[Buffer_Size];

uint32_t PEN_COLOR=WHITE;	//画笔颜色
uint32_t PAPER_COLOR=BLACK;  //画板颜色（背景色）

void qspi_amoled_Init(void)
{
	HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_SET);	// PWR_EN---LOW
	HAL_GPIO_WritePin(IM1_GPIO_Port, IM1_Pin, GPIO_PIN_SET);	// IM1---HIGH
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);	// RESET---LOW
	delay_ms(10);
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);	// RESET---HIGH
	delay_ms(50);
	
	qspi_111_cmd_write(0x11, (unsigned char[]){0x00}, 1);
	delay_ms(100);	

//	qspi_111_cmd_write(0x44, (uint8_t[]){0x01, 0xBF}, 2);
//	qspi_111_cmd_write(0x35, (uint8_t[]){0x00}, 1);
	
	qspi_111_cmd_write(0x3A, (unsigned char[]){0x55}, 1);
	qspi_111_cmd_write(0x53, (unsigned char[]){0x20}, 1);
	delay_ms(1);
	qspi_111_cmd_write(0x51, (unsigned char[]){0x00}, 1);
	delay_ms(1);
	qspi_111_cmd_write(0x29, (unsigned char[]){0x00}, 0);
	delay_ms(1);
	qspi_111_cmd_write(0x51, (unsigned char[]){0xFF}, 1);

	AMOLED_Clear(BLACK);
	delay_ms(10);
}


void qspi_111_cmd_write(uint8_t Cmd, unsigned char* Data, long int Size)
{
	QSPI_CommandTypeDef sCommand = {
		.Instruction = 0x02,
		.Address = 0x000000 | (Cmd << 8),
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.NbData	= Size,
		.InstructionMode = QSPI_INSTRUCTION_1_LINE,
		.AddressMode = QSPI_ADDRESS_1_LINE,
		.DataMode	= QSPI_DATA_1_LINE,
		.DummyCycles = 0,
		
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode	= QSPI_SIOO_INST_EVERY_CMD,
	};
	
	if(Size)
	{
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		
		if(HAL_QSPI_Transmit(&hqspi, Data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}else{
		sCommand.NbData = 1;
		sCommand.Address = 0x0000 | (Cmd);
		sCommand.AddressSize = QSPI_ADDRESS_16_BITS;
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		
		if(HAL_QSPI_Transmit(&hqspi, (uint8_t[]){0x00}, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}
}


void qspi_114_cmd_write(unsigned char Cmd, unsigned char* Data, long int Size)
{
	QSPI_CommandTypeDef sCommand = {
		.Instruction = 0x32,
		.Address = 0x000000 | (Cmd << 8),
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.NbData = Size,
		.InstructionMode = QSPI_INSTRUCTION_1_LINE,
		.AddressMode = QSPI_ADDRESS_1_LINE,
		.DataMode = QSPI_DATA_4_LINES,
		.DummyCycles = 0,
		
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD,
	};
	
	if(Size)
	{
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		if(HAL_QSPI_Transmit(&hqspi, Data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}else{
		sCommand.NbData = 1;
		sCommand.Address = 0x0000 | (Cmd);
		sCommand.AddressSize = QSPI_ADDRESS_16_BITS;
		sCommand.DataMode = QSPI_DATA_1_LINE;
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		
		if(HAL_QSPI_Transmit(&hqspi, (uint8_t[]){0x00}, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}
}

void qspi_144_cmd_write(unsigned char Cmd, unsigned char* Data, long int Size)
{
	QSPI_CommandTypeDef sCommand = {
		.Instruction = 0x12,
		.Address = 0x000000 | (Cmd << 8),
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.NbData = Size,
		.InstructionMode = QSPI_INSTRUCTION_1_LINE,
		.AddressMode = QSPI_ADDRESS_4_LINES,
		.DataMode = QSPI_DATA_4_LINES,
		.DummyCycles = 0,
		
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD,
	};
	
	if(Size)
	{
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		
		if(HAL_QSPI_Transmit(&hqspi, Data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}else{
		sCommand.NbData = 1;
		sCommand.Address = 0x0000 | (Cmd);
		sCommand.AddressSize = QSPI_ADDRESS_16_BITS;
		if(HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Command Failed");
		}
		
		if(HAL_QSPI_Transmit(&hqspi, (uint8_t[]){0x00}, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			//log_debug("HAL_QSPI_Transmit Failed");
		}
	}
}

void HAL_QSPI_CmdCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	CmdCplt++;
}

void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	TxCplt++;
}

void AMOLED_Block_Write(int Xstart, int Xend, int Ystart, int Yend)
{
	qspi_111_cmd_write(0x2A, (uint8_t[]){Xstart>>8, Xstart&0xff, Xend>>8, Xend&0xff}, 4);
	qspi_111_cmd_write(0x2B, (uint8_t[]){Ystart>>8, Ystart&0xff, Yend>>8, Yend&0xff}, 4);
	//qspi_111_cmd_write(0x2C, 0, 0);
}


//清屏函数   
//全屏显示单色
void AMOLED_Clear(long int color)
{  	
	
	AMOLED_Block_Write(0, COL - 1, 0, ROW - 1);//set AMOLED GRAM 	
	
	for(int i = 0; i < COL * ROW; i += 2)
	{
		OLED_Buffer[i] 	   = color>>8; //HIGH_8bits
		OLED_Buffer[i + 1] = color & 0xff;
	}
	
	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer, Buffer_Size);//HIGH_8bits
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer, Buffer_Size);//LOW_8bits
}

//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void AMOLED_Fill(int xsta,int ysta,int xend,int yend,long int color)
{ 
	long int buffer_temp; 
	
	buffer_temp=(yend-ysta+1)*(xend-xsta+1);
	
	AMOLED_Block_Write(xsta,xend,ysta,yend);
 
		for(int i = 0; i <buffer_temp; i += 2)
			{
			OLED_Buffer[i] 	   = color>>8;
			OLED_Buffer[i + 1] = color & 0xff;			
			}	
			
	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer,buffer_temp);
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer,buffer_temp);	

} 


void Color_bar(void)//彩色条
{
int j=0;  
long int buffer_temp; 
	
int color[4]={0xf800,0x07e0,0x001f,0xffff};	

buffer_temp=(COL*ROW)/4;

for(j = 0; j <4; j++)
  {
	  AMOLED_Block_Write(0,COL-1,(ROW/4)*j,(ROW/4)*(j+1));
		for(int i = 0; i <buffer_temp; i += 2)
			{
			OLED_Buffer[i] 	   = color[j]>>8;
			OLED_Buffer[i + 1] = color[j] & 0xff;				
			}
	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer,buffer_temp);
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer,buffer_temp);				
	}

}

void Gray_bar_H16(void)//灰阶横条16
{
int j=0;  
long int buffer_temp; 

int Gray_strip[16]={
						0x0000,0x1082,0x2104,0x3186,0x4208,0x528a,0x630c,0x738e,
						0x8410,0x9492,0xa514,0xb596,0xc618,0xd69a,0xef7d,0xffff
						};

buffer_temp=(COL*ROW)/16;

for(j = 0; j <16; j =j+1)
  {
	  AMOLED_Block_Write(0,COL-1,(ROW/16)*j,(ROW/16)*(j+1));
		for(int i = 0; i <buffer_temp; i += 2)
			{
			OLED_Buffer[i] 	   = Gray_strip[j]>>8;
			OLED_Buffer[i + 1] = Gray_strip[j] & 0xff;				
			}
			
	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer,buffer_temp);
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer,buffer_temp);			
	}

}


void Gray_bar_V16(void)//灰阶竖条16
{
int j=0;  
long int buffer_temp; 

int Gray_strip[16]={
						0x0000,0x1082,0x2104,0x3186,0x4208,0x528a,0x630c,0x738e,
						0x8410,0x9492,0xa514,0xb596,0xc618,0xd69a,0xef7d,0xffff
						};

buffer_temp=(COL*ROW)/16;

for(j = 0; j <16; j++)
  {
	  AMOLED_Block_Write((COL/16)*j,(COL/16)*(j+1)-1,0,ROW-1);
		
		for(int i = 0; i <buffer_temp; i += 2)
			{
			OLED_Buffer[i] 	   = Gray_strip[j]>>8;
			OLED_Buffer[i + 1] = Gray_strip[j] & 0xff;				
			}		
	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer,buffer_temp);
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer,buffer_temp);			
	}

}


void AMOLED_DrawPoint(int x,int y,long int frontcolor)//画点-----PEN_COLOR:此点的颜色
{
	if(x>=COL)
	   x=COL-1;
  if(y>=ROW)
     y=ROW-1;

	AMOLED_Block_Write(x,x,y,y);

	OLED_Buffer[0] 	   = frontcolor >> 8;
	OLED_Buffer[1]     = frontcolor & 0xff;			

	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer, 2);
	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer, 2);			
}



void AMOLED_Draw_2x2point(int x,int y,long int frontcolor)//画一个大点(2*2的点)
{
  AMOLED_DrawPoint(x,y,frontcolor);//中心点 
	AMOLED_DrawPoint(x+1,y,frontcolor);
	AMOLED_DrawPoint(x,y+1,frontcolor);
	AMOLED_DrawPoint(x+1,y+1,frontcolor);	
  	
}
void AMOLED_Draw_4x4point(int x,int y,long int frontcolor)//画一个大点(2*2的点)
{
	//AMOLED_Fill(x,x+3,y,y+3,PEN_COLOR);
	
//	AMOLED_Block_Write(x,x+3,y,y+3);

//	OLED_Buffer[0] 	   = PEN_COLOR >> 8;
//	OLED_Buffer[1]     = PEN_COLOR & 0xff;			

//	qspi_114_cmd_write(0x2c, (unsigned char*)OLED_Buffer, 16);
//	qspi_114_cmd_write(0x3c, (unsigned char*)OLED_Buffer, 16);		

	int i,j;
	int X,Y;		
	
	if(x>=1)X=x-1;
	else X=x;
	if(y>=1)Y=y-1;
	else Y=y;
	
	for(i=Y;i<y+3;i++)
			{
			for(j=X;j<x+3;j++)AMOLED_DrawPoint(j,i,frontcolor);
			}  						   

}

//画大点函数
//x0,y0:坐标
//color:颜色
//以(x0,y0)为中心,画一个9个点的大点
void AMOLED_Draw_3x3point(int x0,int y0)
{
	int i,j;
	int x,y;		
	
	if(x0>=1)x=x0-1;
	else x=x0;
	if(y0>=1)y=y0-1;
	else y=y0;
	
	for(i=y;i<y0+2;i++)
			{
			for(j=x;j<x0+2;j++)AMOLED_DrawPoint(j,i,PEN_COLOR);
			}  						   
}


void AMOLED_DrawLine(int x1, int y1, int x2, int y2)//画线---x1,y1:起点坐标---x2,y2:终点坐标  
{
    int t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1;              //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0)incx = 1;       //设置单步方向
    else if (delta_x == 0)incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )    //画线输出
    {
        AMOLED_DrawPoint(uRow, uCol,PEN_COLOR); //画点
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
		
}



//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void AMOLED_Draw_hline(int x0,int y0,int len,long int color)
{
	if((len==0)||(x0>(COL-1))||(y0>(ROW-1)))return;
	PEN_COLOR=color;
  AMOLED_DrawLine(x0,y0,x0+len-1,y0);//	AMOLED_Fill(x0,y0,x0+len-1,y0,color);	
}


//填充椭圆
//x0,y0:坐标
//rx:x方向半径
//ry:y方向半径
//color:椭圆的颜色
void AMOLED_Fill_ellipse(int x0,int y0,int rx,int ry,long int color) 
{
	uint32_t OutConst, Sum, SumY;
	uint16_t x,y;
	uint32_t _rx = rx;
	uint32_t _ry = ry;
	OutConst = _rx*_rx*_ry*_ry+(_rx*_rx*_ry>>1);  // Constant as explaint above  
	// To compensate for rounding 
	x = rx;
	for (y=0; y<=ry; y++) 
	{
		SumY =((uint32_t)(rx*rx))*((uint32_t)(y*y)); // Does not change in loop  
		while (Sum = SumY + ((uint32_t)(ry*ry))*((uint32_t)(x*x)),(x>0) && (Sum>OutConst))x--;    
		  AMOLED_Draw_hline(x0-x,y0+y,2*x,color);
		if(y)AMOLED_DrawLine(x0-x,y0-y,x0,y0);    
      AMOLED_Draw_hline(x0-x,y0-y,2*x,color);    
	}
}


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void AMOLED_Draw_Circle(int x0,int y0,int r)
{
	int a,b;
	int di;
	a=0;
	b=r;
	//di=3-2*r;   //判断下个点位置的标志
  di=3-(r<<1); //判断下个点位置的标志
	while(a<=b)
	{
		AMOLED_DrawPoint(x0-b,y0-a,PEN_COLOR);             //3           
		AMOLED_DrawPoint(x0+b,y0-a,PEN_COLOR);             //0           
		AMOLED_DrawPoint(x0-a,y0+b,PEN_COLOR);             //1       
		AMOLED_DrawPoint(x0-b,y0-a,PEN_COLOR);             //7           
		AMOLED_DrawPoint(x0-a,y0-b,PEN_COLOR);             //2             
		AMOLED_DrawPoint(x0+b,y0+a,PEN_COLOR);             //4               
		AMOLED_DrawPoint(x0+a,y0-b,PEN_COLOR);             //5
		AMOLED_DrawPoint(x0+a,y0+b,PEN_COLOR);             //6 
		AMOLED_DrawPoint(x0-b,y0+a,PEN_COLOR);             
		a++;
		/***使用Bresenham算法画圆**/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		AMOLED_DrawPoint(x0+a,y0+b,PEN_COLOR);
	}
} 

//画一个触摸点
//用来校准用的
void AMOLED_Draw_Touch_Point(int x,int y)
{
	AMOLED_DrawLine(x-12,y,x+13,y);//横线
	AMOLED_DrawLine(x,y-12,x,y+13);//竖线
	AMOLED_DrawPoint(x+1,y+1,PEN_COLOR);
	AMOLED_DrawPoint(x-1,y+1,PEN_COLOR);
	AMOLED_DrawPoint(x+1,y-1,PEN_COLOR);
	AMOLED_DrawPoint(x-1,y-1,PEN_COLOR);
	AMOLED_Draw_Circle(x,y,6);//画中心圈
}	

//在指定位置画一个指定大小的实心圆
//(x,y):中心点
//r    :半径
void AMOLED_Draw_Donut(int x0,int y0,int r)
{
	int r0;

  for(r0=r;r0>0;r0--)//	
  {
	AMOLED_Draw_Circle(x0,y0,r0);	
   } 
  
} 

static void AMOLED_DrawHLine(int x0, int y,  int x1)
{
    for (;x0 <= x1; x0++)
	     {
        AMOLED_DrawPoint(x0,y,PEN_COLOR);
       }
}

void AMOLED_Fill_Circle(int x0, int y0, int r)
{
  signed long i;
  int imax = ((signed long)((signed long)r*707))/1000+1;
  signed long sqmax = (signed long)r*(signed long)r+(signed long)r/2;
  signed long x=r;
  AMOLED_DrawHLine(x0-r,y0,x0+r);
  for (i=1; i<= imax; i++) {
    if ((i*i+x*x) >sqmax) {
      /* draw lines from outside */
      if (x>imax) {
        AMOLED_DrawHLine (x0-i+1,y0+x, x0+i-1);
        AMOLED_DrawHLine (x0-i+1,y0-x, x0+i-1);
      }
      x--;
    }
    /* draw lines from inside (center) */
    AMOLED_DrawHLine(x0-x,y0+i, x0+x);
    AMOLED_DrawHLine(x0-x,y0-i, x0+x);
  }
 
}

void AMOLED_Fill_Ellipse(int x0, int y0, int rx, int ry)
{
  signed long OutConst, Sum, SumY;
  int x,y;
  unsigned long _rx = rx;
  unsigned long _ry = ry;
  OutConst = _rx*_rx*_ry*_ry  /* Constant as explaint above */
            +(_rx*_rx*_ry>>1); /* To compensate for rounding */
  x = rx;
  for (y=0; y<=ry; y++) {
    SumY =((signed long)(rx*rx))*((signed long)(y*y)); /* Does not change in loop */
    while (Sum = SumY + ((signed long)(ry*ry))*((signed long)(x*x)),
           (x>0) && (Sum>OutConst))
    {
      x--;
    }
    AMOLED_DrawHLine(x0-x, y0+y, x0+x);
    if (y)
      AMOLED_DrawHLine(x0-x, y0-y, x0+x);
  }

}

/*********************************************************************
*
*   DrawEllipse
*/
void AMOLED_Draw_Ellipse(int x0, int y0, int rx, int ry)
{
  signed long OutConst, Sum, SumY;
  int x,y;
  int xOld;
  unsigned long _rx = rx;
  unsigned long _ry = ry;
  OutConst = _rx*_rx*_ry*_ry  /* Constant as explaint above */
            +(_rx*_rx*_ry>>1); /* To compensate for rounding */
  xOld = x = rx;
  for (y=0; y<=ry; y++) {
    if (y==ry) {
      x=0;
    } else {
      SumY =((signed long)(rx*rx))*((signed long)(y*y)); /* Does not change in loop */
      while (Sum = SumY + ((signed long)(ry*ry))*((signed long)(x*x)),
             (x>0) && (Sum>OutConst)) x--;
    }
    /* Since we draw lines, we can not draw on the first
        iteration
    */
    if (y) {
      AMOLED_DrawLine(x0-xOld,y0-y+1,x0-x,y0-y);
      AMOLED_DrawLine(x0-xOld,y0+y-1,x0-x,y0+y);
      AMOLED_DrawLine(x0+xOld,y0-y+1,x0+x,y0-y);
      AMOLED_DrawLine(x0+xOld,y0+y-1,x0+x,y0+y);
    }
    xOld = x;
  }
}


/*---------------------------------------------------------------------------
以下函数正常使用即可，无需修改
---------------------------------------------------------------------------*/
//在指定位置显示一个字符
//x:0~COL-6
//y:0~ROW-12
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void AMOLED_ShowChar(int x,int y,int num,int size,int mode)
{   
  int temp,t1,t;  
	int y0=y;
	long int csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
			if     (size==12)temp=OLED_asc_1206[num][t];//调用1206字体
			else if(size==16)temp=OLED_asc_1608[num][t];//调用1608字体
			else if(size==24)temp=OLED_asc_2412[num][t];//调用2412字体
      else if(size==32)temp=OLED_asc_3216[num][t];//调用3216字体		
			else if(size==48)temp=OLED_asc_4824[num][t];//调用4824字体
			else if(size==60)temp=OLED_BIG_ASCII[num][t];//调用30*60 NUMBER 0~9
		  else return;  //没有的字库
		
		for(t1=0;t1<8;t1++)
		{			    
			if     (temp&0x80) AMOLED_DrawPoint(x,y,PEN_COLOR);
			else if(mode==0)   AMOLED_DrawPoint(x,y,PAPER_COLOR);
			temp<<=1;
			y++;
			if(y>=(ROW-1))return;		//超区域了
			if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=(ROW-1))return;	//超区域了					
					break;
				}
		}  	 
	}  

}

//在指定位置显示一个字符
//x:0~COL-6
//y:0~ROW-12
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void AMOLED_Show2x2Char(int x,int y,int num,int size,int mode)
{   
  int temp,t1,t;  
	int y0=y;
	long int csize=(size/8+((size%8)?1:0))*(size/2)/4;		//得到字体一个字符对应点阵集所占的字节数	
	
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
			if     (size==24)temp=OLED_asc_1206[num][t];//调用1206字体
			else if(size==32)temp=OLED_asc_1608[num][t];//调用1608字体
			else if(size==48)temp=OLED_asc_2412[num][t];//调用2412字体
      else if(size==64)temp=OLED_asc_3216[num][t];//调用3216字体		
			else if(size==96)temp=OLED_asc_4824[num][t];//调用4824字体
			else if(size==120)temp=OLED_BIG_ASCII[num][t];//调用30*60 NUMBER 0~9
		  else return;  //没有的字库
		
		for(t1=0;t1<8;t1++)
		{			    
			if     (temp&0x80) AMOLED_Draw_2x2point(x,y,PEN_COLOR);
			else if(mode==0)   AMOLED_Draw_2x2point(x,y,PAPER_COLOR);
			temp<<=1;
			y=y+2;
			if(y>=(ROW-1))return;		//超区域了
			if((y-y0)==size)
				{
					y=y0;
					x=x+2;
					if(x>=(ROW-1))return;	//超区域了					
					break;
				}
		}  	 
	}  

}

void AMOLED_Show4x4Char(int x,int y,int num,int size,int mode)
{   
  int temp,t1,t;  
	int y0=y;
	long int csize=(size/8+((size%8)?1:0))*(size/2)/16;		//得到字体一个字符对应点阵集所占的字节数	
	
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
			if     (size==48)temp=OLED_asc_1206[num][t];//调用1206字体
			else if(size==64)temp=OLED_asc_1608[num][t];//调用1608字体
			else if(size==96)temp=OLED_asc_2412[num][t];//调用2412字体
      else if(size==128)temp=OLED_asc_3216[num][t];//调用3216字体		
			else if(size==192)temp=OLED_asc_4824[num][t];//调用4824字体
			else if(size==240)temp=OLED_BIG_ASCII[num][t];//调用30*60 NUMBER 0~9
		  else return;  //没有的字库
		
		for(t1=0;t1<8;t1++)
		{			    
			if     (temp&0x80) AMOLED_Draw_4x4point(x,y,PEN_COLOR);
			else if(mode==0)   AMOLED_Draw_4x4point(x,y,PAPER_COLOR);
			temp<<=1;
			y=y+4;
			if(y>=(ROW-1))return;		//超区域了
			if((y-y0)==size)
				{
					y=y0;
					x=x+4;
					if(x>=(ROW-1))return;	//超区域了					
					break;
				}
		}  	 
	}  

}


/*---------------------------------------------------------------------------
以下函数正常使用即可，无需修改
---------------------------------------------------------------------------*/
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void ShowString(int x,int y,const char *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;AMOLED_Clear(PAPER_COLOR);}
        AMOLED_ShowChar(x,y,*p,16,0);
        x+=8;
        p++;
    }  
}


/*---------------------------------------------------------------------------
以下函数正常使用即可，无需修改
---------------------------------------------------------------------------*/
//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);
void AMOLED_ShowNum(int x,int y,int num,int size,int mode)
{         							   
    AMOLED_ShowChar(x,y,(num/10)%10+'0',size,mode); 
    AMOLED_ShowChar(x+size/2,y,num%10+'0',size,mode); 
}

void AMOLED_Show2x2Num(int x,int y,int num,int size,int mode)
{         							   
    AMOLED_Show2x2Char(x,y,(num/10)%10+'0',size,mode); 
    AMOLED_Show2x2Char(x+size/2,y,num%10+'0',size,mode); 
}

void AMOLED_Show4x4Num(int x,int y,int num,int size,int mode)
{         							   
    AMOLED_Show4x4Char(x,y,(num/10)%10+'0',size,mode); 
    AMOLED_Show4x4Char(x+size/2,y,num%10+'0',size,mode); 
}

/*---------------------------------------------------------------------------
以下函数正常使用即可，无需修改
---------------------------------------------------------------------------*/
//显示4个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~9999);
void AMOLED_Show4Num(int x,int y,int num,int size,int mode)
{ 
  
AMOLED_ShowChar(x,y,(num/1000)+'0',size,mode);
AMOLED_ShowChar(x+size/2,y,(num/100)%10+'0',size,mode);      							   
AMOLED_ShowChar(x+size,y,(num/10)%10+'0',size,mode); 
AMOLED_ShowChar(x+size+size/2,y,num%10+'0',size,mode); 
}





//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void AMOLED_Show_Str(int x,int y,int size,unsigned char*str,long int frontcolor)
{
		int bHz=0;     //字符或者中文  	 
		//int size=16;
		int mode=0;
	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(COL-size/2))//换行
									{				   
										y+=size;
										x=0;	   
									}							    
		        if(y>(ROW-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=0;
		            str++; 
		        }  
		        else AMOLED_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }					 
    }  
		
		
}  			 		  



//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void AMOLED_Disp_Str(int x,int y,int size,unsigned char* str)
{
		int bHz=0;     //字符或者中文  	 
		int mode=1;
	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
            if(x>(COL-size/2))//换行
							{				   
							y+=size;
							x=0;	   
							}							    
		   
				    if(y>(ROW-size))break;//越界返回    
		        if(*str==13)//换行符号
							{         
							y+=size;
							x=0;
							str++; 
							}  
		        else AMOLED_ShowChar(x,y,*str,size,mode);//有效部分写入 
				    str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }	    
       					 
    }  
		
		
} 


//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void AMOLED_Disp_SCALE2X2_Str(int x,int y,int size,unsigned char* str)
{
		int bHz=0;     //字符或者中文  	 
		int mode=1;
	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
            if(x>(COL-size/2))//换行
							{				   
							y+=size;
							x=0;	   
							}							    
		   
				    if(y>(ROW-size))break;//越界返回    
		        if(*str==13)//换行符号
							{         
							y+=size;
							x=0;
							str++; 
							}  
		        else AMOLED_Show2x2Char(x,y,*str,size,mode);//有效部分写入 
				    str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }	    
       					 
    }  
		
		
} 

//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void AMOLED_Disp_SCALE4X4_Str(int x,int y,int size,unsigned char* str)
{
		int bHz=0;     //字符或者中文  	 
		int mode=1;
	
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
            if(x>(COL-size/2))//换行
							{				   
							y+=size;
							x=0;	   
							}							    
		   
				    if(y>(ROW-size))break;//越界返回    
		        if(*str==13)//换行符号
							{         
							y+=size;
							x=0;
							str++; 
							}  
		        else AMOLED_Show4x4Char(x,y,*str,size,mode);//有效部分写入 
				    str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }	    
       					 
    }  
		
		
} 



