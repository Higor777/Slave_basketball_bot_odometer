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
extern System_flag System;   // ϵͳ����־
extern Encoder my_encoder1;
extern Encoder my_encoder2;
extern Encoder my_encoder3;

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PD2Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PD2 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD2
 GPIO_SetBits(GPIOD,GPIO_Pin_2);						 //PD2 �����

}
 
/*********************************************************************10ms �жϲ���һ��
�˺�������
�Բ��ֲ����������㣬��ֹ�ۻ�������
*****************************************************************************************/
void clear(void)			  				
{
	my_encoder1.mesure_total_encoder=0;
	my_encoder1.expect_total_encoder=0;
	my_encoder1.past_encoder=0;      //���ϵ翪ʼ�߹��ı������ļ���
	my_encoder1.past_length=0;       //���ϵ翪ʼ���߹����ܽǶ�   ����	  ��������λ�� ����λ������	

	my_encoder2.mesure_total_encoder=0;
	my_encoder2.expect_total_encoder=0;
	my_encoder2.past_encoder=0;      //���ϵ翪ʼ�߹��ı������ļ���
	my_encoder2.past_length=0;
	
	my_encoder3.mesure_total_encoder=0;
	my_encoder3.expect_total_encoder=0;
	my_encoder3.past_encoder=0;      //���ϵ翪ʼ�߹��ı������ļ���
	my_encoder3.past_length=0;
} 


/*********************************************************************10ms �жϲ���һ��
�˺�������
����C������ת���ܽǶ�
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
	TxMessageBuffer.ExtId=((myID<<8)|UID); //������ID |  ������ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // ��Ϣ��־��ʵ���׼��ʶ��
  TxMessageBuffer.DLC=6;					   // ������֡��һ֡8λ
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3����ͻظ��Ϳ���
	TxMessageBuffer.Data[1]=0x02;			 // ָ������   �ظ��Ƕ�
	
  //b=my_encoder.past_length;
	memcpy(byte,&b,sizeof(float));
  for(i=0;i<4;i++)
  {		
		
		TxMessageBuffer.Data[i+2]=byte[i];		
		
  }
	
	CAN_Transmit(CAN1, &TxMessageBuffer);   //�ѵ�ǰ���ٶȺ͵�ǰ�ܽǶȷ������ذ�

}


/*********************************************************************10ms �жϲ���һ��
�˺�������
һ�η�������CAN����֡��������������ת���ܽǶ�
*****************************************************************************************/
void Can_response_All(void)
{	
  CanTxMsg TxMessageBuffer;
	u8 byte[12],i;
  TxMessageBuffer.StdId=0x00;	 
	TxMessageBuffer.ExtId=((MYID1<<8)|UID); //������ID |  ������ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // ��Ϣ��־��ʵ���׼��ʶ��
  TxMessageBuffer.DLC=8;					   // ������֡��һ֡8λ
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3����ͻظ��Ϳ���
	TxMessageBuffer.Data[1]=0x02;			 // ָ������   �ظ��Ƕ�
	memcpy(byte,&my_encoder1.past_length,4);
	memcpy(byte+4,&my_encoder2.past_length,4);
	memcpy(byte+8,&my_encoder3.past_length,4);
  for(i=0;i<6;i++)
  {		
		TxMessageBuffer.Data[i+2]=byte[i];		
  }
	CAN_Transmit(CAN1, &TxMessageBuffer);   //�ѵ�ǰ���ٶȺ͵�ǰ�ܽǶȷ������ذ�
	delay_us(300);
  TxMessageBuffer.StdId=0x00;	 
	TxMessageBuffer.ExtId=((MYID2<<8)|UID); //������ID |  ������ID     
	TxMessageBuffer.RTR=CAN_RTR_DATA;	     // ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessageBuffer.IDE=CAN_ID_EXT;		     // ��Ϣ��־��ʵ���׼��ʶ��
  TxMessageBuffer.DLC=8;					   // ������֡��һ֡8λ
	TxMessageBuffer.Data[0]=0x02;			 // 1or2or3����ͻظ��Ϳ���
	TxMessageBuffer.Data[1]=0x02;			 // ָ������   �ظ��Ƕ�
  for(i=0;i<6;i++)
  {		
		TxMessageBuffer.Data[i+2]=byte[i+6];		
  }
	CAN_Transmit(CAN1, &TxMessageBuffer);   //�ѵ�ǰ���ٶȺ͵�ǰ�ܽǶȷ������ذ�
}



/*********************************************************************10ms �жϲ���һ��
�˺�������
�������������ݴ���
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
/*******************************************������1************************************/
	my_encoder_analysis();
	
    my_encoder1.mesure_unit_encoder = my_encoder1.rcount * encoder_data1 + my_encoder1.count;
	my_encoder1.rcount=0;
	my_encoder1.count=0;
   
	my_encoder1.past_encoder += my_encoder1.mesure_unit_encoder;                 //�Ѿ��߹��ı������		������
	my_encoder1.past_length = (my_encoder1.past_encoder/encoder_data1)*360;     // �Ѿ��߹�������� //can����ֵ   �Ƕ�   ������
/* 	my_encoder1.mesure_anglev  = my_encoder1.mesure_unit_encoder * PID_hz * 360 / encoder_data1 ;            //�������ٶ�   ��λ��
	my_encoder1.mesure_total_encoder += my_encoder1.mesure_unit_encoder ;      //�ⶨ���ܱ���������  ��ʱ��0
	my_encoder1.encoder_totalt_difference = 	my_encoder1.expect_total_encoder -  my_encoder1.mesure_total_encoder ;  //��ֵ	
	my_encoder1.encoder_unit_difference = my_encoder1.expect_unit_encoder - my_encoder1.mesure_unit_encoder; */
		
/***********************************������2****************************************/
	my_encoder2.mesure_unit_encoder = my_encoder2.rcount * encoder_data2 + my_encoder2.count;
	my_encoder2.rcount=0;
	my_encoder2.count=0;
  
	my_encoder2.past_encoder += my_encoder2.mesure_unit_encoder;                 //�Ѿ��߹��ı������		������
	my_encoder2.past_length = (my_encoder2.past_encoder/encoder_data2)*360;                              // �Ѿ��߹��������
/* 	my_encoder2.mesure_anglev  = my_encoder2.mesure_unit_encoder * PID_hz * 360 / encoder_data2 ;            //�������ٶ�   ��λ��
	my_encoder2.mesure_total_encoder += my_encoder2.mesure_unit_encoder ;      //�ⶨ���ܱ���������  ��ʱ��0
    my_encoder2.encoder_totalt_difference = 	my_encoder2.expect_total_encoder -  my_encoder2.mesure_total_encoder ;  //��ֵ	
    my_encoder2.encoder_unit_difference = my_encoder2.expect_unit_encoder - my_encoder2.mesure_unit_encoder;
		 */
/*****************************************������3******************************************************/
	my_encoder3.mesure_unit_encoder = my_encoder3.rcount * encoder_data3 + my_encoder3.count;
	my_encoder3.rcount=0;
	my_encoder3.count=0;
	
	my_encoder3.past_encoder += my_encoder3.mesure_unit_encoder;                 //�Ѿ��߹��ı������		������
	my_encoder3.past_length = (my_encoder3.past_encoder/encoder_data3)*360;                              // �Ѿ��߹��������
/* 		my_encoder3.mesure_anglev  = my_encoder3.mesure_unit_encoder * PID_hz * 360 / encoder_data3 ;            //�������ٶ�   ��λ��
	    my_encoder3.mesure_total_encoder += my_encoder3.mesure_unit_encoder ;      //�ⶨ���ܱ���������  ��ʱ��0
        my_encoder3.encoder_totalt_difference = 	my_encoder3.expect_total_encoder -  my_encoder3.mesure_total_encoder ;  //��ֵ	
		my_encoder3.encoder_unit_difference = my_encoder3.expect_unit_encoder - my_encoder3.mesure_unit_encoder; */
		}


/*********************************************************************10ms �жϲ���һ��
�˺�������
ϵͳָʾ������||���ͨ��λ����||��0������ڿ��Ƶ�����||pid����
*****************************************************************************************/
void TIM2_IRQHandler(void) 							   //5ms  200hz
{ 
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)== SET  && 	System.S_Initialize ==1  )	   //����Ƿ�����������¼�
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
		
		
		if(	System.S_cnt3 >= 2000)  //��0������ڿ��Ƶ�����
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
