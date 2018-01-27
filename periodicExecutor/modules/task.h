#ifndef __TASK_H__
#define __TASK_H__

/** 
 * @brief API of task ADT. 
 * 
 * @created 07.10.17
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */
#include <time.h> /* clockid_t */
#include "periodicExecutor.h" /* Func type */

typedef struct Task Task;


/**  
 * @brief Dynamically create a new task.  
 * @param[in] _func - function to insert to task.
 * @param[in] _context - paramaters for function.
 * @param[in] _period_ms - period of recurrence in milli seconds.
 * @return Task* - on success / NULL on fail 
 */
Task* TaskCreate(Func _func, void* _context, size_t _period_ms, clockid_t _clk_id);

/**  
 * @brief Dynamically deallocate a previously allocated task .  
 * @param[in] _task - _task to be deallocated.
 */
void TaskDestroy(Task* _task);
/**  
 * @brief run task in next run time for that task.  
 * @param[in] _task - Task pointer. 
 * @return 0 - task need to run again / other value - task should be removed from executor.   
 */
int TaskRunWhenTimeIsDue(Task* _task);

/**  
 * @brief set next run time for task.  
 * @param[in] _task - Task pointer. 
 */
void TaskSetRunTime(Task* _task);

/**  
 * @brief compare two task. 
 * @param[in] _firstTask - Task pointer.
 * @param[in] _secondTask - Task pointer.
 * @return 0 - _firstTask < _secondTask / other value - _firstTask >= _secondTask.         
 */
int TaskCompare(const void* _firstTask, const void* _secondTask);

#endif /* __TASK_H__ */
