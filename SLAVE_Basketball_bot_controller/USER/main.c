#include "sys.h"
#include "bsp.h"
#include "queue.h"
#include "myextern.h"
#include "usart3.h"

/********************************************************************
���������������˳ʱ����תΪ������
�������Ϊ��ʱ��
********************************************************************/

#define MAX_DATA_LEN 100   //���ݽ��ջ�������С
volatile Queue rx_queue;
uint8_t radBufferRS485[99]={0};  		 //��¼ת��
int noselect;		  	//����ģʽѡ����
Robot_data  Ke ;
System_flag System;
/***************��������**************/
float xianfu(float v, float x);
/***************����ָ��**************/
void clear(void);
/***************�ٶȸ�ֵ**************/
void start_dc(void);
/*******��ֵ����������ϵ�ٶ�**********/
void start_dc_local(void);
/***************��������**************/
void handle_data(uint8_t buf[], int len);
/***************���ݷ���**************/
void send(void);

int main(void)
{ 
	static uint16_t checksum;
	uint8_t datalen1;
	short data_len;
	int cur=0;
	uint8_t data;
	uint8_t  buffer[MAX_DATA_LEN] = {0};
	enum Recstate								   		//״̬�� 
	{
		RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
	} rs485Recstate = RECFF1;
	//��ʼ��
	System.S_Initialize=0;     //ϵͳ״̬  0  ϵͳû��ʼ��   |1   ϵͳ��ʼ���ɹ�
	BSP_Init();   
 	NVIC_Configuration();  
	queue_init(&rx_queue);
  System.Control_Moder=2;    // ����ģʽ  2 3 ʹ�ܵ������
	System.S_Initialize= 1;

	while(1)
	{
	if(queue_empty(rx_queue))
							continue;

		data = (uint8_t)queue_get(&rx_queue);
  
		switch (rs485Recstate) 
		{			case RECFF1:
				if (data == 0xff)	  
				{	
					rs485Recstate = RECFF2;	
					checksum=0;		
					cur = 0;								//У��λ����
				}
	
				break;
	
			case RECFF2:
				if (data == 0xff)
					rs485Recstate = RECID;
				else
					rs485Recstate = RECFF1;
				break;
	
	
			case RECID:				 					
			  if(data==0x01)	       //�豸���0x01����
			    {
					checksum += data;
					rs485Recstate = RECLEN1;
				  }
				else
					rs485Recstate = RECFF1;	  
				break;
	
			case RECLEN1:				 				
				checksum += data;
				datalen1 = data;
				rs485Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				checksum += data;			 				
				data_len = (short)datalen1 << 8	 | data;
				rs485Recstate = RECSEL;	  
				break;
	
			case RECSEL:
				checksum += data;
				buffer[cur++] = data;
				if(cur >= data_len)
					rs485Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				checksum=checksum%255;
				if(data == checksum)
				{				
					handle_data(buffer, data_len);
  				send();				
					checksum=0;	
					rs485Recstate = RECFF1;	 
				}
				else
					rs485Recstate = RECFF1;
				break;
			 default:
					rs485Recstate = RECFF1;
		}	
	}
}


float xianfu(float v, float x)
{
   if(v>x)
	 {
     return x;
   }	 
   else if(v<-x)
	 {
     return -x;
   }
	 else return v;

}

void clear(void)			  				
{
	
	Ke.Robot.x = 0 ; 
	Ke.Robot.y = 0 ; 
	Ke.Robot.z = 0 ; 
	Ke.Total_angle.M1 = 0 ; 
	Ke.Total_angle.M2 = 0 ; 
	Ke.Total_angle.M3 = 0 ;
	Ke.Total_distance.M1 =0 ; 	
	Ke.Total_distance.M2 =0 ;
	Ke.Total_distance.M3 =0 ;

} 
void start_dc(void)	
{	

		/*********************��ֵ�������ٶ�*******************/
		if(noselect==0x01)
		{

		System.Control_Moder = 2;    //���contro_mode,�����local���Ƹ���

    memcpy(&Ke.E_Global_Speed  , radBufferRS485 , 12);	  //

	  Ke.E_Global_Speed.x=xianfu(Ke.E_Global_Speed.x, 10);  //��ֹ�ٶȹ���ͨ�ź���˵�ġ�
	  Ke.E_Global_Speed.y=xianfu(Ke.E_Global_Speed.y, 10);
		Ke.E_Global_Speed.z=xianfu(Ke.E_Global_Speed.z, 10);
		}
}
void start_dc_local(void)
{
	
		if(noselect==0x05)   
		{
			System.Control_Moder = 3;    //���contro_mode,�����local���Ƹ���
    memcpy(&Ke.Remote_Speed  , radBufferRS485 , 12);	  //����������ٶȴ洢��radBufferRS485

	  Ke.Remote_Speed.x=xianfu(Ke.E_Speed .M1, 10);  //��ֹ�ٶȹ���ͨ�ź���˵�ġ�
	  Ke.Remote_Speed.y=xianfu(Ke.E_Speed .M2, 10);
		Ke.Remote_Speed.z=xianfu(Ke.E_Speed .M3, 10);
		}
}
void handle_data(uint8_t buf[], int len)
{
	  int i=0;	
	
		switch(buf[0])
		{	
			case 1: noselect = 0x01 ;break;		   //��������������ϵ�ٶ�	
			case 2: noselect = 0x02 ;break;		   //��ȡ��������Ľ��ٶ�
			case 3: noselect = 0x03 ;break;	     //���ñ���������		 
      case 4: noselect = 0x04 ;break;	     //��ȡx,y,w����
      case 5: noselect = 0x05 ;break;      //���ƻ���������ϵ�ٶ�			
			default:break;
		}
		if(noselect == 0x01 )			             //��������������ϵ�ٶ���Ϣת�Ƶ�radBufferRS485[i]
		{						
			for(i=0;i< 4 * Motor_Num ; i++)												
			{
				radBufferRS485[i] = buf[i+1] ;							
			}			
		}
		if(noselect == 0x05 )			             //����ٶ���Ϣת�Ƶ�radBufferRS485[i]
		{						
			for(i=0;i< 4 * Motor_Num ; i++)												
			{
				radBufferRS485[i] = buf[i+1] ;							
			}			
		}
}
void send(void)
{

	int i;
	char byte_s[4*Motor_Num],byte_Coord[4*3];	
  uint16_t checksumsend;				 //֮ǰ��	 uint16_t   ���������֮ǰ��ô�������
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=RESET)
	{	

	 if(noselect==0x01) 				 //��������������ϵ�ٶ�			 
		{
			
			System.Signal_Update = 1 ;    //�����źŸ���
			start_dc();										
			noselect=0;
	 	}

	 	if(noselect == 0x02)			 // ��ȡ���ٶ� 
		{	
			USART3_SendChar(0xff);
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);									 										   
			USART3_SendChar(0x00);											   		
			USART3_SendChar(0x0d);											   
			USART3_SendChar(0x02);
			
			checksumsend = 0x10;      //��������У��λ
			
		  memcpy( byte_s , &Ke.local_speed , 4*Motor_Num );		   
			for(i=0 ;i < 4 * Motor_Num ; i++)
			{
				USART3_SendChar( byte_s[i] );
			  checksumsend = checksumsend + byte_s[i];
			}
				
			checksumsend = checksumsend%255 ;
			USART3_SendChar(checksumsend) ;
			
			noselect=0;			  					
	 	}

	   if(noselect==0x03)			   //���ñ�����������
		{	
		 
			clear();				
			USART3_SendChar(0xff);		
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);     //���̿���
			USART3_SendChar(0x00);		 //��λ ��ʾ���ݳ���Ϊ2
			USART3_SendChar(0x02);	   
			USART3_SendChar(0x03);     //��ʾ3�Ŷ���
			USART3_SendChar(0x00);		 //��ʾ����ִ�гɹ�	
			USART3_SendChar(0x06);
			noselect=0;
		}

		if(noselect == 0x04)		   //  ����FLOAT ��X Y W����
		{
		
			USART3_SendChar(0xff);					
			USART3_SendChar(0xff);

			USART3_SendChar(0x01);		 //���̿���							
			USART3_SendChar(0x00);				
			USART3_SendChar(0x0d);		 //��λ ��ʾ���ݳ���Ϊ13
			USART3_SendChar(0x04);     //��ʾ4�Ŷ���

			checksumsend = 0x12;

			memcpy( byte_Coord , &Ke.Robot , 4*3 );

			for(i=0 ; i < 4 * 3 ; i++)
			{
				USART3_SendChar( byte_Coord[i] );
				checksumsend=checksumsend + byte_Coord[i] ;
			}		 
	
			checksumsend =checksumsend % (0xff);
		
			USART3_SendChar(checksumsend) ;						
			noselect=0;		  					
	 	}
	 	if(noselect == 0x05){
	 		System.Signal_Update = 1 ;    //�����źŸ���
			start_dc_local();										
			noselect=0;
	 	}
  }
   while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);				
	
}
