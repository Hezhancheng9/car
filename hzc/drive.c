#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <arpa/inet.h>

#include <wiringPi.h>
#include <softPwm.h>

#define BUFSIZE 512

int PWMA = 1;
int AIN2 = 2;
int AIN1 = 3;

int PWMB = 4;
int BIN2 = 5;
int BIN1 = 6;

void  t_up(unsigned int speed,unsigned int t_time)
{
        digitalWrite(AIN2,0);
        digitalWrite(AIN1,1);
        softPwmWrite(PWMA,speed);
        digitalWrite(BIN2,0);
        digitalWrite(BIN1,1);
        softPwmWrite(PWMB,speed);
        delay(t_time);
}

void t_down(unsigned int speed,unsigned int t_time)
{
     digitalWrite(AIN2,1);
     digitalWrite(AIN1,0);
     softPwmWrite(PWMA,speed);

     digitalWrite(BIN2,1);
     digitalWrite(BIN1,0);
     softPwmWrite(PWMB,speed);
     delay(t_time);
}

void t_stop(unsigned int t_time)
{
     digitalWrite(AIN2,0);
     digitalWrite(AIN1,0);
     softPwmWrite(PWMA,0);

     digitalWrite(BIN2,0);
     digitalWrite(BIN1,0);
     softPwmWrite(PWMB,0);
     delay(t_time);
}

int main(int argc, char *argv[])
{
	 wiringPiSetup();
       /*WiringPi GPIO*/
       pinMode (PWMA, OUTPUT);   
       pinMode (AIN2, OUTPUT);   
       pinMode (AIN1, OUTPUT);   
       pinMode (PWMB, OUTPUT);   
       pinMode (BIN2, OUTPUT);   
       pinMode (BIN1, OUTPUT);   
       /*PWM output*/
       softPwmCreate(PWMA,0,100);
       softPwmCreate(PWMB,0,100);

    t_up(50,5000);
    t_down(50,5000);
    t_stop(5000);
    while(1){};
}
