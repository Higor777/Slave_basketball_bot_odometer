#include "BSP.h"
#include "TF.h"
#include "myextern.h"
#include "Tracks_GUI.h"


extern Robot_data  Ke ;

void Local_Speed_Set(float x , float y , float z)
{
	static float Local[3],Speed[3] ;	
	  Local[0] = -x ;
	  Local[1] = -y ;
	  Local[2] = z;
	  Robot_To_Motor_tf(Local , Speed);
		Ke.E_Speed.M1  = ( Speed[0] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M2  = ( Speed[1] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M3  = ( Speed[2] / wheel_R ) * ( 180/pi );	
}

void Global_Speed_Set(float x ,float y ,float z)   //����ȫ���ٶ�
 {
	  static float Global[3],Speed[3] ;	
	  Global[0] = -x ;
	  Global[1] = -y ;
	  Global[2] = z;
		Global_To_Motor_tf( Global , Speed , Ke.Robot.z) ;
		Ke.E_Speed.M1  = ( Speed[0] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M2  = ( Speed[1] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M3  = ( Speed[2] / wheel_R ) * ( 180/pi );	
	 
 }
 
 void Robot_Speed_Set(float x ,float y ,float z)   //���û����������ٶ�
 {
	  static float Robot[3],Speed[3] ;	
	  Robot[0] = x ;
	  Robot[1] = y ;
	  Robot[2] = z ;
	 
		Robot_To_Motor_tf( Robot , Speed ) ;
	 
		Ke.E_Speed.M1  = ( Speed[0] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M2  = ( Speed[1] / wheel_R ) * ( 180/pi );
		Ke.E_Speed.M3  = ( Speed[2] / wheel_R ) * ( 180/pi );	
 }
 
   //��Բ������  ����������  �Ͷ�����
 void R_Round_Motion_Set( float Line_Speed , float R)    
 {
	 
   Robot_Speed_Set( Line_Speed , 0 , Line_Speed / R) ;
	 
 }
 
 
  //��Բ������  ��������  �߶�����  ��x yΪԲ�� Line_Speed���ٶ�  �뾶R  ��ת�ٶ�z
 void G_Round_Motion_Set(float x ,float y , float Line_Speed , float R ,float z)     
 {
	 
   Robot_Speed_Set( Line_Speed , 0 , Line_Speed / R) ;
	 
 }
 
 
 void Sin_Motion_Set(float Amplitude  , float T , float Line_Speed , float z)    //sin�˶�����  ����������λ
 {
		static float dy ;
		dy= (Amplitude * 2 * pi / T) * cos(2 * pi * Ke.Robot.x /T) ; 
		Ke.E_Global_Speed.x = Line_Speed * ( 1 /  sqrt( 1 + pow(dy,2) ) )  ;
		Ke.E_Global_Speed.y = Line_Speed * ( dy /  sqrt( 1 + pow(dy,2) ) ) ;
		Ke.E_Global_Speed.z = z ; 
	  Global_Speed_Set( Ke.E_Global_Speed.x ,Ke.E_Global_Speed.y , Ke.E_Global_Speed.z);
 }
 
 
 void PSin_Motion_Set(float Amplitude , float Int_Phase , float T , float Line_Speed , float z)    //sin�˶�����
 {
	 
 }