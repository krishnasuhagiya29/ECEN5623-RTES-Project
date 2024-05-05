#include <stdio.h>
#include <wiringPi.h>
#include <semaphore.h>
#include <pthread.h>

#include "motor.h"

sem_t sem_motor;
bool is_forward = true;
bool is_reverse = false;
extern bool abortS2;

// GPIO pin definitions
#define MOTOR_PWM_A 1  // PWM for Motor A (GPIO 18)
#define MOTOR_IN1_A 4  // Direction IN1 for Motor A (GPIO 23)
#define MOTOR_IN2_A 5  // Direction IN2 for Motor A (GPIO 24)
#define MOTOR_PWM_B 23 // PWM for Motor B (GPIO 13)
#define MOTOR_IN1_B 3  // Direction IN1 for Motor B (GPIO 22, WiringPi pin 3)
#define MOTOR_IN2_B 2  // Direction IN2 for Motor B (GPIO 27, WiringPi pin 2)
#define STBY_PIN 6     // Standby pin (GPIO 25)
#define BUTTON_PIN 7   // Button pin (GPIO 4, WiringPi pin 7)

// Initialize GPIO pins
void setupGpio() {
    wiringPiSetup();
    pinMode(BUTTON_PIN, INPUT);  // Set button pin as input
    pullUpDnControl(BUTTON_PIN, PUD_UP);  // Enable pull-up resistor
    pinMode(MOTOR_PWM_A, PWM_OUTPUT);
    pinMode(MOTOR_IN1_A, OUTPUT);
    pinMode(MOTOR_IN2_A, OUTPUT);
    pinMode(MOTOR_PWM_B, PWM_OUTPUT);
    pinMode(MOTOR_IN1_B, OUTPUT);
    pinMode(MOTOR_IN2_B, OUTPUT);
    pinMode(STBY_PIN, OUTPUT);
    digitalWrite(STBY_PIN, HIGH);  // Take motor driver out of standby mode
}

// Control motor speed and direction
void controlMotor(int motor, int speed, int direction) {
    if (motor == 1) {  // Motor A
        pwmWrite(MOTOR_PWM_A, speed);
        digitalWrite(MOTOR_IN1_A, direction ? HIGH : LOW);
        digitalWrite(MOTOR_IN2_A, direction ? LOW : HIGH);
    } else if (motor == 2) {  // Motor B
        pwmWrite(MOTOR_PWM_B, speed);
        digitalWrite(MOTOR_IN1_B, direction ? HIGH : LOW);
        digitalWrite(MOTOR_IN2_B, direction ? LOW : HIGH);
    }
}

void *motor_service(void *threadp)
{
	int button_state = 0;
	int* ptr_arg = (int*)threadp;
    printf("Motor test starts\n");
    printf("threadp: %d\r\n", *ptr_arg);
    
	/*if(*ptr_arg == 0)
	{
		is_forward = true;
		is_reverse = false;
	}
	else if (*ptr_arg == 1) 
	{
		is_forward = false;
		is_reverse = true;
	}*/
		
    while(!abortS2)
    {
        sem_wait(&sem_motor);
        button_state = digitalRead(BUTTON_PIN);  // Read button state
        printf("b_s: %d\r\n", button_state);
        if (button_state == 1) {  // Button is pressed
			is_forward = !is_forward;  // Toggle forward state
			is_reverse = !is_reverse;  // Toggle reverse state
		}
		if(is_forward | is_reverse)
		{
			if(is_forward)
			{
				printf("here\r\n");
				// Test Motor A
				controlMotor(1, 512, 1);  // Half speed forward
				// Test Motor B
				controlMotor(2, 512, 1);  // Half speed forward
			}
			else if(is_reverse)
			{
				printf("here1\r\n");
				controlMotor(1, 512, 0);  // Half speed backward
				controlMotor(2, 512, 0);  // Half speed backward
			}
		}
		
		if(is_obstacle_detected)
		{
			printf("here2\r\n");
			controlMotor(1, 0, 0);    // Stop Motor A
			controlMotor(2, 0, 0);    // Stop Motor B
		}
			
	}
	
        
    delay(500);

    controlMotor(1, 0, 0);    // Stop Motor A

    controlMotor(2, 0, 0);    // Stop Motor B

    printf("Motor stopped\n");
    
    pthread_exit(NULL);
}
