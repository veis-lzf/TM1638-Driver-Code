#ifndef __TM1638_H
#define __TM1638_H

#include <sys.h>
#include <delay.h>

#define DIO_IN()  {GPIOC->MODER&=~(3<<(13*2));GPIOC->MODER|=0<<13*2;}		//PC13输入模式
#define DIO_OUT() {GPIOC->MODER&=~(3<<(13*2));GPIOC->MODER|=1<<13*2;} 	//PC13输出模式 

#define DI PCin(13)				//Data Input
#define DO PCout(13)			//Data Output

#define CLK PCout(15)			//Serial Clock Port

#define STB PCout(14)			//Chip Selection
#define CS STB						//Redefine the Chip Selection's Name

#define	DATA_COMMAND	0X40									
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0



void tm1638_init(void);			//tm1638 Initialize
void write_byte(unsigned char data);	//write one byte to the data bus
unsigned char read_byte(void);	//read one byte from the data bus
void write_allLED(unsigned char LED_flag);//write the data to LED port
void write_data(unsigned char add,unsigned char data);//write the data to the address which the user set
unsigned char read_key(void);//read the value of key to confirm which key be pressed by user
void write_com(unsigned char cmd);//write the command to set the chip

#endif

