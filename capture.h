#ifndef _CAPTURE_H
#define _CAPTURE_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern sem_t sem_camera;

/**
 * @brief Callback Function for Camera Service.
 * @return void.
 */
void *camera_service(void *threadp); 				

#endif
