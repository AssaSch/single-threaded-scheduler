#ifndef __TIME_TOOLS_H__
#define __TIME_TOOLS_H__

/** 
 * @brief API of time tools. 
 * 
 * @created 07.10.17
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <time.h> /* clockid_t */

typedef struct timespec timespec;

/**  
 * @brief compare two timespecs . 
 * @param[in] _first - timespec struct.
 * @param[in] _second - timespec struct.
 * @return 0 - _first < _second / other value - _first >= _second.         
 */
int TimeCompare(timespec _first, timespec _second);

/**  
 * @brief get cuurent time of system.  
 * @param[in] _clk_id - id of system clock.
 * @return timespec - current time of system.  
 */
timespec TimeGetCurrent(clockid_t _clk_id);

/**  
 * @brief substract two timespec times.   
 * @param[in] _first - timespec struct.
 * @param[in] _second - timespec struct.
 * @return substract answer in milli-seconds -  positive if _first > _second / negative if opposite.
 */
long int TimeSub(timespec _first, timespec _second);

/**  
 * @brief adds time in milli-seconds to timespec struct.  
 * @param[in] _time - timespec struct.
 * @param[in] _period_ms - period of recurrence in milli seconds.
 * @return timespec - time after added period.  
 */
timespec TimeAdd(timespec _time, size_t _period_ms);

/**  
 * @brief sleep until time provided.  
 * @param[in] _clk_id - id of system clock.
 * @param[in] _time - timespec struct.
 */
void TimeSleep(clockid_t _clk_id, timespec _time);

#endif /* __TIME_TOOLS_H__ */
