#include <ioCC2530.h>
#include "clock.h"
#include "uart.h"

char recvBuf[256];
int recvCnt = 0;	

void uart0_init(void){
  PERCFG = 0x00;          //设置UART0的备用位置为P0口，即UART0的RX-P0_2，TX-P0_3
  P0SEL  = 0x3c;          //设置P0_2，P0_3为外设功能
  P2DIR &= ~0xc0;         //配置P0端口优先作为UART0
  
  /*设置USART0的方式*/
  U0CSR |= 0x80;          //SPI同步模式 
  
  U0GCR = 10;    
  U0BAUD = 59;    //设置波特率为38400bps
  
  U0UCR |= 0x00; //1位停止位，无奇偶校验位
  
  U0CSR |= 0x40;
}

void uart0_send_char(char ch){
  U0DBUF = ch; //将字符参数的值写入到U1DBUF中
  while(UTX0IF == 0);  //查询UTX1IF（中断），如果值为1，则代表发送完成
  UTX0IF = 0; //发送完成后清零UTX1IF，为下个字符传输做准备
}

void uart0_send_string(char *data){
  while(*data != '\0'){
    uart0_send_char(*data++);
    //uart1_send_char(*data)
    //data++;  地址+1向后移
  }
}

char uart0_recv_char(void){     //Input by Users端
  char ch;
  while(URX0IF == 0); //查询URX1IF，如果值为1，表示接受完毕
  ch = U0DBUF;
  URX0IF = 0;
  return ch;
}

void uart0_interaction(void){
  char ch;
  ch = uart0_recv_char();
  if(ch == '@' || recvCnt >= 255){ //因为最后设置发送位时（指向）下一位，而此时的"@"并没有存入，所以检测前两（三）位
    
     recvBuf[recvCnt] = 0;   //添加字符串末尾结束标志
     
    if(recvBuf[0] == '1' && recvCnt == 2){   //如果是ON@，加完末尾结束符时的recvCnt 为 3
      
      uart0_send_string("LED1 is light!"); //开始传输亮提醒
      
    } else if(recvBuf[0] == '2'  && recvCnt == 2){  //如果是OFF@，加完末尾结束符时的recvCnt 为 4
      
      uart0_send_string("LED1 is OFF!"); //开始传输灭提醒
      
    } else if(recvBuf[0] == '3'  && recvCnt == 2){
      
      uart0_send_string("LED1 is OFF!"); //开始传输灭提醒
      
    }
    uart0_send_string("\r\n"); //传输换行符
    recvCnt = 0; //清零
  } else {
    recvBuf[recvCnt++] = ch; //设置接收数组
  }
}