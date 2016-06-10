#ifndef __myextern_H__
#define __myextern_H__

#include "stm32f10x.h"

#define IMAX   400   //MA
#define PID_hz 200            // 1Ȧ��������������


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


typedef struct Sensor_Encoder{
		int32_t  count;			  //������� 
		int32_t  rcount;	    //���Ȧ��
		int32_t  expect_unit_encoder;      //Ŀ�Ľ��ٶ�ת���ɵı�����������
		int32_t  mesure_unit_encoder;      //�������ⶨ�ļ���
		int32_t  encoder_unit_difference;  //��ֵ
	
	  float  expect_total_encoder;      //�������ܱ���������  ��ʱ��0
		float  mesure_total_encoder;      //�ⶨ���ܱ���������  ��ʱ��0
		float  encoder_totalt_difference;  //��ֵ
	
		float    expect_anglev;	    //����Ŀ�Ľ��ٶ� ��λ����
	  float    mesure_anglev;	    //�����ĵĽ��ٶ�  ��λ���� ��������λ��
		float    past_encoder;      //���ϵ翪ʼ�߹��ı������ļ���
		float    past_length;       //���ϵ翪ʼ���߹����ܽǶ�   ����	  ��������λ�� ����λ������
	 }Encoder;



typedef struct PID_data{
				int8_t I_Flag;         // PID����ʹ�ܱ�־       ���Ŵ���һ��ֵ�ſ�ʼ����
				int8_t Reset_Pose;     //������̬��
				int8_t Set_PID;        //ң�ط���PID
				float P1;
				float I1;
			  float D1;
		    float P2;
				float I2;
			  float D2;
				float A_error_now;
				float A_error_last;
			  float A_error_before_last;
				float A_error_P;
				float A_error_I;
				float A_error_D;
				float A_Pidout;
		    float S_error_now;
				float S_error_last;
			  float S_error_before_last;
				float S_error_P;
				float S_error_I;
				float S_error_D;
				float S_Pidout;
        }PID;

#endif
