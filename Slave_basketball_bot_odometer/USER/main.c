#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h"
#include "stm32f10x.h"
#include "BSP.h"
#include "myextern.h"
    				
//ALIENTEK战舰STM32开发板实验25
//CAN 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  	
 u8 flag1=0;
System_flag System;  // 系统信息
Encoder my_encoder1;  //编码器参数，包括速度等
Encoder my_encoder2;
Encoder my_encoder3;


/*********************************************************************10ms 中断产生一次
此函数作用
系统初始化部分
*****************************************************************************************/
void System_Init(void)
{
	
	System.S_Initialize=0;
    BSP_Init();
	LED_Init();
    NVIC_Configuration();
	System.S_Initialize= 1;

}

 int main(void)
 {	 
  System_Init();
 	while(1)
	{
		//Can_Send_Msg(A,8);
		//LED1=!LED1;
		delay_ms(500);
	}
}



