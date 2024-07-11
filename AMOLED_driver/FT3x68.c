#include "FT3x68.h"
#include "amoled_driver.h"

//向FT3X68写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t FT3X68_WR_Reg(int reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	I2C_Start();	 
	I2C_Send_Byte(FT3X68_CMD_WR);	//发送写地址 
	I2C_Wait_Ack(); 
	
	I2C_Send_Byte(reg&0XFF);   	//发送低8位地址
	I2C_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    I2C_Send_Byte(buf[i]);  	//发数据
		ret=I2C_Wait_Ack();
		if(ret)break;  
	}
    I2C_Stop();					//产生一个停止条件	    
	return ret; 
}
//从FT3X68读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT3X68_RD_Reg(int reg,uint8_t *buf,uint8_t len)
{
	uint8_t i; 
 	I2C_Start();	
 	I2C_Send_Byte(FT3X68_CMD_WR);   	//发送写命令 	 
	I2C_Wait_Ack(); 	 	
	
 	I2C_Send_Byte(reg&0XFF);   	//发送低8位地址
	I2C_Wait_Ack();  
	
 	I2C_Start();  	 	   
	I2C_Send_Byte(FT3X68_CMD_RD);   	//发送读命令		   
	I2C_Wait_Ack();	 
  
	for(i=0;i<len;i++)
	{	   
  buf[i]=I2C_Read_Byte(i==(len-1)?0:1); //发数据
	} 
    I2C_Stop();//产生一个停止条件     
}

int fts_read(uint8_t addr, uint8_t *data, uint16_t len)
{
	uint8_t i; 
 	I2C_Start();	
 	I2C_Send_Byte(FT3X68_CMD_WR);   	//发送写命令 	 
	I2C_Wait_Ack(); 	 										  		   
 	I2C_Send_Byte(addr&0XFF);   	//发送低8位地址
	I2C_Wait_Ack();  
 	I2C_Start();  	 	   
	I2C_Send_Byte(FT3X68_CMD_RD);   	//发送读命令		   
	I2C_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
  data[i]=I2C_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    I2C_Stop();//产生一个停止条件     
}

//初始化FT3X68触摸屏
//返回值:0,初始化成功;1,初始化失败 
uint8_t FT3X68_Init(void)
{
	uint8_t temp[10];
	I2C_Init();      	//初始化电容屏的I2C总线  
	
	FT3X68_RST_H;				//复位
	HAL_Delay(1);	
	FT3X68_RST_L;				//复位
	HAL_Delay(20);
 	FT3X68_RST_H;				//释放复位
	HAL_Delay(50);  	

  printf("FT3X68_Initial...\r\n",((int)temp[0]<<8)+temp[1]);	
	temp[0]=0;
	FT3X68_WR_Reg(FT3X68_DEVIDE_MODE,temp,1);	//进入正常操作模式 	
	
	temp[0]=0;
	FT3X68_WR_Reg(FT3X68_ID_G_MODE,temp,1);		//查询模式 
	
	temp[0]=22; //触摸有效值，22，越小越灵敏	
	FT3X68_WR_Reg(FT3X68_ID_G_THGROUP,temp,1);	//设置触摸有效值
	
	temp[0]=12; //激活周期，不能小于12，最大14	
	FT3X68_WR_Reg(FT3X68_ID_G_PERIODACTIVE,temp,1); 
	
	//读取版本号，参考值：0x3003
	FT3X68_RD_Reg(FT3X68_Chip_vendor_ID,&temp[0],2);  
	printf("FT3X68 vendor ID:0x%x\r\n",((int)temp[0]<<8)+temp[1]);
	if(temp[0]==0X30&&temp[1]==0X03)//版本:0X3003
	{
		printf("Chip vendor ID:0x%x\r\n",((int)temp[0]<<8)+temp[1]);
		return 0;
	} 
	return 0;
}

const int FT3X68_TPX_TBL[5]={FT3X68_TP1_REG,FT3X68_TP2_REG,FT3X68_TP3_REG,FT3X68_TP4_REG,FT3X68_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t FT3X68_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	int res=0;
	uint8_t temp,temp0;
	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
		FT3X68_RD_Reg(FT3X68_TD_STATUS,&mode,1);//读取触摸点的状态  
		//printf("mode: %2x\r\n",((mode>>4)&0X0F));//mode);
    //printf("mode:0x%04x\r\n",mode);
		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					FT3X68_RD_Reg(FT3X68_TPX_TBL[i],buf,4);	//读取XY坐标值
					tp_dev.x[i]=((int)(buf[0]&0X0F)<<8)+buf[1];	
					tp_dev.y[i]=((int)(buf[2]&0X0F)<<8)+buf[3];	
				if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//必须是contact事件，才认为有效
				}
			}
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据	
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X1F)==0)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}
		else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}

