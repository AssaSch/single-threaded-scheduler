#include "timeTools.h"

#include <unistd.h> /* usleep */ 

#define CONVERT_SEC_2_MILLI 1000
#define CONVERT_NANOSEC_2_MILLI 1/1000000
#define CONVERT_MILLI_2_NANOSEC 1000000
#define CONVERT_MILLI_2_SEC 1/1000
#define CONVERT_MILLI_2_MICRO 1000
#define MODULU_NANO 1000000
#define MODULU_MILLI 1000
#define ROUND(N) (N * CONVERT_NANOSEC_2_MILLI + 1)
#define NEED_2_ROUND(N) ((N % MODULU_NANO) > (MODULU_NANO / 2))

/** 
 * @brief functions of time tools. 
 * 
 * @created 07.10.17
 * @author Asa Schneider
 */ 

/*****************************************************************************/
                        /* TimeCompare */
/*****************************************************************************/
int TimeCompare(timespec _first, timespec _second)
{	
	long int secTime;
	long int nanoTime;
	
	secTime = _first.tv_sec - _second.tv_sec;
	nanoTime = _first.tv_nsec - _second.tv_nsec;
	
	if (secTime < 0 || (!secTime && nanoTime < 0))
	{
		return 0;
	}
	
	if (secTime > 0 || (!secTime && nanoTime > 0))
	{
		return 1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* TimeGetCurrent */
/*****************************************************************************/
timespec TimeGetCurrent(clockid_t _clk_id)
{
	timespec tm;
	
	clock_gettime(_clk_id, &tm);
	
	return tm;
}

/*****************************************************************************/
                        /* TimeSub */
/*****************************************************************************/
long int TimeSub(timespec _first, timespec _second)
{	
	long int secTime;
	long int nanoTime;
	
	secTime = (_first.tv_sec - _second.tv_sec) * CONVERT_SEC_2_MILLI;
	nanoTime = _first.tv_nsec - _second.tv_nsec;
	
	/* round nanoTime if necessary */
	if (NEED_2_ROUND(nanoTime))
	{
		nanoTime = ROUND(nanoTime);
	}
	else
	{
		nanoTime *= CONVERT_NANOSEC_2_MILLI;
	}
	
	return secTime + nanoTime;
}

/*****************************************************************************/
                        /* TimeAdd */
/*****************************************************************************/
timespec TimeAdd(timespec _time, size_t _period_ms)
{
	_time.tv_sec += (long int)_period_ms * CONVERT_MILLI_2_SEC;
	_time.tv_nsec += (long int)(_period_ms % MODULU_MILLI) * CONVERT_MILLI_2_NANOSEC;
	
	return _time; 
}

/*****************************************************************************/
                        /* TimeSleep */
/*****************************************************************************/
void TimeSleep(clockid_t _clk_id, timespec _time)
{
	timespec tm;
	int timeToSleep;
	
	tm = TimeGetCurrent(_clk_id);
	timeToSleep = (int)TimeSub(_time, tm);
	
	if (timeToSleep > 0)
	{
		usleep((unsigned int)timeToSleep * CONVERT_MILLI_2_MICRO);
	}
	
	return;
}




















