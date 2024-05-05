
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#include "motor.h"

// Define GPIO pins for Trigger and Echo pins
#define TRIG 15
#define ECHO 16

sem_t sem_ultrasonic;
extern bool is_forward;
extern bool is_reverse;
extern bool abortS3;

void ultasonic_sensor_setup() {
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Ensure the trigger pin is low
    digitalWrite(TRIG, LOW);
    delay(30);
}

void *ultrasonic_sensor_service(void *threadp) {
    printf("Distance Measurement In Progress\n");

    while (!abortS3) {
		sem_wait(&sem_ultrasonic);
		//printf("is_r2: %d\r\n", is_forward);
		if(is_forward == true)
		{
			struct timeval start, end;
			long travelTime, distance;

			// Triggering the sensor for 10 microseconds
			digitalWrite(TRIG, HIGH);
			delayMicroseconds(10);
			digitalWrite(TRIG, LOW);

			// Wait for the echo start
			while (digitalRead(ECHO) == LOW);

			// Record time of signal return
			gettimeofday(&start, NULL);
			while (digitalRead(ECHO) == HIGH);
			gettimeofday(&end, NULL);

			// Calculate the distance
			travelTime = (end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec - start.tv_usec;
			distance = travelTime / 58;
			printf("Distance: %d cm\n", distance);
			if(distance < 10)
			{
				is_obstacle_detected = true;
			}
			else
			{
				is_obstacle_detected = false;
			}
		}
    }
    
    printf("Sensor stopped\n");

    pthread_exit(NULL);
}
