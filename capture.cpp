/*
 *
 *  Example by Sam Siewert 
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

#include <semaphore.h>

#include "capture.h"
#include "motor.h"

using namespace cv;
using namespace std;

extern bool is_forward;
extern bool is_reverse;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

sem_t sem_camera;

void *camera_service(void *threadp)
{
    printf("Camera test starts\n");
    printf("is_r: %d\r\n", is_reverse);
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

    while (true)
    {
        sem_wait(&sem_camera);

        if (is_reverse)
        {
            cam0.read(frame);
            imshow("video_display", frame);
        }
        else
        {
            imshow("video_display", blackframe);
        }

        winInput = waitKey(10);
        if (winInput == ESCAPE_KEY)
        {
            break;
        }
        else if (winInput == 'n')
        {
            cout << "input " << winInput << " ignored" << endl;
        }
    }

    destroyWindow("video_display");
}

