#include <ioCC2530.h>
#include "timer1.h"

void timer1_init(void){
  T1CTL |= 0x0F;                    //设置分频以及计数模式（[0-1位：计数模式]，[2-3位：n分频]）
    T1CCTL0 |= 0x44;              //设置定时器Timer1的Channel0（通道0）的工作模式[bit6]
    //错误的思路
//  T1CCTL0 |= 0x40;              //设置定时器Timer1的Channel0（通道0）的工作模式[bit6]
//  T1CCTL0 |= 0x04;              //设置Timer1的Channel0（通道0）允许提交中断[bit2]
  T1IE = 1;  //IEN1 = 1;         //设置Timer1允许提交中断请求
  EA = 1;                                 //允许外部中断请求
  
        //0xF424 = 62500(10)
  T1CC0L = 0x24;
  T1CC0H = 0xF4; 
}


  //错误的思路
/*
void timer1_on(void){
  T1IE =1; //IEN1 = 1;           ///设置Timer1禁止提交中断请求
}

void timer1_off(void){
  T1IE =0;  //IEN1 = 0;         //设置Timer1禁止提交中断请求
}
*/

