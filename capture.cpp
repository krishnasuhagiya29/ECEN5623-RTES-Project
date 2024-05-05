/*
 *
 *  Example by Sam Siewert
 *  Modified by Krishna Suhagiya and Unmesh Phaterpekar
 *
 *  Created for OpenCV 4.x for Jetson Nano 2g, based upon
 *  https://docs.opencv.org/4.1.1
 *
 *  Tested with JetPack 4.6 which installs OpenCV 4.1.1
 *  (https://developer.nvidia.com/embedded/jetpack)
 *
 *  Based upon earlier simpler-capture examples created
 *  for OpenCV 2.x and 3.x (C and C++ mixed API) which show
 *  how to use OpenCV instead of lower level V4L2 API for the
 *  Linux UVC driver.
 *
 *  Verify your hardware and OS configuration with:
 *  1) lsusb
 *  2) ls -l /dev/video*
 *  3) dmesg | grep UVC
 *
 *  Note that OpenCV 4.x only supports the C++ API
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <syslog.h>
#include <semaphore.h>
#include <pthread.h>

#include "capture.h"
#include "motor.h"
#include "time.h"

using namespace cv;
using namespace std;

extern bool is_forward;
extern bool is_reverse;
extern bool abortS1;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

sem_t sem_camera;

void *camera_service(void *threadp)
{
    struct timespec start = {0,0};
    struct timespec stop = {0,0};
    static struct timespec wcet = {0,0};
    struct timespec time_taken = {0,0};
    printf("Camera test starts\n");
    VideoCapture cam0(0);
    namedWindow("video_display");
    char winInput;

    if (!cam0.isOpened())
    {
        exit(SYSTEM_ERROR);
    }

    cam0.set(CAP_PROP_FRAME_WIDTH, 640);
    cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

    Mat frame;
    Mat blackframe = Mat::zeros(Size(640, 480), CV_8UC3);

    while (!abortS1)
    {
        sem_wait(&sem_camera);
        if (is_reverse)
        {
            clock_gettime(CLOCK_REALTIME, &start);
            cam0.read(frame);
            imshow("video_display", frame);
            clock_gettime( CLOCK_REALTIME, &stop);
            delta_t(&stop, &start, &time_taken);
            if(check_wcet(&time_taken, &wcet))
            {
                syslog(LOG_INFO, "camera wcet: %lu sec, %lu msec (%lu microsec), ((%lu nanosec))\n\n", wcet.tv_sec, (wcet.tv_nsec / NSEC_PER_MSEC), (wcet.tv_nsec / NSEC_PER_MICROSEC),wcet.tv_nsec);
            }
        }
        else
        {
            imshow("video_display", blackframe);
        }

    }

    destroyWindow("video_display");
    printf("Camera stopped\n");
    pthread_exit(NULL);
}

