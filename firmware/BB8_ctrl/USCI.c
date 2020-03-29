#include "USCI.h"

unsigned char USCIA0_Get_Flag;
unsigned char USCIA0_Get_Data;

void UART_init(void)
{
  P1DIR &= ~BIT1;  //RXD ����
  P1DIR |= BIT2;  //TXD ���
  
  P1SEL |= BIT1 + BIT2;
  P1SEL2 |= BIT1 + BIT2;  //ѡ��ڶ����ܣ�UART
  
  UCA0CTL1 |= UCSWRST;  //����ǰ�������λ
  //UCA0CTL0  //Ĭ�ϲ�������У�� 8λ���� 1λֹͣλ  
  UCA0CTL1 |= UCSSEL_2;  //SMCLKʱ�� 12MHZ
  /*
  N = F_brclk / Baud_rate = 12MHZ / 9600kps = 1250.0;
  (UCA0BR1 << 8 + UCA0BR0) = (int)N;
  UCA0BRSx = (round)(( N �C INT(N)) �� 8);
  */
  UCA0BR0 = 0xE2;
  UCA0BR1 = 0x04;
  UCA0MCTL = 0x00;
  UCA0CTL1 &= ~UCSWRST;  //��������λ
  //ME1  //msp430g2553��ģ��ʹ��
  UC0IE |= UCA0RXIE;  //ʹ�ܴ���UART_A0�����ж�
  //_BIS_SR(GIE);  
  __bis_SR_register(GIE);  //��Ӧ�������ж�
}

void UART_SendChar(unsigned char Ch)
{
  UCA0TXBUF = Ch;

  while((IFG2 & UCA0TXIFG) == 0);
  //IFG2 &= ~UCA0TXIFG;  
}

unsigned char UART_ReceiveChar(void)
{
  unsigned char Ch = 0;

  while((IFG2 & UCA0RXIFG) == 0);
  IFG2 &= ~UCA0RXIFG;
  Ch = UCA0RXBUF;
  
  return Ch;  
}

void UART_SendString(unsigned char *str)
{
  while(*str != '\0')
  {
    UART_SendChar(*str);
    str++;
  }
}

#pragma vector = USCIAB0RX_VECTOR
//__interrupt void USCIA0_RX_ISR(void)
//{
//  while((IFG2 & UCA0TXIFG) == 0);
//  //IFG2 &= ~UCA0TXIFG; 
//  UCA0TXBUF = UCA0RXBUF; 
//}
__interrupt void USCIA0_RX_ISR(void)
{
  while((IFG2 & UCA0RXIFG) == 0);
  IFG2 &= ~UCA0RXIFG;  
  if(USCIA0_Get_Flag == 0)
  {
    USCIA0_Get_Data = UCA0RXBUF;
    USCIA0_Get_Flag = 1;  //���ݽ������    
  }
}