#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>

extern bool is_obstacle_detected;
extern sem_t sem_motor;

void setupGpio();

void controlMotor(int motor, int speed, int direction);

void *motor_service(void *threadp);


