#ifndef __bsp_H
#define	__bsp_H

#include "stm32f10x.h"
#include "myextern.h"
#include <stdio.h>
#include "string.h"	 
#include <stdarg.h>
#include "stm32f10x_tim.h"
#define MAX_COUNT  700    //下发速度0.6时 电机最大转速 506//0.8电机最大转速 689
#define TIM_PERIOD 1500   // TIM_PERIOD>2*MAX_COUNT


#define MYID  0X03
#define MYID1 0X01
#define MYID2 0X02
#define MYID3 0X03

#define UID  0X10
#define CAN_BAUDRATE	500     //1M波特率



#define encoder_data1 55200//32000//55197          // 1号轮1圈编码器的脉冲数
#define encoder_data2 55200//32000//55204       // 2号轮1圈编码器的脉冲数
#define encoder_data3 55200//32000//55103       // 3号轮1圈编码器的脉冲数





//#define encoder_data1 13795           // 1号轮1圈编码器的脉冲数
//#define encoder_data2 13379           // 2号轮1圈编码器的脉冲数
//#define encoder_data3 13774           // 3号轮1圈编码器的脉冲数

//bsp.c
void NVIC_Configuration(void) ;

// delay.c
void delay_ms(unsigned int t);
void delay_us(unsigned int t);
void SysTick_Init(void);
float GET_Nowtime(void);//返回当前systick计数器值,24位
float GET_microstime(float * lasttime);

//usart.c
void usart_init(USART_TypeDef* USARTx,u32 bound);

//can.c
void Can_response(unsigned char c);
void Can_response_All(void);
void encoder_analysis(void);

void BSP_Init(void);

void clear(void);
//lcd.c
int32_t   GetCount1(void);
int32_t   GetCount2(void);
int32_t   GetCount3(void);
void my_encoder_analysis(void);
#endif
