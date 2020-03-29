#include "mpu6050.h"
#include "config.h"

//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
unsigned char mpu6050_init(void)
{
  unsigned char res;
  
  IIC_init();
  mpu6050_write_byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  delay_ms(100);
  mpu6050_write_byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050
  mpu6050_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
  mpu6050_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
  mpu6050_Set_Rate(50);						//���ò�����50Hz
  mpu6050_write_byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
  mpu6050_write_byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
  mpu6050_write_byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
  mpu6050_write_byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
  res = mpu6050_read_byte(MPU_DEVICE_ID_REG);
  if(res == MPU_ADDR)   //����ID��ȷ
  {
    mpu6050_write_byte(MPU_PWR_MGMT1_REG,0X01);	   //����CLKSEL,PLL X��Ϊ�ο�
    mpu6050_write_byte(MPU_PWR_MGMT2_REG,0X00);	   //���ٶ��������Ƕ�����
    mpu6050_Set_Rate(50);     //���ò�����Ϊ50Hz
  }else return 1;
  return 0;  
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
unsigned char mpu6050_Set_Gyro_Fsr(unsigned char fsr)
{
  return mpu6050_write_byte(MPU_GYRO_CFG_REG,fsr<<3);   //���������������̷�Χ  
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
unsigned char mpu6050_Set_Accel_Fsr(unsigned char fsr)
{
  return mpu6050_write_byte(MPU_ACCEL_CFG_REG,fsr<<3);   //���ü��ٶȴ����������̷�Χ  
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
unsigned char mpu6050_Set_LPF(unsigned int lpf)
{
  unsigned char data=0;
  
  if(lpf >= 188)data = 1;
  else if(lpf >= 98)data = 2;
  else if(lpf >= 42)data = 3;
  else if(lpf >= 20)data = 4;
  else if(lpf >= 10)data = 5;
  else data = 6; 
  return mpu6050_write_byte(MPU_CFG_REG,data);   //�������ֵ�ͨ�˲���  
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
unsigned char mpu6050_Set_Rate(unsigned int rate)
{
  unsigned char data;
  if(rate > 1000)rate = 1000;
  if(rate < 4)rate = 4;
  data = 1000/rate - 1;
  data = mpu6050_write_byte(MPU_SAMPLE_RATE_REG,data);	     //���ò�����
  return mpu6050_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ
float mpu6050_Get_Temperature(void)
{
  unsigned char buf[2]; 
  short raw;
  float temp;
  mpu6050_read_len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
  raw = ((unsigned char)buf[0] << 8) | buf[1];  
  temp = 36.53 + ((double)raw) / 340;  
  return temp;
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
unsigned char mpu6050_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  unsigned char buf[6],res;  
  res = mpu6050_read_len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
  if(res == 0)
  {
    *gx = ((unsigned int)buf[0] << 8) | buf[1];  
    *gy = ((unsigned int)buf[2] << 8) | buf[3];  
    *gz = ((unsigned int)buf[4] << 8) | buf[5];
  } 	
  return res;
}

//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
unsigned char mpu6050_Get_Accelerometer(short *ax,short *ay,short *az)
{
  unsigned char buf[6],res;  
  res = mpu6050_read_len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
  if(res == 0)
  {
    *ax = ((unsigned int)buf[0] << 8) | buf[1];  
    *ay = ((unsigned int)buf[2] << 8) | buf[3];  
    *az = ((unsigned int)buf[4] << 8) | buf[5];
  } 	
  return res;
}

//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
unsigned char mpu6050_write_byte(unsigned char reg, unsigned char data)
{
  IIC_Start();
  IIC_Send_Byte((MPU_ADDR<<1)|0);  //����������ַ+д����
  if(IIC_Wait_Ack())  //�ȴ�Ӧ��
  {
    IIC_Stop();
    return 1;
  }
  IIC_Send_Byte(reg);    //д�Ĵ�������
  IIC_Wait_Ack();        //�ȴ�Ӧ��
  IIC_Send_Byte(data);   //��������
  if(IIC_Wait_Ack())
  {
    IIC_Stop();
    return 1;    
  }
  IIC_Stop();
  return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
unsigned char mpu6050_read_byte(unsigned char reg)
{
  unsigned char res;
  
  IIC_Start();
  IIC_Send_Byte((MPU_ADDR<<1)|0);  //����������ַ+д����
  IIC_Wait_Ack();        //�ȴ�Ӧ��
  IIC_Send_Byte(reg);    //д�Ĵ�����ַ
  IIC_Wait_Ack();        //�ȴ�Ӧ��
  IIC_Start();
  IIC_Send_Byte((MPU_ADDR<<1)|1);  //����������ַ+������
  IIC_Wait_Ack();        //�ȴ�Ӧ��

  res = IIC_Read_Byte(0);        //��ȡ����,����NAck
  IIC_Stop();        //����һ��ֹͣ����

  return res;
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
unsigned char mpu6050_write_len(unsigned char addr, unsigned char reg, \
                unsigned char len, unsigned char *buf)
{
  unsigned char i = 0;
  
  IIC_Start();
  IIC_Send_Byte((addr<<1)|0);  //����������ַ+д����
  if(IIC_Wait_Ack())  //�ȴ�Ӧ��
  {
    IIC_Stop();
    return 1;
  }
  IIC_Send_Byte(reg);  //д�Ĵ�����ַ
  IIC_Wait_Ack();  //�ȴ�Ӧ��
  for(i=0; i<len; i++)
  {
    IIC_Send_Byte(buf[i]);  //��������
    if(IIC_Wait_Ack())  //�ȴ�Ack
    {
      IIC_Stop();
      return 1;
    }
  }
  IIC_Stop();
  return 0;
}

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
unsigned char mpu6050_read_len(unsigned char addr, unsigned char reg, \
                unsigned char len, unsigned char *buf)
{
  IIC_Start();
  IIC_Send_Byte((addr<<1)|0);  //����������ַ+д����
  if(IIC_Wait_Ack())  //�ȴ�Ӧ��
  {
    IIC_Stop();
    return 1;
  }
  IIC_Send_Byte(reg);    //д�Ĵ�����ַ
  IIC_Wait_Ack();        //�ȴ�Ӧ��
  IIC_Start();
  IIC_Send_Byte((addr<<1)|1);  //����������ַ+������
  IIC_Wait_Ack();        //�ȴ�Ӧ�� 
  while(len)
  {
    if(len == 1)
    {
      *buf = IIC_Read_Byte(0);  //�����ݣ�����NAck
    }
    else
    {
      *buf = IIC_Read_Byte(1);  //�����ݣ�����Ack
    }
    len--;
    buf++;
  }
  IIC_Stop();       //����һ��ֹͣ����
  return 0;
}



