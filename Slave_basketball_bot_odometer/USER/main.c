#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h"
#include "stm32f10x.h"
#include "BSP.h"
#include "myextern.h"
    				
//ALIENTEKս��STM32������ʵ��25
//CAN ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  	
 u8 flag1=0;
System_flag System;  // ϵͳ��Ϣ
Encoder my_encoder1;  //�����������������ٶȵ�
Encoder my_encoder2;
Encoder my_encoder3;


/*********************************************************************10ms �жϲ���һ��
�˺�������
ϵͳ��ʼ������
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



