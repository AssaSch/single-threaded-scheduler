#include "task.h"

#include <stdlib.h> /* malloc & NULL & free */
#include "timeTools.h"

#define TASK_MAGIC 0xDeaDBeeF
#define IS_TASK(T) ((T) && TASK_MAGIC == (T)->m_magic)

/** 
 * @brief functions of task ADT. 
 * 
 * @created 07.10.17
 * @author Asa Schneider
 */ 

struct Task
{
   Func m_func;
   void* m_context;
   size_t m_magic; 
   size_t m_period_ms;
   clockid_t m_clk_id;
   timespec m_runTime;
};

/*****************************************************************************/
                        /* TaskCreate */
/*****************************************************************************/
Task* TaskCreate(Func _func, void* _context, size_t _period_ms, clockid_t _clk_id)
{
	Task* task;
	
	if (!_func)
	{
		return NULL;
	}
	
	task = (Task*)malloc(sizeof(Task));
	if (!task)
	{
		return NULL;
	}
	
	task->m_func = _func;
	task->m_context = _context;
	task->m_period_ms = _period_ms;
	task->m_clk_id = _clk_id;
	task->m_runTime.tv_sec = 0;
	task->m_runTime.tv_nsec = 0;
	task->m_magic = TASK_MAGIC;
	
	return task;
}

/*****************************************************************************/
                        /* TaskDestroy */
/*****************************************************************************/
void TaskDestroy(Task* _task)
{
	if (!IS_TASK((Task*)_task))
	{
		return;
	}
	
	((Task*)_task)->m_magic = 0;
	free(_task);
}

/*****************************************************************************/
                        /* TaskRun */
/*****************************************************************************/
int TaskRunWhenTimeIsDue(Task* _task)
{	
	if (!IS_TASK(_task))
	{
		return 1;
	}
	
	TimeSleep(_task->m_clk_id, _task->m_runTime);
	
	return _task->m_func(_task->m_context);
}

/*****************************************************************************/
                        /* TaskSetRunTime */
/*****************************************************************************/
void TaskSetRunTime(Task* _task)
{
	timespec tm;
	
	tm = TimeGetCurrent(_task->m_clk_id);
	tm = TimeAdd(tm, _task->m_period_ms);
	_task->m_runTime = tm;
	
}

/*****************************************************************************/
                        /* TaskCompare */
/*****************************************************************************/
int TaskCompare(const void* _firstTask, const void* _secondTask)
{
	return TimeCompare(((Task*)_firstTask)->m_runTime, ((Task*)_secondTask)->m_runTime);
}






