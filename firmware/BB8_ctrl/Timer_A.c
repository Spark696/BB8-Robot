#include "Timer_A.h"
#include "config.h"

void PWM_config(void)
{
  /***���ƶ˿����� ***/
  //ENA
  //ENB
  //��ӦIN1��IN2��IN3��IN4
  P2DIR |= BIT0 + BIT1 + BIT3 + BIT5;
  P2OUT |= BIT0 + BIT1 + BIT3 + BIT5;
  
  /***TA1 2·PWM����***/
  P2DIR |= BIT2 + BIT4;  //���ģʽ
  P2SEL |= BIT2 + BIT4;  //P2.2 -> TA1.1 P2.4 -> TA1.2 �Ƚ����
  P2SEL2 &= ~(BIT2 + BIT4);
  
  TA1CTL |= TASSEL_2 + ID_2 + MC_0 + TACLR;  //ֹͣģʽ������: MCLK,4��Ƶ,=3MHZ���TAR
  TA1CCR0 = 60000;
  TA1CCTL1 = OUTMOD_7;  //PWM��λ/��λ
  TA1CCR1 = 0;
  TA1CCTL2 = OUTMOD_7;
  TA1CCR2 = 0;
  
  TA1CTL |= MC_1;  //������ģʽ

  /***TA0 1·PWM����***/  
  P2DIR |= BIT6;  //���ģʽ
  P2SEL |= BIT6;  //P2.6 -> TA0.1 �Ƚ����
  P2SEL &= ~BIT7;
  P2SEL2 &= ~(BIT6 + BIT7);
  
  TA0CTL |= TASSEL_2 + ID_2 + MC_0 + TACLR;  //ֹͣģʽ������: MCLK,4��Ƶ,=3MHZ���TAR
  TA0CCTL0 |= CCIE;  // ����/�Ƚ��жϿ���
  TA0CCR0 = 60000;  //����: 3MHZ / 60000 = 50HZ
  TA0CCTL1 = OUTMOD_7;  //PWM��λ/��λ
  TA0CCR1 = 4500;
  
  TA0CTL |= MC_1;  //������ģʽ  
}

//�޸�TA1 ���PWM��ռ�ձ�
void PWM_Control_1(unsigned int Val)
{
//  TA1CTL |= MC_0 + TACLR;   //��ͣ���������
  TA1CCR1 = Val; 
  TA1CCR2 = Val;
  
  TA1CTL |= MC_1;  //������ģʽ
}

//�޸�TA0 ���PWM��ռ�ձ�
void PWM_Control_0(unsigned int Val)
{
  TA0CTL |= MC_0 + TACLR;   //��ͣ���������
  TA0CCR1 = Val; 
  
  TA0CTL |= MC_1;  //������ģʽ
}

unsigned char Capture_count = 0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  if(Capture_count < 50)
  {
    Capture_count++;
  }
  else
  {
    P1OUT ^= BIT0;
    P1OUT ^= BIT6;    
    Capture_count = 0;  
  }
}

//------------------------------
/***���ֶ�� & ˫������***/
//------------------------------

//���ֶ���Ƕȿ���: 0�� -> 180��
//PWM: ����: 50HZ,��20ms   ���Ʒ�Χ: 500us -> 2500us
//angle: ���ƽǶ�     preangle: ��ǰ�Ƕ�  1�ȶ�Ӧ���� = (7500 - 1500) / 180 = 33.3333333 
void Servo_Angle_Control(unsigned char angle, unsigned char preangle)
{
  unsigned char angle_temp;
  
  angle_temp = preangle;
  if(preangle < angle)
  {
    while(angle_temp < angle)
    {
      angle_temp++;
      PWM_Control_0((unsigned int)(1500 + 33.3333333 * angle_temp));
      delay_ms(20);
    }
  }
  else if(preangle > angle)
  {
    while(angle_temp > angle)
    {
      angle_temp--;
      PWM_Control_0((unsigned int)(1500 + 33.3333333 * angle_temp));
      delay_ms(20);
    }    
  }
  else  //���ʱֱ�ӿ���
  {
    PWM_Control_0((unsigned int)(1500 + 33.3333333 * angle_temp));
    delay_ms(20);
  }
}

//�ٶȷ�Χ: 0 -> 255
void Motor_Speed_Control(int vL, int vR)
{
  if(vL >= 0 && vR >= 0)  //ǰ��
  {
    IN1_0; IN2_1;
    IN3_0; IN4_1;
  }
  else if(vL >= 0 && vR < 0)  //��ת
  {
    IN1_1; IN2_0;
    IN3_0; IN4_1;    
  }
  else if(vL < 0 && vR >= 0)  //��ת
  {
    IN1_0; IN2_1;
    IN3_1; IN4_0;    
  }
  else if(vL < 0 && vR < 0)  //����
  {
    IN1_1; IN2_0;
    IN3_1; IN4_0;    
  }
  
  if(vL < 0)  //��vR
  {
    vL = -vL;
  }
  
  vL = (int)((float)vL / 255 * 60000);  //����50HZ PWM��ռ�ձ�
  PWM_Control_1(vL);
}

