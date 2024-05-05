#include <stdio.h>
#include <wiringPi.h>

extern sem_t sem_ultrasonic;

void setup_ultasonic_sensor();

void *ultrasonic_sensor_service(void *threadp);
