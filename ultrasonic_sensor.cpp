#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#include "motor.h"
#include "time.h"

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
    struct timespec start = {0,0};
    struct timespec stop = {0,0};
    static struct timespec wcet = {0,0};
    struct timespec time_taken = {0,0};
    printf("Distance Measurement In Progress\n");

    while (!abortS3) {
		sem_wait(&sem_ultrasonic);
		if(is_forward == true)
		{
			clock_gettime(CLOCK_REALTIME, &start);
			struct timeval detection_start, detection_end;
			long travel_time, distance;

			// Triggering the sensor for 10 microseconds
			digitalWrite(TRIG, HIGH);
			delayMicroseconds(10);
			digitalWrite(TRIG, LOW);

			// Wait for the echo start
			while (digitalRead(ECHO) == LOW);

			// Record time of signal return
			gettimeofday(&detection_start, NULL);
			while (digitalRead(ECHO) == HIGH);
			gettimeofday(&detection_end, NULL);

			// Calculate the distance
			travel_time = (detection_end.tv_sec - detection_start.tv_sec) * 1000000L + detection_end.tv_usec - detection_start.tv_usec;
			distance = travel_time / 58;
			syslog(LOG_INFO, "Distance: %d cm\n", distance);
			if(distance < 10)
			{
				is_obstacle_detected = true;
			}
			else
			{
				is_obstacle_detected = false;
			}
		        clock_gettime( CLOCK_REALTIME, &stop);
		        delta_t(&stop, &start, &time_taken);
		        if(check_wcet(&time_taken, &wcet))
		        {
			    syslog(LOG_INFO, "sensor wcet: %lu sec, %lu msec (%lu microsec), ((%lu nanosec))\n\n", wcet.tv_sec, (wcet.tv_nsec / NSEC_PER_MSEC), (wcet.tv_nsec / NSEC_PER_MICROSEC),wcet.tv_nsec);
		        }
		}
    }
    
    syslog(LOG_INFO, "Sensor stopped\n");

    pthread_exit(NULL);
}
