#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#define MSEC_PER_SEC (1000)
#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_MICROSEC (1000)

int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t);

bool check_wcet(struct timespec *time_taken, struct timespec *wcet);
