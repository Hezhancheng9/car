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

#define F_LEFT 23
#define F_RIGHT 25

#define B_LEFT 27
#define B_RIGHT 26

#define SPEED 20
#define DELAY 80

#define PWMA 1
#define AIN2 2
#define AIN1 3

#define PWMB 4
#define BIN2 5
#define BIN1 6

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
    /*RPI*/
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

    int F_SR;
    int F_SL;
    int B_SR;
    int B_SL;

    int flag = 1;
    while (1)
    {
        F_SR = digitalRead(F_RIGHT); // 有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
        F_SL = digitalRead(F_LEFT);  // 有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭

        B_SR = digitalRead(B_RIGHT); // 有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
        B_SL = digitalRead(B_LEFT);  // 有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
        if (B_SR || B_SL == LOW)
        {
            flag = 0;
        }
        if (B_SR == HIGH && B_SL == HIGH)
        {
            flag = 1;
        }

        if (flag == 1)
        {
            if (F_SL == LOW && F_SR == LOW)
            {
                printf("GO\n");
                t_up(SPEED, 1);
            }
            else if (F_SL == HIGH && F_SR == LOW)
            {
                printf("F_RIGHT\n");
                t_right(SPEED, 1);
            }
            else if (F_SR == HIGH && F_SL == LOW)
            {
                printf("F_LEFT\n");
                t_left(SPEED, 1);
            }
            else
            { // 都是白色, 停止
                printf("STOP\n");
                t_stop(100);
            }
        }
        if (flag == 0)
        {
            if (B_SL == LOW && B_SR == LOW)
            {
                printf("B_BACK"); // 前面有物体时小车后退ms 再转弯
                t_down(SPEED, 500);
                t_left(SPEED, 500); // 左转400ms
            }
            else if (B_SL == HIGH && B_SR == LOW)
            {
                printf("B_RIGHT");
                t_right(SPEED, 500);
            }
            else if (B_SR == HIGH && B_SL == LOW)
            {
                printf("B_LEFT");
                t_left(SPEED, 500);
            }
            else
            { // 前面没有物体 前进
                printf("GO");
                t_up(SPEED, 500);
            }
        }
    }
    return 0;
}
