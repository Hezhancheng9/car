#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>

#define Trig 28
#define Echo 29

#define SPEED 20
#define DELAY 80

#define PWMA 1
#define AIN2 2
#define AIN1 3

#define PWMB 4
#define BIN2 5
#define BIN1 6

void ultraInit(void)
{
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);
}

float disMeasure(void)
{
    struct timeval tv1;
    struct timeval tv2;
    long start, stop;
    float dis;
        printf("**************************************************************\n");
    digitalWrite(Trig, LOW);
        printf("**************************************************************\n");
    delayMicroseconds(2);
        printf("**************************************************************\n");
    digitalWrite(Trig, HIGH);
        printf("**************************************************************\n");
    delayMicroseconds(10); // 发出超声波脉冲
        printf("**************************************************************\n");
    digitalWrite(Trig, LOW);
        printf("**************************************************************\n");
    while (!(digitalRead(Echo) == 1));
        printf("**************************************************************\n");
    gettimeofday(&tv1, NULL); 
        printf("**************************************************************\n");
    while (!(digitalRead(Echo) == 0));
    printf("**************************************************************\n");
    gettimeofday(&tv2, NULL);          
    start = tv1.tv_sec * 1000000 + tv1.tv_usec; // 微秒级的时间
    stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

    dis = (float)(stop - start) / 1000000 * 34000 / 2; // 求出距离
    return dis;
}

void t_up(unsigned int speed, unsigned int t_time)
{
    digitalWrite(AIN2, 0);
    digitalWrite(AIN1, 1);
    softPwmWrite(PWMA, speed);
    digitalWrite(BIN2, 0);
    digitalWrite(BIN1, 1);
    softPwmWrite(PWMB, speed);
    delay(t_time);
}
void t_down(unsigned int speed, unsigned int t_time)
{
    digitalWrite(AIN2, 1);
    digitalWrite(AIN1, 0);
    softPwmWrite(PWMA, speed);

    digitalWrite(BIN2, 1);
    digitalWrite(BIN1, 0);
    softPwmWrite(PWMB, speed);
    delay(t_time);
}

void t_left(unsigned int speed, unsigned int t_time)
{
    digitalWrite(AIN2, 1);
    digitalWrite(AIN1, 0);
    softPwmWrite(PWMA, speed);
    digitalWrite(BIN2, 0);
    digitalWrite(BIN1, 1);
    softPwmWrite(PWMB, 50);
    delay(t_time);
}

void t_right(unsigned int speed, unsigned int t_time)
{
    digitalWrite(AIN2, 0);
    digitalWrite(AIN1, 1);
    softPwmWrite(PWMA, 50);
    digitalWrite(BIN2, 1);
    digitalWrite(BIN1, 0);
    softPwmWrite(PWMB, speed);
    delay(t_time);
}

void t_stop(unsigned int t_time)
{
    digitalWrite(AIN2, 0);
    digitalWrite(AIN1, 0);
    softPwmWrite(PWMA, 0);

    digitalWrite(BIN2, 0);
    digitalWrite(BIN1, 0);
    softPwmWrite(PWMB, 0);
    delay(t_time);
}

int main(int argc, char *argv[])
{
    float dis;
    wiringPiSetup();

    pinMode(PWMA, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    /*PWM output*/
    softPwmCreate(PWMA, 0, 100);
    softPwmCreate(PWMB, 0, 100);

    ultraInit();

    while (1)
    {
        dis = disMeasure();
        printf("distance = %0.2f cm\n", dis); // 输出当前超声波测得的距离
        t_up(SPEED,1);
        if(dis<5.0)
        {
            t_down(SPEED,1000);
            t_left(SPEED,1);
        }
    }
    return 0;
}