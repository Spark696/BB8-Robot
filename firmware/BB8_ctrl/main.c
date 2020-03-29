#include "io430.h"
#include "config.h"
#include "USCI.h"
#include "Timer_A.h"
//#include "IIC.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h"
//#include "ANO_Tech.h"

int main(void)
{ 
//  float temp = 0.0;
  char str[80];
//  short gx, gy, gz;
//  short ax, ay, az;
  USCIA0_Get_Flag = 0;
  USCIA0_Get_Data = 0;
  
  unsigned char CMD = 0;  //���������յĿ���ָ��
  unsigned char Speed_init = 0;  //��ʼ�ٶ�
  unsigned char Speed_control = 150;  //�����ٶ�
  unsigned char Angle_control = 90;  //���ƽǶ�
  
  WDT_init();  
  CLK_config();
  Port_init();
  UART_init();
  PWM_config();
//  IIC_init();
//  mpu6050_init();
  
  Servo_Angle_Control(Angle_control, Angle_control);
  while(1)
  {
//    LED1_ON;
//    LED2_OFF;
//    delay_ms(20);
//    LED1_OFF;
//    LED2_ON;
//    delay_ms(20);
//    UART_SendString("BB8 is working...\r\n");
    //while(mpu_dmp_init())
//    while(mpu6050_init())
//    {
//      UART_SendString("MPU6050 error !!!\r\n");  
//    }
//    UART_SendString("MPU6050 is working...\r\n");
    
//    temp = mpu6050_Get_Temperature();
//    mpu6050_Get_Gyroscope(&gx, &gy, &gz);
//    mpu6050_Get_Accelerometer(&ax, &ay, &az);
    
//    sprintf(str, "�¶�: %f\r\n", temp);
//    UART_SendString((unsigned char *)str);
//    sprintf(str, "gx: %d;gy: %d;gz: %d\r\n", gx, gy, gz);
//    UART_SendString((unsigned char *)str);
//    sprintf(str, "ax: %d;ay: %d;az: %d\r\n", ax, ay, az);
//    UART_SendString((unsigned char *)str);
    if(USCIA0_Get_Flag)  //�յ��������͵�����
    {
//      sprintf(str, "CMD: %d;Speed_init: %d;Speed_control: %d;Angle_control: %d\r\n", CMD, Speed_init, Speed_control, Angle_control);
//      UART_SendString((unsigned char *)str);      
      CMD = USCIA0_Get_Data;  //���¿���ָ��
      USCIA0_Get_Flag = 0;
    }
    else if(CMD == 'v')  //����ѹ
    {
      /***/
      CMD = 't';
    }
    else if(CMD == 'h')  //����ģʽ
    {
      Speed_control = 240;
      CMD = 't';
    }
    else if(CMD == 'm')  //����ģʽ
    {
      Speed_control = 160;
      CMD = 't';
    }
    else if(CMD == 'l')  //����ģʽ
    {
      Speed_control = 100;
      CMD = 't';
    }    
    else if(CMD == '1')  //ǰ��
    {
      Servo_Angle_Control(110, Angle_control);
      Angle_control = 110;
      
      Speed_init = 80;
      while(CMD == '1')
      {
        if(USCIA0_Get_Flag)
        {
          CMD = USCIA0_Get_Data;
          USCIA0_Get_Flag = 0;
        }
        else if(Speed_init < Speed_control)  //�ȼ���
        {
          Speed_init++;
          Motor_Speed_Control(Speed_init, Speed_init);
          delay_ms(10);
        }
        else
        {
          Motor_Speed_Control(Speed_control, Speed_control);
          delay_ms(10);
        }
      }
      while(Speed_init > 80)  //�ȼ���
      {
        Speed_init--;
        Motor_Speed_Control(Speed_init, Speed_init);
        delay_ms(10);
      }
      Servo_Angle_Control(90, Angle_control);
      Angle_control = 90;      
    }
    else if(CMD == '2')  //��ת
    {
      Motor_Speed_Control(-Speed_control, Speed_control);
      delay_ms(10);
    }
    else if(CMD == '3')  //��ת
    {
      Motor_Speed_Control(Speed_control, -Speed_control);
      delay_ms(10);
    }
    else if(CMD == '4')  //����
    {
      Servo_Angle_Control(70, Angle_control);
      Angle_control = 70;
      
      Speed_init = 80;
      while(CMD == '4')
      {
        if(USCIA0_Get_Flag)
        {
          CMD = USCIA0_Get_Data;
          USCIA0_Get_Flag = 0;
        }
        else if(Speed_init < Speed_control)  //�ȼ���
        {
          Speed_init++;
          Motor_Speed_Control(-Speed_init, -Speed_init);
          delay_ms(10);
        }
        else
        {
          Motor_Speed_Control(-Speed_control, -Speed_control);
          delay_ms(10);          
        }
      }
      while(Speed_init > 80)  //�ȼ���
      {
        Speed_init--;
        Motor_Speed_Control(-Speed_init, -Speed_init);
        delay_ms(10);
      }
      Servo_Angle_Control(90, Angle_control);
      Angle_control = 90;      
    }
    else if(CMD == '5' && Angle_control < 140)  //̧ͷ
    {
      Servo_Angle_Control(140, Angle_control);
      Angle_control = 140;      
    }
    else if(CMD == '6' && Angle_control > 50)  //��ͷ
    {
      Servo_Angle_Control(50, Angle_control);
      Angle_control = 50;       
    }
    else if(CMD == '7')
    {
      Servo_Angle_Control(90, Angle_control);
      Angle_control = 90;      
    }
    else if(CMD == '8')
    {
      /***/
    }
    else if(CMD == '9')
    {
      /***/
    }
    else
    {
      Motor_Speed_Control(0, 0);
      delay_ms(10);
    }
 
  }  
}
