/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project :
Version :
Date    : 8/15/2018
Author  :
Company :
Comments:


Chip type               : ATmega32A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32a.h>
#include <i2c.h>
#include <stdio.h>
#include <math.h>
#include <delay.h>
/*
 * mpu6050.h
 *
 *  Created on: Aug 14, 2018
 *      Author: ali
 */




#define MPU_ADDRESS 0xD0
#define MPU_WHO_AM 0x75
#define GYRO_CONFIG 0x1B
#define SLEEP_CONFIG 0x6B
#define SAMPLE_RATE 0x19
#define USER_CONTROL 0x6A

#define G_X_H 0x43
#define G_X_L 0x44
#define G_Y_H 0x45
#define G_Y_L 0x46
#define G_Z_H 0x47
#define G_Z_L 0x48

#define A_X_H 0x3B
#define A_X_L 0x3C
#define A_Y_H 0x3D
#define A_Y_L 0x3E
#define A_Z_H 0x3F
#define A_Z_L 0x40

#define GRYRO_SCALE 131
#define ACCEL_SCALE 100

#define TIME 0.00012799
#define PRE 131
#define OFFSET 120
#define M_PI 3.1415


unsigned int  mpu6050_get_accel_z();
unsigned int mpu6050_get_accel_x();
unsigned int mpu6050_get_accel_y();

unsigned int mpu6050_get_gyro_z();
unsigned int mpu6050_get_gyro_y();
unsigned int mpu6050_get_gyro_x();

void mpu6050_init();

void WriteModule(unsigned char Address,unsigned char reg,unsigned char value)
{
    i2c_start();
    i2c_write(Address);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}
unsigned int ReadModule(unsigned char Address,unsigned char Reg)
{
    unsigned int Data;

    i2c_start();
    i2c_write(Address);
    i2c_write(Reg);
    i2c_start();
    i2c_write(Address+1);
    Data=i2c_read(1);
    Data=Data*256+i2c_read(0);
    i2c_stop();
    return Data;
}
void mpu6050_init()
{
    WriteModule(MPU_ADDRESS,SLEEP_CONFIG,0);
    WriteModule(MPU_ADDRESS,GYRO_CONFIG,0);
    WriteModule(MPU_ADDRESS,SAMPLE_RATE,0);
    WriteModule(MPU_ADDRESS,26,0);
    WriteModule(MPU_ADDRESS,27,0);
    WriteModule(MPU_ADDRESS,28,0);


}
unsigned int mpu6050_get_gyro_x()
{
    return ReadModule(MPU_ADDRESS,G_X_H);
}
unsigned int mpu6050_get_gyro_y()
{
    return ReadModule(MPU_ADDRESS,G_Y_H);
}
unsigned int mpu6050_get_gyro_z()
{
    return ReadModule(MPU_ADDRESS,G_Z_H);
}
unsigned int mpu6050_get_accel_x()
{
    return ReadModule(MPU_ADDRESS,A_X_H);
}
unsigned int mpu6050_get_accel_y()
{
    return ReadModule(MPU_ADDRESS,A_Y_H);
}
unsigned int mpu6050_get_accel_z()
{
    return ReadModule(MPU_ADDRESS,A_Z_H);
}

void my_putstr(char *p)
{
    int i=0;
    for(;p[i]!='\0';i++)
    {
        putchar(p[i]);
    }
}

void my_put_int(int number)
{
    char f[50];
    sprintf(f,"%d",number);
    my_putstr(f);

}
void my_put_float(float number,int div)
{

    int i=0;
    float temp=number-(int)number;
    my_put_int((int)number);
    putchar('.');
    for(;i<div;i++)
    {
        if((temp*10)>1)
            (my_put_int(temp*10));
        else
            my_put_int(0);
        temp*=10;
         temp=temp-(int)temp;
    }
}
void ena_int();
void dis_int();
//void gyro_cal(int OFF_X,int OFF_Y,int OFF_Z);
//void accel_cal_2();
//void accel_cal(int OFF_X,int OFF_Y,int OFF_Z);
void exception(unsigned char *string,float number,int div);
void mpu_calibrate(int *OFFSET_X,int *OFFSET_Y,int *OFFSET_Z);
//float mabs(float numb) ;
void fuse_data(int OFF_X,int OFF_Y,int OFF_Z);


float g_x;
float g_y;
float g_z;

float a_x;
float a_y;
float a_z;

float degree_x=0;
float degree_y=0;
float degree_z=0;
float deg_accel_x_2=0;
float deg_accel_y_2=0;
float deg_accel_z_2=0;

int counter=0;
float fused_x=0;
float fused_y=0;
float fused_z=0;



interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
TCCR0=0;

counter++;
    if(counter>30000)
    {
           counter=0;
    }
    exception("counter",counter,0);


TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
}


#define _EXCEPTION_ON 1

void main(void)
{


int counter_temp=0;
int OFF_X,OFF_Y,OFF_Z;

DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);


// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7.813 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 32.768 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

// Bit-Banged I2C Bus initialization
// I2C Port: PORTC
// I2C SDA bit: 1
// I2C SCL bit: 0
// Bit Rate: 100 kHz
// Note: I2C settings are specified in the
// Project|Configure|C Compiler|Libraries|I2C menu.
i2c_init();

// Global enable interrupts
my_putstr("-------------------");
my_putstr("\nsalam chetro\n");
mpu6050_init();

mpu_calibrate(&OFF_X,&OFF_Y,&OFF_Z);
#asm("sei")
while (1)
      {

        if(counter_temp>1)
        {
            //my_put_int(degree_x);
            //my_putstr("\t");
            //my_put_int(deg_accel_x_2);
            //my_putstr("\t");
            my_put_int(degree_z);
            my_putstr("\t");
            my_put_int(deg_accel_z_2);
            my_putstr("\t");
            my_put_int(fused_z);
            my_putstr("\t");
            my_putstr("\n");

            counter_temp=0;
        }
        else
            counter_temp++;

        fuse_data(OFF_X,OFF_Y,OFF_Z);


    }
}
void fuse_data(int OFF_X,int OFF_Y,int OFF_Z)
{

    int t_x,t_y,t_z;
    int p_x,p_y,p_z;
    t_x=g_x;
    t_y=g_y;
    t_z=g_z;

    p_x=a_x;
    p_y=a_y;
    p_z=a_z;

    g_x=(float)mpu6050_get_gyro_x()-OFF_X;
    g_y=(float)mpu6050_get_gyro_y()-OFF_Y;
    g_z=(float)mpu6050_get_gyro_z()-OFF_Z;

    a_x=mpu6050_get_accel_x();
    a_y=mpu6050_get_accel_y();
    a_z=mpu6050_get_accel_z();

    g_x=(int)g_x/GRYRO_SCALE;
    g_y=(int)g_y/GRYRO_SCALE;
    g_z=(int)g_z/GRYRO_SCALE;

    g_x=t_x*.2+g_x*.8;
    g_y=t_y*.2+g_y*.8;
    g_z=t_z*.2+g_z*.8;

    a_x/=100;
    a_y/=100;//because of avr variable size i destroyed my precision
    a_z/=100;

    a_x=a_x*.4+p_x*.6;
    a_y=a_y*.4+p_y*.6;
    a_z=a_z*.4+p_z*.6 ;

    deg_accel_z_2= atan2(a_y,a_x)*180/M_PI; //xy plane
    deg_accel_x_2= atan2(a_y,a_z)*180/M_PI;//yz plane
    deg_accel_y_2= atan2(a_x,a_z)*180/M_PI;//zx plane

    deg_accel_x_2=(deg_accel_x_2>0)?deg_accel_x_2:360+deg_accel_x_2;
    deg_accel_y_2=(deg_accel_y_2>0)?deg_accel_y_2:360+deg_accel_y_2;
    //deg_accel_z_2=(deg_accel_z_2>0)?deg_accel_z_2:360+deg_accel_z_2;
    deg_accel_y_2=360-deg_accel_y_2;
    //deg_accel_z_2=360-deg_accel_z_2;


    dis_int();


    fused_x=(((float)g_x*(float)((long)counter*256+(TCNT0))*(float)TIME)+fused_x)*.96+deg_accel_x_2*.04;
    fused_y=(((float)g_y*(float)((long)counter*256+(TCNT0))*(float)TIME)+fused_y)*.96+deg_accel_y_2*.04;
    fused_z=(((float)g_z*(float)((long)counter*256+(TCNT0))*(float)TIME)+fused_z)*.96+deg_accel_z_2*.04;


    degree_x+=((float)g_x*(float)((long)counter*256+(TCNT0))*(float)TIME);//0.000000977022=;
    degree_y+=((float)g_y*(float)((long)counter*256+(TCNT0))*(float)TIME);//0.000000977022=TIME/GRYRO_SCALE;
    degree_z+=((float)g_z*(float)((long)counter*256+(TCNT0))*(float)TIME);//0.000000977022=TIME/GRYRO_SCALE;

    counter=0;
    ena_int();
    while((fused_x>360)||(fused_z>360)||(fused_y>360))
    {
        if(fused_x>360){
            //putchar('c');
            fused_x=fused_x-360;
        }
        if(fused_y>360){
            //putchar('d');
            fused_y=fused_y-360;
        }
        if(fused_z>360){
            fused_z=fused_z-360;
            //exception("z",fused_z,0);
        }

    }
    while((fused_x<0)||(fused_y<0)||(fused_z<0))
    {
        if(fused_x<0)
            fused_x=360+fused_x;
        if(fused_y<0)
            fused_y=360+fused_y;
        if(fused_z<0)
            fused_z=360+fused_z;

    }

    while((degree_x>360)||(degree_y>360)||(degree_z>360))
    {
        if(degree_x>360)
            degree_x=degree_x-360;
        if(degree_y>360)
            degree_y=degree_y-360;
        if(degree_z>360)
            degree_z=degree_z-360;
    }
    while((degree_x<0)||(degree_z<0)||(degree_y<0))
    {
        if(degree_x<0)
            degree_x=360+degree_x;
        if(degree_y<0)
            degree_y=360+degree_y;
        if(degree_z<0)
            degree_z=360+degree_z;
    }




}
void mpu_calibrate(int *OFFSET_X,int *OFFSET_Y,int *OFFSET_Z)
{
        int i=0;

    my_putstr("\n----------start_calibratin-------\n");

    for(;i<500;i++)
    {
           *OFFSET_X=(float)mpu6050_get_gyro_x();
           *OFFSET_Y=(float)mpu6050_get_gyro_y();
           *OFFSET_Z=(float)mpu6050_get_gyro_z();
    }
    i=0;
    for(;i<50;i++)
    {
           *OFFSET_X+=mpu6050_get_gyro_x();
           *OFFSET_Y+=mpu6050_get_gyro_y();
           *OFFSET_Z+=mpu6050_get_gyro_z();
    }
    *OFFSET_X/=50;
    *OFFSET_Y/=50;
    *OFFSET_Z/=50;

    exception("G_x_OFFSET",*OFFSET_X,0);
    exception("G_y_OFFSET",*OFFSET_Y,0);
    exception("G_z_OFFSET",*OFFSET_Z,0);

    my_putstr("\n----------finish_calibratin-------\n");
    delay_ms(500);

}
void exception(unsigned char *string,float number,int div)
{
#ifdef _EXCEPTION_ON

        my_putstr("\n");
        my_putstr(string);
        my_putstr("{");
        my_put_float(number,div);
        my_putstr("}\n");
#endif
}
void dis_int()
{
    TCCR0=0;
}
void ena_int()
{
    TCNT0=0;
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);

}

//void accel_cal_2()
//{
//    a_x=mpu6050_get_accel_x();
//    a_y=mpu6050_get_accel_y();
//    a_z=mpu6050_get_accel_z();
//
//     a_x/=100;
//     a_y/=100;
//     a_z/=100;
//
//
//
////
////    exception("a_x",a_x,7);
////    exception("a_y",a_y,7);
////    exception("a_z",a_z,7);
//
////
////    deg_accel_x_2= acos ((float)a_x/(float)sqrt((a_x*a_x+a_y*a_y+a_z*a_z)) )*180/M_PI; //xy plane
////    deg_accel_y_2= acos ((float)a_y/sqrt((float)(a_x*a_x+a_y*a_y+a_z*a_z)))*180/M_PI;//yz plane
////    deg_accel_z_2= acos  ((float)a_z/(float)sqrt((a_x*a_x+a_y*a_y+a_z*a_z)) )*180/M_PI;//zx plane
////    if(a_x>0&&a_z>0)
////    {
////        deg_accel_z_2=360-deg_accel_z_2 ; //xy plane
////      //  putchar('a');
////    }
////    else if(a_x<0&&a_z>0)
////    {
////    //    deg_accel_z_2=270+(90-deg_accel_x_2) ;
////    }
////    else if(a_x>0&&a_z<0)
////    {
////
////        deg_accel_z_2=270+(90-deg_accel_z_2) ;
////        deg_accel_x_2=270+(90-deg_accel_x_2) ; //xy plane
////    }
////    else if(a_x<0&&a_z<0)
////    {
////        deg_accel_x_2=360-deg_accel_x_2 ; //xy plane
////    }
//
//
//
//
//    deg_accel_z_2= atan2(a_y,a_x)*180/M_PI; //xy plane
//    deg_accel_x_2= atan2(a_y,a_z)*180/M_PI;//yz plane
//    deg_accel_y_2= atan2(a_x,a_z)*180/M_PI;//zx plane
////
//
//    deg_accel_x_2=(deg_accel_x_2>0)?deg_accel_x_2:360+deg_accel_x_2;
//    deg_accel_y_2=(deg_accel_y_2>0)?deg_accel_y_2:360+deg_accel_y_2;
//    deg_accel_z_2=(deg_accel_z_2>0)?deg_accel_z_2:360+deg_accel_z_2;
//    deg_accel_y_2=360-deg_accel_y_2;
//    deg_accel_z_2=360-deg_accel_z_2;
//
//
//    //exception("a_x/accel",t,10);   //acos (  )*180/M_PI
//
////    deg_accel_x_2=(deg_accel_x_2<0)?(360+deg_accel_x_2):deg_accel_x_2;
////    deg_accel_y_2=(deg_accel_y_2<0)?(360+deg_accel_y_2):deg_accel_y_2;
////    deg_accel_z_2=(deg_accel_z_2<0)?(360+deg_accel_z_2):deg_accel_z_2;
//
////    degree_fuse_x=.98*(degree_fuse_x+degree_x)+.02*(a_x);
////    degree_fuse_y=.98*(degree_fuse_y+degree_y)+.02*(a_y);
////    degree_fuse_z=.98*(degree_fuse_z+degree_z)+.02*(a_z);
////
////
////    while((degree_fuse_x>360)||(degree_y>360)||(degree_z>360))
////    {
////        if(degree_fuse_x>360)
////            degree_fuse_x=degree_fuse_x-360;
////        if(degree_y>360)
////            degree_y=degree_y-360;
////        if(degree_z>360)
////            degree_z=degree_z-360;
////        //exception("upper",6,0);
////    }
////    while((degree_fuse_x<0)||(degree_z<0)||(degree_y<0))
////    {
////        if(degree_fuse_x<0)
////            degree_fuse_x=360+degree_fuse_x;
////        if(degree_y<0)
////            degree_y=360+degree_y;
////        if(degree_z<0)
////            degree_z=360+degree_z;
////        //exception("leve",5,0);
////    }
//}
//float mabs(float numb)
//{
//    return (numb>0)?(numb):(numb*-1);
//}
//void gyro_cal(int OFF_X,int OFF_Y,int OFF_Z,)
//{
//
//        int t_x,t_y,t_z;
//        t_x=g_x;
//        t_y=g_y;
//        t_z=g_z;
//
//        g_x=(float)mpu6050_get_gyro_x()-OFF_X;
//        g_y=(float)mpu6050_get_gyro_y()-OFF_Y;
//        g_z=(float)mpu6050_get_gyro_z()-OFF_Z;
//
//        g_x=(int)g_x/GRYRO_SCALE;
//        g_y=(int)g_y/GRYRO_SCALE;
//        g_z=(int)g_z/GRYRO_SCALE;
//
//        g_x=t_x*.1+g_x*.9;
//        g_y=t_y*.1+g_y*.9;
//        g_z=t_z*.1+g_z*.9;
//
//
//        dis_int();
//
//        degree_x+=(float)g_x*(float)(counter*256+(TCNT0))*(float)TIME;//0.000000977022=;
//        degree_y+=(float)g_y*(float)(counter*256+(TCNT0))*(float)TIME;//0.000000977022=TIME/GRYRO_SCALE;
//        degree_z+=(float)g_z*(float)(counter*256+(TCNT0))*(float)TIME;//0.000000977022=TIME/GRYRO_SCALE;
//
//        //exception("gx_*",(float)g_x*(float)(counter*256+(TCNT0))*(float)TIME,5);
//        //exception("gy_*",(float)g_y*(float)(counter*256+(TCNT0))*(float)TIME,5);
//        //exception("gz_*",(float)g_z*(float)(counter*256+(TCNT0))*(float)TIME,5);
//        // exception("TCNT0",TCNT0,0);
//        counter=0;
//        ena_int();
//
//        while((degree_x>360)||(degree_y>360)||(degree_z>360))
//        {
//            if(degree_x>360)
//                degree_x=degree_x-360;
//            if(degree_y>360)
//                degree_y=degree_y-360;
//            if(degree_z>360)
//                degree_z=degree_z-360;
//            //exception("upper",6,0);
//        }
//        while((degree_x<0)||(degree_z<0)||(degree_y<0))
//        {
//            if(degree_x<0)
//                degree_x=360+degree_x;
//            if(degree_y<0)
//                degree_y=360+degree_y;
//            if(degree_z<0)
//                degree_z=360+degree_z;
//            //exception("leve",5,0);
//        }
//}
//void accel_cal(int OFF_X,int OFF_Y,int OFF_Z)
//{
//
//
//    int t_x,t_y,t_z;
//    int zero=180;
//    int p_x,p_y,p_z;
//    t_x=g_x;
//    t_y=g_y;
//    t_z=g_z;
//
//    p_x=a_x;
//    p_y=a_y;
//    p_z=a_z;
//
//    g_x=(float)mpu6050_get_gyro_x()-OFF_X;
//    g_y=(float)mpu6050_get_gyro_y()-OFF_Y;
//    g_z=(float)mpu6050_get_gyro_z()-OFF_Z;
//
//    a_x=mpu6050_get_accel_x();
//    a_y=mpu6050_get_accel_y();
//    a_z=mpu6050_get_accel_z();
//
//    g_x=(int)g_x/GRYRO_SCALE;
//    g_y=(int)g_y/GRYRO_SCALE;
//    g_z=(int)g_z/GRYRO_SCALE;
//
//    g_x=t_x*.2+g_x*.8;
//    g_y=t_y*.2+g_y*.8;
//    g_z=t_z*.2+g_z*.8;
//
//    a_x/=100;
//    a_y/=100;//because of avr variable size i destroyed my precision
//    a_z/=100;
//
//    a_x=a_x*.4+p_x*.6;
//    a_y=a_y*.4+p_y*.6;
//    a_z=a_z*.4+p_z*.6;
//
//    deg_accel_z_2= atan2(a_y,a_x)*180/M_PI; //xy plane
//    deg_accel_x_2= atan2(a_y,a_z)*180/M_PI;//yz plane
//    deg_accel_y_2= atan2(a_x,a_z)*180/M_PI;//zx plane
//
//    deg_accel_x_2=(deg_accel_x_2>0)?deg_accel_x_2:360+deg_accel_x_2;
//    deg_accel_y_2=(deg_accel_y_2>0)?deg_accel_y_2:360+deg_accel_y_2;
//    deg_accel_z_2=(deg_accel_z_2>0)?deg_accel_z_2:360+deg_accel_z_2;
//    deg_accel_y_2=360-deg_accel_y_2;
//
//    //deg_accel_z_2=360-deg_accel_z_2;
//
//
//    dis_int();
//
//    degree_x+=((float)g_x*(float)(counter*256+(TCNT0))*(float)TIME);//0.000000977022=;
//    degree_y+=((float)g_y*(float)(counter*256+(TCNT0))*(float)TIME);//0.000000977022=TIME/GRYRO_SCALE;
//    degree_z+=((float)g_z*(float)(counter*256+(TCNT0))*(float)TIME);//0.000000977022=TIME/GRYRO_SCALE;
//
//    counter=0;
//    ena_int();
//    while((degree_x>360)||(degree_y>360)||(degree_z>360))
//    {
//        if(degree_x>360)
//            degree_x=degree_x-360;
//        if(degree_y>360)
//            degree_y=degree_y-360;
//        if(degree_z>360)
//            degree_z=degree_z-360;
//    }
//    while((degree_x<0)||(degree_z<0)||(degree_y<0))
//    {
//        if(degree_x<0)
//            degree_x=360+degree_x;
//        if(degree_y<0)
//            degree_y=360+degree_y;
//        if(degree_z<0)
//            degree_z=360+degree_z;
//    }
//
//    degree_x_z=0;
//    if(zero<=degree_x)
//    {
//        degree_x_z=degree_x-zero;
//    }
//    else
//    {
//        degree_x_z=360-(zero-degree_x);
//    }
//    if(zero<=degree_y)
//    {
//        degree_y_z=degree_y-zero;
//    }
//    else
//    {
//        degree_y_z=360-(zero-degree_y);
//    }if(zero<=degree_z)
//    {
//        degree_z_z=degree_z-zero;
//    }
//    else
//    {
//        degree_z_z=360-(zero-degree_z);
//    }
//    ////
//    if(zero<=deg_accel_x_2)
//    {
//        deg_accel_x_z=deg_accel_x_2-zero;
//    }
//    else
//    {
//        deg_accel_x_z=360-(zero-deg_accel_x_2);
//    }
//    if(zero<=deg_accel_y_2)
//    {
//        deg_accel_y_z=deg_accel_y_2-zero;
//    }
//    else
//    {
//        deg_accel_y_z=360-(zero-deg_accel_y_2);
//    }
//    if(zero<=deg_accel_z_2)
//    {
//        deg_accel_z_z=deg_accel_z_2-zero;
//    }
//    else
//    {
//        deg_accel_z_z=360-(zero-deg_accel_z_2);
//    }
//
//    fused_x=(degree_x_z)*.3+deg_accel_x_z*.1+fused_x*.6;
//    fused_y=fused_y*.3+(degree_y_z)*.3+deg_accel_y_z*.4;
//    fused_z=fused_z*.3+(degree_z_z)*.3+deg_accel_z_z*.4;
//
//
//
//}

