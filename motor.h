#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

extern bool is_obstacle_detected;
extern sem_t sem_motor;

void setup_gpio();

void control_motor(int motor, int speed, int direction);

void *motor_service(void *threadp);


