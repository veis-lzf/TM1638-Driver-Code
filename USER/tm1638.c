#include "tm1638.h"

void tm1638_init()
{
	unsigned char i;

	 //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	write_com(0x8a);       //亮度 (0x88-0x8f)8级亮度可调
	write_com(0x40);       //采用地址自动加1
	STB = 0;		           
	write_byte(0xc0);    //设置起始地址
	for(i = 0; i < 16; i++)	   //传送16个字节的数据
		write_byte(0x00);
	STB = 1; 
}

void write_byte(unsigned char data)
{
	unsigned char i = 0;
	DIO_OUT();
	for(i = 0;i < 8; i++)
	{
		CLK = 0;
		if(data & 0X01)
		{
			DO = 1;
		}
		else
		{
			DO = 0;
		}
		delay_us(500);
		data >>= 1;
		CLK = 1;
		delay_us(500);
	}
	
}

unsigned char read_byte()
{
	unsigned char data = 0;
	unsigned char i = 0;
	DIO_IN();
	for(i = 0; i < 8; i++)
	{
		data >>= 1;
		CLK=0;
		delay_us(500);
		if(DI)
			data |= 0x80;
		CLK=1;
		delay_us(500);
	}
	return data;
}

void write_com(unsigned char cmd)		//发送命令字
{
	DIO_OUT();
	STB = 0;
	delay_us(500);
	write_byte(cmd);
	STB = 1;
	delay_us(500);
}

unsigned char read_key(void)
{
	unsigned char c[4], i, key_value = 0;
	STB = 0;
	write_byte(0x42);		           //读键扫数据 命令
	delay_ms(10);
	for(i = 0;i < 4; i++)	
	{	
		c[i] = read_byte();
	}
	STB = 1;					           //4个字节数据合成一个字节
	for(i = 0;i < 4; i++)
		key_value |= c[i] << i;
	for(i = 0; i < 8; i++)
		if((0x01 << i) == key_value)
			break;
	return i;
}

void write_data(unsigned char add,unsigned char data)		//指定地址写入数据
{
	write_com(0x44);
	delay_us(500);
	STB = 0;
	delay_us(500);
	write_byte(0xc0 | add);
	write_byte(data);
	STB = 1;
}

void write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i = 0; i < 8; i++)
		{
			delay_us(500);
			if(LED_flag & (1 << i))
				write_data(2*i + 1,1);
			else
				write_data(2*i + 1,0);
		}
}

