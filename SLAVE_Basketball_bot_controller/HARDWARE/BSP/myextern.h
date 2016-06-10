#ifndef __myextern_H__
#define __myextern_H__
#include "stm32f4xx.h"


#define IMAX   400            //MA
#define PID_hz 200            // 1Ȧ��������������
#define  wheel_R 0.051f       //���Ӱ뾶
#define  pi 3.1415f
#define  radius 0.255f					//�����ĵİ뾶���� 	50/sqrt(3)	


typedef struct System{   // ϵͳ���ֱ�־
	      int8_t Identity ;        //ID ��
	      int8_t Signal_Update;    //ͨ�Ÿ��±�־
	      int8_t Signal_error;     //ͨ�Ŵ����־
	      int8_t S_Initialize;     //ϵͳ״̬  0  ϵͳû��ʼ��   |1   ϵͳ��ʼ���ɹ�
	      float  Sys_Time;         //ϵͳʱ�� 
	      float  CPU_T;            //CPU�¶�
	      float  Eledat;           //������С	
	      int8_t Elemoder;         //������־   1Ϊ����
        int8_t Control_Moder;    // ����ģʽ
	      int16_t S_cnt1 ;
	      int16_t S_cnt2 ;
	      int16_t S_cnt3 ;
        }System_flag;      // ϵͳʱ��



#define Motor_Num 3 

typedef struct float_motor{
		float M1;
		float M2;
		float M3;
	 }Motor; 

//�����˾�������
typedef struct float_Coordinate{        
		float x;
		float y;
		float z;
	 }Coord;

 //ͷ����̬
typedef struct float_head_Attitude{    
		float Yaw;
		float Pitch;
	 }Head;


 //ͷ����̬
typedef struct float_IMU{    
	  float Yaw_Angle;
		float Pitch_Angle;
	  float Roll_Angle;
	  float Acc_x;
		float Acc_y;
		float Acc_z;
	 }float_IMU;

typedef struct Robot_Local_Speed{
	float x;
	float y;
	float z;
}Robot_Local_Speed;

//��������
typedef struct Robot_data{
	Robot_Local_Speed local_speed;
	Coord E_Global_Speed ;    //�����˵�Ŀ���ٶ�����
	Coord Remote_Speed ;       //ң���������ٶ�
	Motor L_Speed ;            //����ĵ�ǰ�ٶ�
	Motor E_Speed ;            //�����Ŀ���ٶ�
	Coord Robot ;              //����������
	Motor Total_angle ;        //����ߵ��ܽǶ� 
	Motor Total_distance ;     //����ߵ������
	float_IMU Imu ;
}Robot_data;


#endif
