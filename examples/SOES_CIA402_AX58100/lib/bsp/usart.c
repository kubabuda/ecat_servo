#include "usart.h"


USART_InitTypeDef USART_InitStructure;


int __io_putchar(int c) {
	while (USART_GetFlagStatus(APP_USART, USART_FLAG_TXE) == RESET) {}
	USART_SendData(APP_USART, c);

	return c;
}


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}


// Hack to hide botched printf("%d") on platformio SPL
// We can printf up to 5 integers wrapped in this
#define INTS_IN_PRINTF 5
char buffer[INTS_IN_PRINTF][INTS_IN_PRINTF];
// This is ...not great, but works for now and its planned to switch to different libraries
// that hopefully dont have this problem

char* STR5(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer[index ++ % INTS_IN_PRINTF];

    s[0] = (val / 1000) + ASCII_0;
    s[1] = ((val % 1000) / 100) + ASCII_0;
    s[2] = ((val % 100) / 10) + ASCII_0;
    s[3] = ((val % 10)) + ASCII_0;
    s[4] = '\0';

    return s;
}

char* DEC4B(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer[index ++ % INTS_IN_PRINTF];

    s[0] = (val / 10000) + ASCII_0;
    s[1] = ((val % 10000) / 1000) + ASCII_0;
    s[2] = ((val % 1000) / 100) + ASCII_0;
    s[3] = ((val % 100) / 10) + ASCII_0;
    s[4] = ((val % 10)) + ASCII_0;
    s[5] = '\0';

    return s;
}


char buffer_hex[INTS_IN_PRINTF][7];

char C(uint8_t i) {
  
  switch (i){
    case 0xF: return 'F';
    case 0xE: return 'E';
    case 0xD: return 'D';
    case 0xC: return 'C';
    case 0xB: return 'B';
    case 0xA: return 'A';
    default: return (i % 0xA) + ASCII_0;    
  }
}

char* HEX4B(uint32_t val) {
    #define ASCII_0  48
    static uint8_t index = 0;

    char* s = buffer_hex[index ++ % INTS_IN_PRINTF];

    s[0] = '0';
    s[1] = 'x';
    s[2] = C(val / 0x1000);
    s[3] = C((val % 0x1000) / 0x100);
    s[4] = C((val % 0x100) / 0x10);
    s[5] = C((val % 0x10));
    s[6] = '\0';

    return s;
}

void USART_COMInit(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(APP_USART_TX_GPIO_CLK | APP_USART_RX_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(APP_USART_CLK, ENABLE);
  // RCC_APB1PeriphClockCmd(APP_USART_CLK, ENABLE);

  GPIO_PinAFConfig(APP_USART_TX_GPIO_PORT, APP_USART_TX_SOURCE, APP_USART_TX_AF);
  GPIO_PinAFConfig(APP_USART_RX_GPIO_PORT, APP_USART_RX_SOURCE, APP_USART_RX_AF);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = APP_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(APP_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = APP_USART_RX_PIN;
  GPIO_Init(APP_USART_RX_GPIO_PORT, &GPIO_InitStructure);

  USART_Init(APP_USART, USART_InitStruct);
  USART_Cmd(APP_USART, ENABLE);
}


void APP_USART_Init() {
 /* USARTx configured as follow:
  - BaudRate = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  // TX on PC6

  USART_COMInit(&USART_InitStructure);
}


void APP_USART_Send(char *c)
{
	int i = 0;
	while(c[i] != '\0')
	{
		 __io_putchar((int)c[i]);
		i++;
	}
}
