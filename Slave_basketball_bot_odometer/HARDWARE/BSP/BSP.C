#include "BSP.h"
#include "stm32f10x_can.h"
#include "can.h"
#include "usart.h"	  
static  void RCC_Init(void)
{  	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	   //很重要的一条必须加上复用功能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
 
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 		
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

}


static void TIM_Configuration(void)
{
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2); 
	
    TIM_TimeBaseStructure.TIM_Period=4999;		 								/* 自动重装载寄存器周期的值(计数值) 5ms  200hz */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* 开启时钟 */    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/*先关闭等待使用*/    

}





/************************* TIM3编码器模式*******************************/

#define ENCODER_PPR           (u16)(1)   // number of pulses per revolution 
#define ICx_FILTER            (u8)1      // 6<-> 670nsec



void ENCODE_Init(void)	
{
	
		 /******************************TIM3 CHI1 CHI2---PA6 PA7********************************/
	
	  TIM_ICInitTypeDef TIM_ICInitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	
  //  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

	  GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  	TIM_DeInit(TIM3);
  	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);	
  	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  
  	TIM_TimeBaseStructure.TIM_Period =TIM_PERIOD-1;    //(4*ENCODER_PPR)-1   ;  	 	
 	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	
  	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//编码器模式设置3
  	TIM_ICStructInit(&TIM_ICInitStructure);
  	TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;//配置输入滤波器
  	TIM_ICInit(TIM3, &TIM_ICInitStructure);
  	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  	TIM_Cmd(TIM3, ENABLE); 
		
/******************************TIM4 CHI1 CHI2---PB6 PB7********************************/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		TIM_DeInit(TIM4);
  	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    
  	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;   
  	TIM_TimeBaseStructure.TIM_Period =  TIM_PERIOD-1;//(4*ENCODER_PPR)-1 ;  	 	
 	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

  	TIM_ICStructInit(&TIM_ICInitStructure);
  	TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;
  	TIM_ICInit(TIM4, &TIM_ICInitStructure); 
  	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  	//TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  	TIM_Cmd(TIM4, ENABLE); 		
		
/******************************TIM5 CHI1 CHI2---PA0 PA1********************************/
		GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		TIM_DeInit(TIM5);
  	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;   
  	TIM_TimeBaseStructure.TIM_Period = TIM_PERIOD-1;//(4*ENCODER_PPR)-1 ;  	 	
 	  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
 
  	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

  	TIM_ICStructInit(&TIM_ICInitStructure);
  	TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;
  	TIM_ICInit(TIM5, &TIM_ICInitStructure); 
  	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
  //	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  	TIM_Cmd(TIM5, ENABLE);
}


void NVIC_Configuration(void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;     //通道设置为串口1中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	  //中断响应优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		    //打开中断
		NVIC_Init(&NVIC_InitStructure); 

		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;       //  TIM3编码器模式
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
  	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		
  	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	
	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&NVIC_InitStructure);

   				
}

extern System_flag System;  // 系统各标志
void BSP_Init(void)
{
	
	RCC_Init();		        //所有时钟使能
	SysTick_Init();	      //系统时钟初始化
	ENCODE_Init();	      //  TIM3编码器模式
  TIM_Configuration();  //TIM2主中断
 // CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_3tq,6,CAN_Mode_Normal);         //CAN初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_3tq,6,CAN_Mode_Normal);         //CAN初始化
	
	uart_init(115200);
  delay_ms(300);	
}


