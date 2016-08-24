#include "led.h"
#include "stm32f10x_it.h" 
#include "stm32f10x.h"
#include "string.h"  
#include "stdio.h"
#include "math.h"
#include "BSP.h"
#include "queue.h"
#include "led.h"
#include "can.h"
#include "usart.h"
extern System_flag System;   // 系统各标志
extern Encoder my_encoder1;
extern Encoder my_encoder2;
extern Encoder my_encoder3;

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PD2 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD2
 GPIO_SetBits(GPIOD,GPIO_Pin_2);						 //PD2 输出高

}
 
/*********************************************************************10ms 中断产生一次
此函数作用
对部分参数进行清零，防止累积误差过大
*****************************************************************************************/
void clear(void)			  				
{
	my_encoder1.mesure_total_encoder=0;
	my_encoder1.expect_total_encoder=0;
	my_encoder1.past_encoder=0;      //从上电开始走过的编码器的计数
	my_encoder1.past_length=0;       //从上电开始走走过的总角度   正负	  反馈给上位机 由上位机清零	

	my_encoder2.mesure_total_encoder=0;
	my_encoder2.expect_total_encoder=0;
	my_encoder2.past_encoder=0;      //从上电开始走过的编码器的计数
	my_encoder2.past_length=0;
	
	my_encoder3.mesure_total_encoder=0;
	my_encoder3.expect_total_encoder=0;
	my_encoder3.past_encoder=0;      //从上电开始走过的编码器的计数
	my_encoder3.past_length=0;
} 


/*********************************************************************10ms 中断产生一次
此函数作用
返回C编码器转动总角度
*****************************************************************************************/
void Can_response(unsigned char c)
{	
	unsigned char myID;
  CanTxMsg TxMessageBuffer;
	u8 byte[4],i;
	float b;
	if(c==1){myID=MYID1;b=my_encoder1.past_length;}
	else if(c==2){myID=MYID2;b=my_encoder2.past_length;}
	else if(c==3){myID=MYID3;b=my_encoder3.past_length;};
  TxMessageBuffer.StdId=0x00;	 
	TxMessageBuffer.ExtId=((myID<<8)|UID); //发送者ID |  接受者ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // 传输消息的帧类型为数据帧（还有远程帧）
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // 消息标志符实验标准标识符
  TxMessageBuffer.DLC=6;					   // 发送两帧，一帧8位
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3请求和回复和控制
	TxMessageBuffer.Data[1]=0x02;			 // 指令类型   回复角度
	
  //b=my_encoder.past_length;
	memcpy(byte,&b,sizeof(float));
  for(i=0;i<4;i++)
  {		
		
		TxMessageBuffer.Data[i+2]=byte[i];		
		
  }
	
	CAN_Transmit(CAN1, &TxMessageBuffer);   //把当前角速度和当前总角度发给主控板

}


/*********************************************************************10ms 中断产生一次
此函数作用
一次发送两个CAN数据帧返回三个编码器转动总角度
*****************************************************************************************/
void Can_response_All(void)
{	
  CanTxMsg TxMessageBuffer;
	u8 byte[12],i;
  TxMessageBuffer.StdId=0x00;	 
	TxMessageBuffer.ExtId=((MYID1<<8)|UID); //发送者ID |  接受者ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // 传输消息的帧类型为数据帧（还有远程帧）
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // 消息标志符实验标准标识符
  TxMessageBuffer.DLC=8;					   // 发送两帧，一帧8位
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3请求和回复和控制
	TxMessageBuffer.Data[1]=0x02;			 // 指令类型   回复角度
	memcpy(byte,&my_encoder1.past_length,4);
	memcpy(byte+4,&my_encoder2.past_length,4);
	memcpy(byte+8,&my_encoder3.past_length,4);
  for(i=0;i<6;i++)
  {		
		TxMessageBuffer.Data[i+2]=byte[i];		
  }
	CAN_Transmit(CAN1, &TxMessageBuffer);   //把当前角速度和当前总角度发给主控板
	delay_us(300);
  TxMessageBuffer.StdId=0x00;	 
	TxMessageBuffer.ExtId=((MYID2<<8)|UID); //发送者ID |  接受者ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // 传输消息的帧类型为数据帧（还有远程帧）
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // 消息标志符实验标准标识符
  TxMessageBuffer.DLC=8;					   // 发送两帧，一帧8位
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3请求和回复和控制
	TxMessageBuffer.Data[1]=0x02;			 // 指令类型   回复角度
  for(i=0;i<6;i++)
  {		
		TxMessageBuffer.Data[i+2]=byte[i+6];		
  }
	CAN_Transmit(CAN1, &TxMessageBuffer);   //把当前角速度和当前总角度发给主控板
}



/*********************************************************************10ms 中断产生一次
此函数作用
编码器测量数据处理
*****************************************************************************************/
int32_t GetCount1(void)
{
  static  u16   lastCount1 = 0;
  u16  curCount1 = TIM3->CNT;
  int32_t dAngle = curCount1 - lastCount1;
  if(dAngle >= MAX_COUNT){
    dAngle -= TIM_PERIOD;
  }else if(dAngle < -MAX_COUNT){
    dAngle += TIM_PERIOD;
  }
  lastCount1 = curCount1;
  return dAngle;
}

int32_t GetCount2(void)
{
  static  u16   lastCount2= 0;
  u16  curCount2 = TIM4->CNT;
  int32_t dAngle = curCount2 - lastCount2;
  if(dAngle >= MAX_COUNT){
    dAngle -= TIM_PERIOD;
  }else if(dAngle < -MAX_COUNT){
    dAngle += TIM_PERIOD;
  }
  lastCount2= curCount2;
  return dAngle;
}
int32_t GetCount3(void)
{
  static  u16   lastCount3 = 0;
  u16  curCount3 = TIM5->CNT;
  int32_t dAngle = curCount3 - lastCount3;
  if(dAngle >= MAX_COUNT){
    dAngle -= TIM_PERIOD;
  }else if(dAngle < -MAX_COUNT){
    dAngle += TIM_PERIOD;
  }
  lastCount3= curCount3;
  return dAngle;
}
void my_encoder_analysis(void)
{
	my_encoder1.count=GetCount1();
	if(my_encoder1.count> encoder_data1)     	 
  {
		my_encoder1.count=0;
		my_encoder1.rcount++;	  	
  }
  if(my_encoder1.count<- encoder_data1)
  {
  	my_encoder1.count=0;
 	  my_encoder1.rcount--;	  	
  }
	
	
	my_encoder2.count=GetCount2();
	
	if(my_encoder2.count> encoder_data2)     	 
  {
		my_encoder2.count=0;
		my_encoder2.rcount++;	  	
  }
  if(my_encoder2.count<- encoder_data2)
  {
  	  my_encoder2.count=0;
 	  my_encoder2.rcount--;	  	
  }
	
	my_encoder3.count=GetCount3();
	if(my_encoder3.count> encoder_data3)     	 
  {
		my_encoder3.count=0;
		my_encoder3.rcount++;	  	
  }
  if(my_encoder3.count<- encoder_data3)
  {
  	   my_encoder3.count=0;
 	   my_encoder3.rcount--;	  	
  } 
}



void encoder_analysis(void)
{
/*******************************************编码器1************************************/
	my_encoder_analysis();
	
    my_encoder1.mesure_unit_encoder = my_encoder1.rcount * encoder_data1 + my_encoder1.count;
	my_encoder1.rcount=0;
	my_encoder1.count=0;
   
	my_encoder1.past_encoder += my_encoder1.mesure_unit_encoder;                 //已经走过的编码计数		有正负
	my_encoder1.past_length = (my_encoder1.past_encoder/encoder_data1)*360;     // 已经走过的总里程 //can返回值   角度   弧度制
/* 	my_encoder1.mesure_anglev  = my_encoder1.mesure_unit_encoder * PID_hz * 360 / encoder_data1 ;            //测量的速度   单位化
	my_encoder1.mesure_total_encoder += my_encoder1.mesure_unit_encoder ;      //测定的总编码器计数  定时清0
	my_encoder1.encoder_totalt_difference = 	my_encoder1.expect_total_encoder -  my_encoder1.mesure_total_encoder ;  //差值	
	my_encoder1.encoder_unit_difference = my_encoder1.expect_unit_encoder - my_encoder1.mesure_unit_encoder; */
		
/***********************************编码器2****************************************/
	my_encoder2.mesure_unit_encoder = my_encoder2.rcount * encoder_data2 + my_encoder2.count;
	my_encoder2.rcount=0;
	my_encoder2.count=0;
  
	my_encoder2.past_encoder += my_encoder2.mesure_unit_encoder;                 //已经走过的编码计数		有正负
	my_encoder2.past_length = (my_encoder2.past_encoder/encoder_data2)*360;                              // 已经走过的总里程
/* 	my_encoder2.mesure_anglev  = my_encoder2.mesure_unit_encoder * PID_hz * 360 / encoder_data2 ;            //测量的速度   单位化
	my_encoder2.mesure_total_encoder += my_encoder2.mesure_unit_encoder ;      //测定的总编码器计数  定时清0
    my_encoder2.encoder_totalt_difference = 	my_encoder2.expect_total_encoder -  my_encoder2.mesure_total_encoder ;  //差值	
    my_encoder2.encoder_unit_difference = my_encoder2.expect_unit_encoder - my_encoder2.mesure_unit_encoder;
		 */
/*****************************************编码器3******************************************************/
	my_encoder3.mesure_unit_encoder = my_encoder3.rcount * encoder_data3 + my_encoder3.count;
	my_encoder3.rcount=0;
	my_encoder3.count=0;
	
	my_encoder3.past_encoder += my_encoder3.mesure_unit_encoder;                 //已经走过的编码计数		有正负
	my_encoder3.past_length = (my_encoder3.past_encoder/encoder_data3)*360;                              // 已经走过的总里程
/* 		my_encoder3.mesure_anglev  = my_encoder3.mesure_unit_encoder * PID_hz * 360 / encoder_data3 ;            //测量的速度   单位化
	    my_encoder3.mesure_total_encoder += my_encoder3.mesure_unit_encoder ;      //测定的总编码器计数  定时清0
        my_encoder3.encoder_totalt_difference = 	my_encoder3.expect_total_encoder -  my_encoder3.mesure_total_encoder ;  //差值	
		my_encoder3.encoder_unit_difference = my_encoder3.expect_unit_encoder - my_encoder3.mesure_unit_encoder; */
		}


/*********************************************************************10ms 中断产生一次
此函数作用
系统指示灯闪亮||电机通信位保护||清0里程用于控制的数据||pid控制
*****************************************************************************************/
void TIM2_IRQHandler(void) 							   //5ms  200hz
{ 
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)== SET  && 	System.S_Initialize ==1  )	   //检测是否发生溢出更新事件
	{
		
	  static int16_t led_hz = 1;
		
		System.S_cnt1++;		
		System.S_cnt2++; 	
		System.S_cnt3++; 
		if(	System.S_cnt1 >= (int16_t)(1000/(led_hz*5)))
		{
      System.S_cnt1=0;
			LED=!LED;
    }
		
		
		if(	System.S_cnt3 >= 2000)  //清0里程用于控制的数据
		{
      System.S_cnt3=0;
			my_encoder1.expect_total_encoder = 0;
			my_encoder1.mesure_total_encoder = 0;
			my_encoder1.encoder_totalt_difference = 0;
			my_encoder2.expect_total_encoder = 0;
			my_encoder2.mesure_total_encoder = 0;
			my_encoder2.encoder_totalt_difference = 0;
			my_encoder3.expect_total_encoder = 0;
			my_encoder3.mesure_total_encoder = 0;
			my_encoder3.encoder_totalt_difference = 0;

    }
	  encoder_analysis();
		//ANO_DT_Send_Status(my_encoder1.mesure_anglev ,my_encoder2.mesure_anglev ,-my_encoder3.mesure_anglev  );
		//ANO_DT_Send_Status2(my_encoder1.past_length ,my_encoder2.past_length ,-my_encoder3.past_length  );
		//ANO_DT_Send_INT(C1,C2,C3);
	
	}
	
	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
}
