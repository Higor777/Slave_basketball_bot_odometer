#ifndef _TRACKS_GUI_H
#define _TRACKS_GUI_H
void Local_Speed_Set(float x , float y , float z);
void Global_Speed_Set(float x ,float y ,float z) ;   //����ȫ���ٶ�
void Robot_Speed_Set(float x ,float y ,float z)  ;  //���û����������ٶ�
void R_Round_Motion_Set( float Line_Speed , float R) ;    //��Բ������  ����������  �Ͷ�����
 //��Բ������  ��������  �߶�����  ��x yΪԲ�� Line_Speed���ٶ�  �뾶R  ��ת�ٶ�z
void G_Round_Motion_Set(float x ,float y , float Line_Speed , float R ,float z)   ;
void Sin_Motion_Set(float Amplitude  , float T , float Line_Speed , float z)  ;  //sin�˶�����  ����������λ
#endif
