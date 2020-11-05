#include <ioCC2530.h>

void uart0_init(void);

void uart0_send_char(char ch);

void uart0_send_string(char *data);

char uart0_recv_char(void);

void uart0_interaction(void);
