#include "time.h"

int delta_t(struct timespec *stop, struct timespec *start, struct timespec *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_nsec=stop->tv_nsec - start->tv_nsec;

  if(dt_sec >= 0)
  {
    if(dt_nsec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_nsec=dt_nsec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
    }
  }
  else
  {
    if(dt_nsec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_nsec=dt_nsec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_nsec=NSEC_PER_SEC+dt_nsec;
    }
  }

  return(1);
}

bool check_wcet(struct timespec *time_taken, struct timespec *wcet)
{
	if(((double)((double)time_taken->tv_sec * MSEC_PER_SEC) + (double)((double)time_taken->tv_nsec / NSEC_PER_MSEC)) > ((double)((double)wcet->tv_sec * MSEC_PER_SEC) + (double)((double)wcet->tv_nsec / NSEC_PER_MSEC)))
	{
		wcet->tv_sec = time_taken->tv_sec;
		wcet->tv_nsec = time_taken->tv_nsec;
		return true;
	}
	return false;
}
