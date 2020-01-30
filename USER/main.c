#include "usart.h"
#include "tm1638.h"

unsigned char tab[] = 
{
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
	0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};

unsigned char num[8];		//各个数码管显示的值

int main(void)
{
	unsigned char i;
	
	uart_init(115200);
	delay_init(84);
	tm1638_init();
	for(i = 0; i < 8; i++)
	{
		write_data(i << 1,tab[0]);	               //初始化寄存器
		delay_us(500);
	}
  while(1)
	{
		i = read_key();                          //读按键值
		if(i<8)
		{
			num[i]++;
			while(read_key() == i);		       //等待按键释放
			if(num[i] > 15)
				num[i] = 0;
			write_data(i * 2,tab[num[i]]);
			write_allLED(1 << i);
		}
	}
}



