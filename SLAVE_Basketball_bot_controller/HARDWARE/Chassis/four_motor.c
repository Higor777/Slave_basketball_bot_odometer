#include "bsp.h"
#include "four_motor.h"
#include "myextern.h"
#include "TF.h"
#include "Tracks_GUI.h"
#include "stm32f4xx_can.h"
#include "can.h"
#include "usart1_IMU.h"
extern Robot_data  Ke ;
extern System_flag System;   // ϵͳ����־
extern struct SAngle 	stcAngle;
extern float FITST_angle_erroer;
extern float Yaw_Angle;
//����ת����������10����ת��Ϊ16���ƣ��β�NΪ��ת��ֵ��ת������ֵ����ָ��h��Ӧ��������
void itoh(int N, uint8_t *h)   
{
    uint8_t a[10]={0};
    int count=0;
    while(N != 0)
    {
       a[count++]=N&0xf;
       N>>=4;
			if(count==8) break;
    }
		
    *h=(a[1]<<4)|a[0];
    *(h+1)=(a[3]<<4)|a[2];
    *(h+2)=(a[5]<<4)|a[4];
    *(h+3)=(a[7]<<4)|a[6];
}
/****************���̿��ƺ���****************/
void Chassis_control(void)            
{ 	 
	  static u8 Motor_cnt = 0;
	  static char motor_cnt=1;
	  motor_cnt=!motor_cnt;
	  Motor_cnt++;
		if(System.Control_Moder == 0)   //ֹͣ
		{
				Ke.E_Speed.M1=0 ;
				Ke.E_Speed.M2=0 ;
				Ke.E_Speed.M3=0 ;

    }
		else if(System.Control_Moder == 1)  //ң��������ģʽ
		{

				
			Robot_Speed_Set(Ke.Remote_Speed.x , Ke.Remote_Speed.y , Ke.Remote_Speed.z);
		

    }
		else if(System.Control_Moder == 2)  //��λ�����ƣ�һ����˵���ǽ����������ģʽ
		{
			
      Global_Speed_Set(Ke.E_Global_Speed.x ,Ke.E_Global_Speed.y ,Ke.E_Global_Speed.z);  //����ȫ���ٶ�

    }
			
		else if(System.Control_Moder == 3)//����������ϵ����
		{
			Robot_Speed_Set(Ke.Remote_Speed.x , Ke.Remote_Speed.y , Ke.Remote_Speed.z);
		}
	  if(System.Control_Moder ==1 || System.Control_Moder == 2 || System.Control_Moder ==3 )               //ʹ�ܿ���
		{		
			if(motor_cnt)
				{
					Can_send_motorx( Ke.E_Speed.M3 ,0x30);//������3��ID��0x30
					delay_us(400);
			    Can_send_motorx( Ke.E_Speed.M2 ,0x32);//������2��ID��0x32
				}
				else
				{
					Can_send_motorx( Ke.E_Speed.M1 ,0x36);//������1��ID��0x36
				}
	   

    }
	  else              //ʧ�ܿ���
		{
			Can_send_motorx( 0,0x36);
			delay_us(200);
			Can_send_motorx( 0,0x30);
			delay_us(200);
			Can_send_motorx( 0,0x32);
			delay_us(200);	
    }
}


/*************************************************
����  void Encoder_analysis(void) 
�����������ݷ���
�����x��y��w,����λ��
**************************************************/
float Len_M[3]={0,0,0};	   	           //ÿ���������߹���̲�ֵ
float Global_C[3]={0,0,0};	   	       //ȫ�ֲ�������
/**************����������ϵ����**************/
void Encoder_analysis(void)
{
	static u8 Encoder_cnt = 0;
	static u8 cnt=0;
	if(cnt==0)     //�����ϵ�����������������
	{
		cnt=1;
		Can_get_encoder(0x01,0x01);
		delay_us(300);
		Can_get_encoder(0x03,0x01);
		delay_us(300);
		Can_get_encoder(0x02,0x01);
		delay_us(300);
  }
	 else
	 {
		 
		  //������������һ�𷢻���ֶ�֡������������ÿ�η�������
	
	  	Can_get_encoder(++Encoder_cnt%3+1,0x00);
      delay_us(300) ;
	  	Can_get_encoder(++Encoder_cnt%3+1,0x00);
      delay_us(300) ;

			Len_M[0] = ( Ke.Total_angle.M1/360.0f ) * 2.0f* pi * wheel_R - Ke.Total_distance .M1;
			Ke.Total_distance .M1 += Len_M[0] ;
	

		
			Len_M[2] = ( Ke.Total_angle.M3/360.0f ) * 2.0f * pi * wheel_R - Ke.Total_distance .M3;
			Ke.Total_distance .M3 += Len_M[2] ;
			
			Len_M[1] = ( Ke.Total_angle.M2/360.0f ) * 2.0f * pi * wheel_R - Ke.Total_distance .M2;
			Ke.Total_distance .M2 += Len_M[1] ;	
			delay_us(200) ;
		 
		 
			Motor_To_Global_tf( Len_M , Global_C , (float)Ke.Robot.z ) ; 
			
			
			if(Global_C[0]<0.000001f&&Global_C[0]>-0.000001f)
			{
				Global_C[0]=0;
			}
			if(Global_C[1]<0.000001f&&Global_C[1]>-0.000001f)
			{
				Global_C[1]=0;
			}		
			if(Global_C[2]<0.000001f&&Global_C[2]>-0.000001f)
			{
				Global_C[2]=0;
			}	
			
			Ke.Robot.x -= Global_C[0];
			Ke.Robot.y -= Global_C[1];
		  Ke.Robot.z =Yaw_Angle/180.0f*pi;
			
			
			//moto_to_robot
			Motor_To_Robot_tf(Len_M , Global_C);
			if(Global_C[0]<0.000001f&&Global_C[0]>-0.000001f)
			{
				Global_C[0]=0;
			}
			if(Global_C[1]<0.000001f&&Global_C[1]>-0.000001f)
			{
				Global_C[1]=0;
			}		
			if(Global_C[2]<0.000001f&&Global_C[2]>-0.000001f)
			{
				Global_C[2]=0;
			}	
			Ke.local_speed.x = Global_C[0];
			Ke.local_speed.y = Global_C[1];
			Ke.local_speed.z = Global_C[2];

   }

}



u8 Can_send_motorx(float v,u8 UID)   //  ���ͽ��ٶȵ�������
{
	u8 mbox;
	u16 i=0;
	uint8_t b[4]={0};
	CanTxMsg TxMessageBuffer;
	
	TxMessageBuffer.StdId=UID;	 
	TxMessageBuffer.ExtId=UID;
	TxMessageBuffer.RTR=0;	              // ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessageBuffer.IDE=0;	              // ��Ϣ��־��ʵ���׼��ʶ��
	TxMessageBuffer.DLC=8;					       // ������֡��һ֡8λ
	TxMessageBuffer.Data[0]=0x08;			     // 1or2or3����ͻظ��Ϳ���
	TxMessageBuffer.Data[1]=UID;			     // ������ID
	TxMessageBuffer.Data[2]=0x90;          //�ٶ�ָ��
  TxMessageBuffer.Data[3]=0x00;
	itoh((int)v, b);                      /*********10����ת16����***********/
	TxMessageBuffer.Data[4]=*b;
	TxMessageBuffer.Data[5]=*(b+1);
	TxMessageBuffer.Data[6]=*(b+2);
	TxMessageBuffer.Data[7]=*(b+3);
	
	i=0;	
  do{	
	mbox = CAN_Transmit(CAN1, &TxMessageBuffer);   //���������ٶȷ���������
	i++;
  }while((mbox==CAN_TxStatus_NoMailBox)&&i<0xfff);	//�������ط�
	i=0;	
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���	
  if(i>=0XFFF)return 1;
  return 0;
}


void Can_get_encoder(u8 UID , u8 noselect)          //  ��������ָ��
{
	u16 i=0;
	u8 mbox;
	CanTxMsg TxMessageBuffe;
	
  TxMessageBuffe.StdId=UID;	 
	TxMessageBuffe.ExtId=UID; //������ID |  ������ID     
	TxMessageBuffe.RTR=0;	     // ������Ϣ��֡����Ϊ����֡������Զ��֡��
	TxMessageBuffe.IDE=0;		     // ��Ϣ��־��ʵ���׼��ʶ��
	TxMessageBuffe.DLC=3;					       // ������֡��һ֡8λ
	TxMessageBuffe.Data[0]=0x03;			     // ���ݳ���
	TxMessageBuffe.Data[1]=UID;           // ������ID
	TxMessageBuffe.Data[2]=noselect;			 // ָ������	 
	

	i=0;	
  do{	
	mbox = CAN_Transmit(CAN1, &TxMessageBuffe);   //�����󷢸�F1
	i++;
  }while((mbox==CAN_TxStatus_NoMailBox)&&i<0xfff);	//�������ط�
	
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���	
	
}



void CAN1_TX_IRQHandler(void)
{
	
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		//LED1=!LED1;
	}
	
}




void CAN1_RX0_IRQHandler(void)
{	
	
	CanRxMsg RxMessage;	
	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		
		
			u8 byte[4],i;
	    //LED1=!LED1;
			RxMessage.StdId=0x0000;   //������ID
			RxMessage.ExtId=0x0032;   //������ID 
			RxMessage.IDE=0;
			RxMessage.DLC=0;
			RxMessage.FMI=0;	
				
		  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); 		
			
			if((RxMessage.ExtId==((0x01<<8)|MYID)) && (RxMessage.DLC==0x06) && (RxMessage.Data[0]==0x02))	//�������
			{
				 
				 for(i=0;i<4;i++)
				 {
					 
						byte[i]=RxMessage.Data[i+2];	
					 
				 }
		     memcpy( &Ke.Total_angle.M1 , byte ,sizeof(int) );	  //���1			 
	 
	    }
		 
		 	if((RxMessage.ExtId==((0x02<<8)|MYID)) && (RxMessage.DLC==0x06) && (RxMessage.Data[0]==0x02))	//�������
			{

				 for(i=0;i<4;i++)
				 {
					 
						byte[i]=RxMessage.Data[i+2];	
					 
				 }
		      memcpy( &Ke.Total_angle.M2 , byte ,sizeof(int) );	  //���2			 
	 
	   }
		 
		 	if((RxMessage.ExtId==((0x03<<8)|MYID)) && (RxMessage.DLC==0x06) && (RxMessage.Data[0]==0x02))	//�������
			{
				 
				 for(i=0;i<4;i++)
				 {
					 
						byte[i]=RxMessage.Data[i+2];	
					 
				 }
		    memcpy( &Ke.Total_angle.M3 , byte ,sizeof(int) );	  //���3			 
	 
	   }
		 
						 
		 CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		 CAN_FIFORelease(CAN1,CAN_FIFO0); //���жϱ�־
			
	}
	
}



/**********************************************************************
TxMessage���ݰ���ʽ
uint32_t StdId;   //������ID
uint32_t ExtId;   //������ID
uint8_t IDE;      //CAN��ʶ��  ϵͳռ��
uint8_t RTR;       // ��Ϣ����  ϵͳռ��
uint8_t DLC;       // ����֡�ĸ���
uint8_t Data[8];   // ���֧��8֡����

���̵��ID    0X01 0X02 0X03 0X04
��������ID    0X10 

���غ͵��֮���ͨѶ

1.���ط���һ��������������ָ��DLC=2��DATA[0]=1or2or3����ͻظ��Ϳ���   DATA[1]=ָ������
  ��������2��������Ϣ
  ��1   DLC= 6   DATA[0]=1or2or3����ͻظ��Ϳ���     DATA[1]=0X00ָ������   DATA[2]DATA[3]DATA[4]DATA[5]    Ϊ���ӵ�ǰ��� 
  ��2   DLC= 6   DATA[0]=1or2or3����ͻظ��Ϳ���     DATA[1]=0X01ָ������   DATA[2]DATA[3]DATA[4]DATA[5]    clear
  ��3   DLC= 6   DATA[0]=1or2or3����ͻظ��Ϳ���     DATA[1]=0X03ָ������   DATA[2]DATA[3]DATA[4]DATA[5]    Ϊ���������ٶ� ����̨���Ϊ�����Ƕȣ�
 
2.���ط���һ�����Ŀ���ָ��DLC=6��DATA[0]=1or2or3����ͻظ��Ϳ���   DATA[1]=ָ������  DATA[2]DATA[3]DATA[4]DATA[5] Ϊ�����ٶȣ���̨���Ϊ�����Ƕȣ�
  ��������1����ȷ���յ���Ϣ 
  ��   DLC= 3   DATA[0]=1or2or3����ͻظ��Ϳ���   DATA[1]=ָ������    DATA[3]=1 ȷ���յ�


**********************************************************************/
