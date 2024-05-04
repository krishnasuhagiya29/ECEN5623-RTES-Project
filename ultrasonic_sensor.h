#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/time.h>

extern sem_t sem_ultrasonic;

void ultasonic_sensor_setup();

void *ultrasonic_sensor_service(void *threadp);
