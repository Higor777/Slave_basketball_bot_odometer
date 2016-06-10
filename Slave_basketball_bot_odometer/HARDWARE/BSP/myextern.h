#ifndef __myextern_H__
#define __myextern_H__

#include "stm32f10x.h"

#define IMAX   400   //MA
#define PID_hz 200            // 1圈编码器的脉冲数


typedef struct System{   // 系统各种标志
	      int8_t Identity ;        //ID 号
	      int8_t Signal_Update;    //通信更新标志
	      int8_t Signal_error;     //通信错误标志
	      int8_t S_Initialize;     //系统状态  0  系统没初始化   |1   系统初始化成功
	      float  Sys_Time;         //系统时钟 
	      float  CPU_T;            //CPU温度
	      float  Eledat;           //电流大小	
	      int8_t Elemoder;         //过流标志   1为过流
        int8_t Control_Moder;    // 控制模式
	      int16_t S_cnt1 ;
	      int16_t S_cnt2 ;
	      int16_t S_cnt3 ;
        }System_flag;      // 系统时间


typedef struct Sensor_Encoder{
		int32_t  count;			  //脉冲个数 
		int32_t  rcount;	    //电机圈数
		int32_t  expect_unit_encoder;      //目的角速度转化成的编码器计数，
		int32_t  mesure_unit_encoder;      //编码器测定的计数
		int32_t  encoder_unit_difference;  //差值
	
	  float  expect_total_encoder;      //期望的总编码器计数  定时清0
		float  mesure_total_encoder;      //测定的总编码器计数  定时清0
		float  encoder_totalt_difference;  //差值
	
		float    expect_anglev;	    //输入目的角速度 单位数据
	  float    mesure_anglev;	    //测量的的角速度  单位数据 回馈给上位机
		float    past_encoder;      //从上电开始走过的编码器的计数
		float    past_length;       //从上电开始走走过的总角度   正负	  反馈给上位机 由上位机清零
	 }Encoder;



typedef struct PID_data{
				int8_t I_Flag;         // PID积分使能标志       油门大于一定值才开始积分
				int8_t Reset_Pose;     //重置姿态角
				int8_t Set_PID;        //遥控发送PID
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
