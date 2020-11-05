#include <ioCC2530.h>
#include "clock.h"
#include "uart.h"

char recvBuf[256];
int recvCnt = 0;	

void uart0_init(void){
  PERCFG = 0x00;          //����UART0�ı���λ��ΪP0�ڣ���UART0��RX-P0_2��TX-P0_3
  P0SEL  = 0x3c;          //����P0_2��P0_3Ϊ���蹦��
  P2DIR &= ~0xc0;         //����P0�˿�������ΪUART0
  
  /*����USART0�ķ�ʽ*/
  U0CSR |= 0x80;          //SPIͬ��ģʽ 
  
  U0GCR = 10;    
  U0BAUD = 59;    //���ò�����Ϊ38400bps
  
  U0UCR |= 0x00; //1λֹͣλ������żУ��λ
  
  U0CSR |= 0x40;
}

void uart0_send_char(char ch){
  U0DBUF = ch; //���ַ�������ֵд�뵽U1DBUF��
  while(UTX0IF == 0);  //��ѯUTX1IF���жϣ������ֵΪ1������������
  UTX0IF = 0; //������ɺ�����UTX1IF��Ϊ�¸��ַ�������׼��
}

void uart0_send_string(char *data){
  while(*data != '\0'){
    uart0_send_char(*data++);
    //uart1_send_char(*data)
    //data++;  ��ַ+1�����
  }
}

char uart0_recv_char(void){     //Input by Users��
  char ch;
  while(URX0IF == 0); //��ѯURX1IF�����ֵΪ1����ʾ�������
  ch = U0DBUF;
  URX0IF = 0;
  return ch;
}

void uart0_interaction(void){
  char ch;
  ch = uart0_recv_char();
  if(ch == '@' || recvCnt >= 255){ //��Ϊ������÷���λʱ��ָ����һλ������ʱ��"@"��û�д��룬���Լ��ǰ��������λ
    
     recvBuf[recvCnt] = 0;   //����ַ���ĩβ������־
     
    if(recvBuf[0] == '1' && recvCnt == 2){   //�����ON@������ĩβ������ʱ��recvCnt Ϊ 3
      
      uart0_send_string("LED1 is light!"); //��ʼ����������
      
    } else if(recvBuf[0] == '2'  && recvCnt == 2){  //�����OFF@������ĩβ������ʱ��recvCnt Ϊ 4
      
      uart0_send_string("LED1 is OFF!"); //��ʼ����������
      
    } else if(recvBuf[0] == '3'  && recvCnt == 2){
      
      uart0_send_string("LED1 is OFF!"); //��ʼ����������
      
    }
    uart0_send_string("\r\n"); //���任�з�
    recvCnt = 0; //����
  } else {
    recvBuf[recvCnt++] = ch; //���ý�������
  }
}