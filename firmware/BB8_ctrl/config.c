#include "config.h"

//ϵͳʱ������
void CLK_config(void)
{ 
  int i = 0;
  BCSCTL1 = CALBC1_12MHZ;  //��Ϣ��A��DCOУ������ RSELx  
  DCOCTL = CALDCO_12MHZ;  //��Ϣ��A��DCOУ������ DCOx MODx  
  BCSCTL2 |= SELM_0 + DIVM_0 + DIVS_0;  //MCLK SMCLK: DCO = 12MHZ ����Ƶ
  BCSCTL3 |= LFXT1S_2;  //��Ƶʱ��ѡ��ΪVLOCLK = 12KHZ
   
  while((IFG1&OFIFG) != 0)
  {
    IFG1 &= ~OFIFG;  //���OSCFault ��־
    for(i=0xff; i>0; i--);  //��ʱ���ȶ�
  }
}

//���Ź���ʼ��
void WDT_init(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;  
}

//�˿ڳ�ʼ��
void Port_init(void)
{
  P1DIR |= BIT0 + BIT6;                            // Set P1.0 P1.6 to output direction
  P1OUT &= ~(BIT0 + BIT6);
  P1OUT &= ~BIT0;
  P1OUT |= BIT6;
}


void Delay_nms(unsigned int n)
{
  for(unsigned int i=0; i<n; i++)
  {
    delay_ms(1);
  }
}