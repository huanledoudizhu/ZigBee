#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>
#include "info.h"
#include "htu21d.h"

#define RF_CHANNEL            17                                 // 2.4 GHz RF channel
#define PAN_ID                0x2000
#define SEND_ADDR             0x2530
#define RECV_ADDR             0x2520

//#define NODE_TYPE             0                                 //0:接收节点，1:发送节点

static basicRfCfg_t basicRfConfig;
uint16 recvice_address = RECV_ADDR;
char pTxData[128];
uint8 pRxData[128];
/* 射频模块发送数据函数 */

void rfSendData(void){                                          //定义要发送的数据
     basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = SEND_ADDR;
    basicRfInit(&basicRfConfig);
    basicRfReceiveOff();                                        //关闭射频接收器
    basicRfSendPacket(0xffff, (uint8*)pTxData, sizeof pTxData);//广播发送数据包 
   // halMcuWaitMs(2000);
}


/* 射频模块接收数据函数 */

void rfRecvData(void)
{
  float data = 0;                                               //存储寄存器数据变量
  int rlen;
     basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = RECV_ADDR;
    basicRfInit(&basicRfConfig);
   basicRfReceiveOn();                                          //开启射频接收器
   while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 && pRxData[0] == 0x31) {
            hal_led_on(1);  
            halMcuWaitMs(100);
            hal_led_off(1); 
            data = htu21d_get_data(TEMPERATURE)/100.0;                //读取传感器值
            sprintf(pTxData,"Device:2,addr:0x%x, %.2f\r\n",recvice_address, data);
             printf("%s",pTxData);
             halMcuWaitMs(1000);
             rfSendData();
             
          }
}

void main(void)
{
    halMcuInit();                                               //初始化mcu
    hal_led_init();                                             //初始化LED 
    htu21d_init();                                                //温湿度传感器初始化
    hal_uart_init();                                            //初始化串口
    //lcd_dis();
    halRfInit();
    while(1){
      rfRecvData();
      halMcuWaitMs(10);
    }
}

    /*
    if (FAILED == halRfInit()) {                                //halRfInit()为初始化射频模块函数
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;                               //panID
    basicRfConfig.channel = RF_CHANNEL;                         //通信信道
    basicRfConfig.ackRequest = TRUE;                            //应答请求
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;                            //安全秘钥
#endif
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
/*   
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;                           //发送地址
#else
    basicRfConfig.myAddr = RECV_ADDR;                           //接收地址
#endif
    */
    
 /*
#if NODE_TYPE
  rfSendData();                                                 //发送数据
#else 
  rfRecvData();                                                 //接收数据
#endif
    */
    /*
       basicRfConfig.myAddr = RECV_ADDR;
       basicRfReceiveOn(); 
     while(1){
     while(!basicRfPacketIsReady());
     rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
     if(rlen > 0 && pRxData[0] == 0x31) {
         hal_led_on(1);  
         data = htu21d_get_data(TEMPERATURE)/100.0;                //读取传感器值
         basicRfConfig.myAddr = SEND_ADDR; 
         basicRfReceiveOff();                                        //关闭射频接收器
         sprintf(pTxData,"addr:0x%x, %.2f\r\n",recvice_address, data);
         printf("%s",pTxData);
         basicRfSendPacket(0xffff, (uint8*)pTxData, sizeof pTxData);  
         basicRfConfig.myAddr = RECV_ADDR;
         basicRfReceiveOn();
     }
      }
        
    */
     
 
