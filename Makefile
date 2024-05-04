INCLUDE_DIRS = -I/usr/include/opencv4 -I/home/shruti/Desktop/pi-parking/
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt -lwiringPi

HFILES= 
CFILES= main.cpp motor.cpp capture.cpp ultrasonic_sensor.cpp

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.cpp=.o}

all:	main

clean:
	-rm -f *.o *.d
	-rm -f main

main: main.o capture.o motor.o ultrasonic_sensor.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ main.o motor.o capture.o ultrasonic_sensor.o `pkg-config --libs opencv4` $(LIBS)

depend:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c $<
