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
#include <sys/socket.h>

#define BUFSIZE 512
#define LEFT  23
#define RIGHT 25

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

void t_left(unsigned int speed,unsigned int t_time)
{
        digitalWrite(AIN2,1);
        digitalWrite(AIN1,0);
        softPwmWrite(PWMA,speed);
        digitalWrite(BIN2,0);
        digitalWrite(BIN1,1);
        softPwmWrite(PWMB,speed);
        delay(t_time);
}

void t_right(unsigned int speed,unsigned int t_time)
{
        digitalWrite(AIN2,0);
        digitalWrite(AIN1,1);
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
       /*RPI*/
       wiringPiSetup();

       pinMode (PWMA, OUTPUT);
       pinMode (AIN2, OUTPUT);
       pinMode (AIN1, OUTPUT);
       pinMode (PWMB, OUTPUT);
       pinMode (BIN2, OUTPUT);
       pinMode (BIN1, OUTPUT);
       /*PWM output*/
       softPwmCreate(PWMA,0,100);
       softPwmCreate(PWMB,0,100);


        int SR,SL;
   while(1){
  SR = digitalRead(RIGHT);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
  SL = digitalRead(LEFT);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭


        if (SL == LOW&&SR==LOW){
           printf("GO\n");
           t_up(50,100);
       }
  else if (SL == HIGH&&SR == LOW){
         printf("RIGHT\n");
          t_right(50,100);

      }
  else if (SR == HIGH&&SL == LOW) {
          printf("LEFT\n");
          t_left(50,100);
      }
  else {// 都是白色, 停止
         printf("STOP\n");
         t_stop(100);
     }
        }

}

