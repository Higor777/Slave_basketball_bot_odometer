#include "BSP.h"

int16_t time=0;
 
void delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=10300;
 		while(a--);
	}
}

void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=9;
		while(a--);
	}
}

//SYSTICK的时钟固定为HCLK时钟的1/8
void SysTick_Init(void)
{

	SysTick->CTRL&=0xfffffffb;  //bit2清空,选择外部时钟  HCLK/8
	SysTick->LOAD=1000000;      // 1/9s
	SysTick->VAL =0x00;        //清空计数器
	SysTick->CTRL=0x03;       //开始倒数 
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  

}


void SysTick_Handler(void) // 1/9s
{
	
    time++;
		if(time>=1000)
		{
		  time=0;
		}
		
}

float GET_Nowtime(void)//返回当前systick计数器值,24位
{
	
	float temp; 
	temp=(float)(1000000-SysTick->VAL+1000000*time)/9000000;	       //得到当前系统时钟的计数   时间=TIM5->CNT+65536*time/1000000			 1m的时钟
	return temp;
	
}


float GET_microstime(float * lasttime)
{
	float temp1,temp2;
	temp1 =(float)(1000000-SysTick->VAL+1000000*time)/9000000;
	temp2 =temp1 - *lasttime;
	*lasttime = temp1;
	if(temp2<0)
	{
      return  ((1000000 *1000/9000000-*lasttime)+temp1);
  }	
	return temp2;
}



