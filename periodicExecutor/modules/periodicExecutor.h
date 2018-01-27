#ifndef __PERIODIC_EXECUTOR_H__
#define __PERIODIC_EXECUTOR_H__

/** 
 * @brief API of Periodic Executor. 
 * The Periodic executor will execute tasks added to it. Each task can be executed more than one
 * time. The period of the recurrence is set per task and each task can specify if it needs more
 * execution cycles or it has finished.
 *
 * @created 07.10.17 
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <time.h> /* clock_id */

typedef struct PeriodicExecutor PeriodicExecutor;

typedef int	(*Func)(void *_context); /* user provided function */

/**  
 * @brief Create a periodic executor that will measure time using given clock.
 * Clockid_t specify the id of the system clock to be used. CLOCK_REALTIME,
 * CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC and similar provided it’s
 * supported by the underlying system.  
 * @param[in] _clk_id - id of system clock.
 * @return PeriodicExecutor* - on success / NULL on fail 
 * @warning do not create new Executor while another is in work! (undefined behavior)
 */
PeriodicExecutor* PeriodicExecutorCreate(clockid_t _clk_id);

/**  
 * @brief Destroy previously created executor. Cleaning all allocated memory and resources.  
 * @param[in] _executor - executor to be deallocated.
 */
void PeriodicExecutorDestroy(PeriodicExecutor* _executor);

/**  
 * @brief Adding task to the executor.  
 * @param[in] _executor - executor pointer.
 * @param[in] _func - user provided function pointer. 
 * @params[in] _context - paramaters for function.
 * @params[in] _period_ms - period of recurrence in milli seconds.
 * @return 1 on seccess / 0 if failed.
 */
int PeriodicExecutorAdd(PeriodicExecutor* _executor, Func _func, void* _context, size_t _period_ms);

/**  
 * @brief Start running the tasks previously added to the executor or resume a previously paused Executor. 
 * This function will return in one of two cases:
 * 1. The tasks were executed to completion.
 * 2. The executor was paused
 * @param[in] _executor - executor pointer.
 * @return number of execution of functions / 0 if no execution or failed.         
 */
size_t PeriodicExecutorRun(PeriodicExecutor* _executor);

/**  
 * @brief Pause the execution of the executor. If a task is currently being executed then pause after
 * it has been completed current cycle.
 * @param[in] _executor - executor pointer.
 * @return number of execution of functions / 0 if no execution or failed..          
 */
size_t PeriodicExecutorPause(PeriodicExecutor* _executor);

#endif /* __PERIODIC_EXECUTOR_H__ */
