#include "ANO_Tech.h"
#include "USCI.h"
#include "string.h"

void Send_data_to_ANO(unsigned char fun, unsigned char *data, unsigned char len)
{
  unsigned char send_buf[32];
  unsigned char i;

  if (len > 28) return;	   //���28�ֽ����� 
  send_buf[len+3] = 0;	  //У��������
  send_buf[0] = 0X88;	 //֡ͷ
  send_buf[1] = fun;	//������
  send_buf[2] = len;	//���ݳ���
  for(i=0;i<len;i++) send_buf[3+i] = data[i];			//��������
  for(i=0;i<len+3;i++) send_buf[len+3] += send_buf[i];	        //����У���	
  for(i=0;i<len+4;i++) UART_SendChar(send_buf[i]);	        //�������ݵ����� 
}

//ͨ�����ڴ��ͼ�������̬���ݸ�ANO_Tech
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void ANO_report_1(short aacx,short aacy,short aacz,short gyrox,short gyroy, \
  short gyroz,short roll,short pitch,short yaw)
{
  unsigned char tbuf[28];
  
  memset(tbuf, 0, sizeof(tbuf));
  tbuf[0] = (aacx>>8) & 0XFF;
  tbuf[1] = aacx & 0XFF;
  tbuf[2] = (aacy>>8) & 0XFF;
  tbuf[3] = aacy & 0XFF;
  tbuf[4] = (aacz>>8) & 0XFF;
  tbuf[5] = aacz & 0XFF; 
  tbuf[6] = (gyrox>>8) & 0XFF;
  tbuf[7] = gyrox & 0XFF;
  tbuf[8] = (gyroy>>8) & 0XFF;
  tbuf[9] = gyroy & 0XFF;
  tbuf[10] = (gyroz>>8) & 0XFF;
  tbuf[11] = gyroz & 0XFF;	
  tbuf[18] = (roll>>8) & 0XFF;
  tbuf[19] = roll & 0XFF;
  tbuf[20] = (pitch>>8) & 0XFF;
  tbuf[21] = pitch & 0XFF;
  tbuf[22] = (yaw>>8) & 0XFF;
  tbuf[23] = yaw & 0XFF;
  
  Send_data_to_ANO(0XAF, tbuf, 28);    //�ɿ���ʾ֡,0XAF  
}

//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void ANO_report_2(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
  unsigned char tbuf[12];
  
  tbuf[0] = (aacx>>8) & 0XFF;
  tbuf[1] = aacx & 0XFF;
  tbuf[2] = (aacy>>8) & 0XFF;
  tbuf[3] = aacy & 0XFF;
  tbuf[4] = (aacz>>8) & 0XFF;
  tbuf[5] = aacz & 0XFF; 
  tbuf[6] = (gyrox>>8) & 0XFF;
  tbuf[7] = gyrox & 0XFF;
  tbuf[8] = (gyroy>>8) & 0XFF;
  tbuf[9] = gyroy & 0XFF;
  tbuf[10] = (gyroz>>8) & 0XFF;
  tbuf[11] = gyroz & 0XFF;
  
  Send_data_to_ANO(0XA1, tbuf, 12);    //�Զ���֡,0XA1  
}

