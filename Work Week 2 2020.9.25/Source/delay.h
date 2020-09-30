/*********************************************************************************************
* 文件：delay.h
* 作者：Meixin 2017.09.27
* 说明：延时驱动头文件
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <ioCC2530.h>                                           //引入CC2530所对应的头文件（包含各SFR的定义）

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

/*********************************************************************************************
* 申明定义无符号数据类型
*********************************************************************************************/
typedef unsigned char           u8;                           	//将unsigned char 声明定义为 u8        
typedef unsigned short          u16;                          	//将unsigned short 声明定义为 u16 
typedef unsigned long           u32;                          	//将unsigned int 声明定义为 u32

/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define CLKSPD                  (CLKCONCMD & 0x07)              //宏定义系统时钟分频系数

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void delay_s(u16 times);                                        //硬件延时函数秒
void delay_ms(u16 times);                                       //硬件延时函数毫秒
void delay_us(u16 times);                                       //硬件延时函数微秒
void hal_wait(u8 wait);                                         //硬件毫秒延时函数
void delay_pwm(unsigned int t);                                	//PWM专用延时

#endif /*__DELAY_H_*/