/*********************************************************************************************
* 文件：MP-4.c
* 作者：Lixm 2017.10.17
* 说明：可燃气体驱动代码
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "MP-4.h"
#include "uart1.h"
#include <stdio.h>

/*********************************************************************************************
* 名称：CombustibleGas_init()
* 功能：可燃气体传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void combustiblegas_init(void)
{
  APCFG |= 0x20;                                                //模拟 I/O 使能  
  P0SEL |= 0x20;                                                //端口0_5 功能选择外设功能
  P0DIR &= ~0x20;                                               //设置输入模式
  ADCCON3  = 0xB5;                                              //选择AVDD5为参考电压；12分辨率；P0_5  ADC  
  ADCCON1 |= 0x30;                                              //选择ADC的启动模式为手动
}

/*********************************************************************************************
* 名称：unsigned int get_infrared_status(void)
* 功能：获取可燃气体传感器状态
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
int get_combustiblegas_data(void)
{ 
  unsigned int  value;
  ADCCON3  = 0xB5;                                              //选择AVDD5为参考电压；12分辨率；P0_5  ADC    
  ADCCON1 |= 0x30;                                              //选择ADC的启动模式为手动
  ADCCON1 |= 0x40;                                              //启动AD转化             
  
  while(!(ADCCON1 & 0x80));                                     //等待ADC转化结束  
  value =  ADCL >> 2;
  value |= (ADCH << 6)>> 2;                                     //取得最终转化结果，存入value中  
  return value;
}