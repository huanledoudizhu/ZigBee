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

//#define NODE_TYPE             0                                 //0:���սڵ㣬1:���ͽڵ�

static basicRfCfg_t basicRfConfig;
uint16 recvice_address = RECV_ADDR;
char pTxData[128];
uint8 pRxData[128];
/* ��Ƶģ�鷢�����ݺ��� */

void rfSendData(void){                                          //����Ҫ���͵�����
     basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = SEND_ADDR;
    basicRfInit(&basicRfConfig);
    basicRfReceiveOff();                                        //�ر���Ƶ������
    basicRfSendPacket(0xffff, (uint8*)pTxData, sizeof pTxData);//�㲥�������ݰ� 
   // halMcuWaitMs(2000);
}


/* ��Ƶģ��������ݺ��� */

void rfRecvData(void)
{
  float data = 0;                                               //�洢�Ĵ������ݱ���
  int rlen;
     basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = RECV_ADDR;
    basicRfInit(&basicRfConfig);
   basicRfReceiveOn();                                          //������Ƶ������
   while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 && pRxData[0] == 0x31) {
            hal_led_on(1);  
            halMcuWaitMs(100);
            hal_led_off(1); 
            data = htu21d_get_data(TEMPERATURE)/100.0;                //��ȡ������ֵ
            sprintf(pTxData,"Device:2,addr:0x%x, %.2f\r\n",recvice_address, data);
             printf("%s",pTxData);
             halMcuWaitMs(1000);
             rfSendData();
             
          }
}

void main(void)
{
    halMcuInit();                                               //��ʼ��mcu
    hal_led_init();                                             //��ʼ��LED 
    htu21d_init();                                                //��ʪ�ȴ�������ʼ��
    hal_uart_init();                                            //��ʼ������
    //lcd_dis();
    halRfInit();
    while(1){
      rfRecvData();
      halMcuWaitMs(10);
    }
}

    /*
    if (FAILED == halRfInit()) {                                //halRfInit()Ϊ��ʼ����Ƶģ�麯��
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;                               //panID
    basicRfConfig.channel = RF_CHANNEL;                         //ͨ���ŵ�
    basicRfConfig.ackRequest = TRUE;                            //Ӧ������
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;                            //��ȫ��Կ
#endif
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
/*   
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;                           //���͵�ַ
#else
    basicRfConfig.myAddr = RECV_ADDR;                           //���յ�ַ
#endif
    */
    
 /*
#if NODE_TYPE
  rfSendData();                                                 //��������
#else 
  rfRecvData();                                                 //��������
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
         data = htu21d_get_data(TEMPERATURE)/100.0;                //��ȡ������ֵ
         basicRfConfig.myAddr = SEND_ADDR; 
         basicRfReceiveOff();                                        //�ر���Ƶ������
         sprintf(pTxData,"addr:0x%x, %.2f\r\n",recvice_address, data);
         printf("%s",pTxData);
         basicRfSendPacket(0xffff, (uint8*)pTxData, sizeof pTxData);  
         basicRfConfig.myAddr = RECV_ADDR;
         basicRfReceiveOn();
     }
      }
        
    */
     
 
