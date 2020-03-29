#include "IIC.h"
#include "config.h"

void IIC_init(void)
{
  P1DIR |= BIT4 + BIT5;
  //P1REN |= BIT4 + BIT5;  //����/��������ʹ��
  P1OUT |= BIT4 + BIT5;  //��Ӧ��������
}

void IIC_Start(void)
{
  Set_IIC_SDA_out();  //SDA ���
  IIC_SDA_1;
  IIC_SCL_1;
  IIC_delay();
  IIC_SDA_0;  //SCL = 1ʱ��SDA�����½���
  IIC_delay();
  IIC_SCL_0;  //ǯסIIC���ߣ�׼�����ͻ��������  
}

void IIC_Stop(void)
{
  Set_IIC_SDA_out();  //SDA ���
  IIC_SCL_0;  
  IIC_SDA_0;
  IIC_delay();
  IIC_SCL_1;
  IIC_SDA_1;  //SCL = 1ʱ��SDA����������
  IIC_delay();  
}

unsigned char IIC_Wait_Ack(void)
{
  unsigned char ucErrTime = 0;
    
  Set_IIC_SDA_in();
  IIC_SDA_1;  //������������
  //Set_IIC_SDA_in();
  IIC_delay();
  IIC_SCL_1; 
  IIC_delay();
  
  while(IIC_SDA_IN)
  {
    ucErrTime++;
    if(ucErrTime > 250)
    {
      IIC_Stop();
      return 1;
    }    
  }
  IIC_SCL_0;
  return 0;
}

void IIC_Ack(void)
{
  IIC_SCL_0;
  Set_IIC_SDA_out();
  IIC_SDA_0;
  IIC_delay();
  IIC_SCL_1;
  IIC_delay();
  IIC_SCL_0;  
}

void IIC_NAck(void)
{
  IIC_SCL_0;
  Set_IIC_SDA_out();
  IIC_SDA_1;
  IIC_delay();
  IIC_SCL_1;
  IIC_delay();
  IIC_SCL_0;  
}

void IIC_Send_Byte(unsigned char sdata)
{
  unsigned char t = 0;

  Set_IIC_SDA_out();
  IIC_SCL_0;
  for(t=0; t<8; t++)
  {
    if(sdata & 0x80)
    {
      IIC_SDA_1;
    }
    else
    {
      IIC_SDA_0;
    }
    sdata <<= 1;
    IIC_SCL_1;
    IIC_delay();
    IIC_SCL_0;
    IIC_delay();
  }
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������NACK
unsigned char IIC_Read_Byte(unsigned char ack)
{
  unsigned char i, rdata = 0;
  
  //IIC_SDA_1;
  Set_IIC_SDA_in();
  IIC_SDA_1;
  for(i=0; i<8; i++)
  {
    IIC_SCL_0;
    IIC_delay();
    IIC_SCL_1;
    rdata <<= 1;
    if(IIC_SDA_IN)
    {
      rdata++;  
    }
    IIC_delay();
  }
  if(ack)
  {
    IIC_Ack();  
  }
  else
  {
    IIC_NAck();
  }
  return rdata;
}

void Set_IIC_SDA_out(void)
{
  P1DIR |= BIT5;  //SDA ���
}

void Set_IIC_SDA_in(void)
{
  P1DIR &= ~BIT5;  //SDA ����  
}

void IIC_delay(void)
{
  delay_us(2);  
}