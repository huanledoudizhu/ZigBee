#include <ioCC2530.h>
#include "timer1.h"

void timer1_init(void){
  T1CTL |= 0x0F;                    //���÷�Ƶ�Լ�����ģʽ��[0-1λ������ģʽ]��[2-3λ��n��Ƶ]��
    T1CCTL0 |= 0x44;              //���ö�ʱ��Timer1��Channel0��ͨ��0���Ĺ���ģʽ[bit6]
    //�����˼·
//  T1CCTL0 |= 0x40;              //���ö�ʱ��Timer1��Channel0��ͨ��0���Ĺ���ģʽ[bit6]
//  T1CCTL0 |= 0x04;              //����Timer1��Channel0��ͨ��0�������ύ�ж�[bit2]
  T1IE = 1;  //IEN1 = 1;         //����Timer1�����ύ�ж�����
  EA = 1;                                 //�����ⲿ�ж�����
  
        //0xF424 = 62500(10)
  T1CC0L = 0x24;
  T1CC0H = 0xF4; 
}


  //�����˼·
/*
void timer1_on(void){
  T1IE =1; //IEN1 = 1;           ///����Timer1��ֹ�ύ�ж�����
}

void timer1_off(void){
  T1IE =0;  //IEN1 = 0;         //����Timer1��ֹ�ύ�ж�����
}
*/

