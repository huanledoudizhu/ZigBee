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

#define NODE_TYPE             0                                 //0:接收节点，！0：发送节点

void io_init(void)
{
  P2SEL &= ~0x01;
  P2DIR &= ~0x01;
}

static basicRfCfg_t basicRfConfig;
int ledstatus = 0;
void rfSendData(void)
{
    uint8 ch;
    uint8 pTxDataNode1[] = {0x31};
    uint8 pTxDataNode2[] = {0x32};
    uint8 pTxDataNode3[] = {0x33};
    // Keep Receiver off when not needed to save power
    basicRfReceiveOff();
    // Main loop
    while (TRUE) {
      ch = uart0_recv_char();
      switch(ch){
      case 0x31:
        basicRfSendPacket(0xffff, (uint8*)pTxDataNode1, sizeof pTxDataNode1);  
        break;
        
      case 0x32:
          hal_led_on(1);
          basicRfSendPacket(0xffff, (uint8*)pTxDataNode2, sizeof pTxDataNode2);  
          hal_led_off(1);
          break;
      
      case 0x33:
          hal_led_on(1);
          basicRfSendPacket(0xffff, (uint8*)pTxDataNode3, sizeof pTxDataNode3);  
          hal_led_off(1);
          break;
      }
        halMcuWaitMs(50);
    }
}

void rfRecvData(void)
{
  uint8 pRxData[128];
  int rlen;
   basicRfReceiveOn();    
    // Main loop
    while (TRUE) {
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 && pRxData[0] == 0x33) {
            hal_led_on(1);  
            printf("123");
          } 
    }
}

void main(void)
{
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
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;
#endif

    
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;
#else
    basicRfConfig.myAddr = RECV_ADDR; 
#endif
    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
#if NODE_TYPE
  rfSendData();
#else
  rfRecvData();   
#endif
}
