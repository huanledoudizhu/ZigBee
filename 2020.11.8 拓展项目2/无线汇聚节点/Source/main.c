#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>
#include "info.h"
#include "uart.h"

#define RF_CHANNEL            17                                // 2.4 GHz RF channel
#define PAN_ID                0x2000
#define SEND_ADDR             0x2530
#define RECV_ADDR             0x2520

//#define NODE_TYPE             0                                 //0:接收节点，！0：发送节点

void io_init(void)
{
  P2SEL &= ~0x01;
  P2DIR &= ~0x01;
}

static basicRfCfg_t basicRfConfig;
int count = 0;
void rfSendData(void)
{
    uint8 pTxDataNode1[] = {0x31};
     basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = SEND_ADDR;
    basicRfInit(&basicRfConfig);
    // Keep Receiver off when not needed to save power
    basicRfReceiveOff();
    basicRfSendPacket(0xffff, (uint8*)pTxDataNode1, sizeof pTxDataNode1);  
}

uint8 pRxData[128];

void rfRecvData(void)
{
  int rlen;
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = RECV_ADDR;
    basicRfInit(&basicRfConfig);
        basicRfReceiveOn();
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 ) {
                uart0_send_string((char *)pRxData);
          } 
}

uint8 ch;
void main(void)
{
    halMcuInit();
    uart0_init();
    io_init();
    hal_led_init();
    hal_uart_init();
     halRfInit();
  while(1){
    ch = uart0_recv_char();
    
    if(ch == 0x31){
      while(count != 2){
        rfSendData();
        rfRecvData();
        count++;
        halMcuWaitMs(500);
      }
      ch = 0x00;
    }
    count = 0;
    halMcuWaitMs(50);
  }
}
  
  
  
  
  
  
  
 /* 
    int rlen;
    uint8 ch;
    uint8 pRxData[128]; //接受
    uint8 pTxDataNode1[] = {0x31}; //命令
    halMcuInit();
    uart0_init();
    io_init();
    hal_led_init();
    hal_uart_init();
    //lcd_dis();
    
    if (FAILED == halRfInit()) {
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
/*    
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;
#else
    basicRfConfig.myAddr = RECV_ADDR; 
#endif
*/
  /*
        basicRfConfig.myAddr = SEND_ADDR;
        basicRfReceiveOff();
      // Keep Receiver off when not needed to save power
      while(1){
        ch = uart0_recv_char();
        while(ch != 0x31){
          ch = uart0_recv_char();
        }
      if(ch == 0x31){
       basicRfSendPacket(0xffff, (uint8*)pTxDataNode1, sizeof pTxDataNode1);  
       basicRfConfig.myAddr = RECV_ADDR; 
       ch = 0x00;
       basicRfInit(&basicRfConfig);
       basicRfReceiveOn();
       
       while(!basicRfPacketIsReady());
       rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
       if(rlen > 0) {
             uart0_send_string((char *)pRxData);
             basicRfConfig.myAddr = SEND_ADDR;
             basicRfReceiveOff();
        } 
      }
      }
}
*/
