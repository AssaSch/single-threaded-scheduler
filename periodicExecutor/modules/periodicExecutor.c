#include "periodicExecutor.h"

#include <stdlib.h> /* malloc & NULL & free */
#include <time.h> /* clockid_t */
#include "task.h"
#include "execute_container.h"
#include "logger.h"

#define IS_EXECUTOR(E) ((E) && EXECUTOR_MAGIC == (E)->m_magic)
#define IS_PAUSED(E) ((E)->m_paused)
#define IS_FIRST_RUN(E) (!((E)->m_firstRun))
#define EXECUTOR_MAGIC 0xDeaDBeaF
#define STOP 1
#define CONTINUE 0


/** 
 * @brief Periodic Executor. 
 * @created 07.10.17
 * @author Asa Schneider
 */ 

Logger* logger; /* global variable */

struct PeriodicExecutor
{
	size_t m_magic;
	size_t m_paused;
	size_t m_runCounter;
	clockid_t m_clk_id;
	ExecutorContainer* m_container;
}; 

/*****************************************************************************/

static void InitializeExecutor(PeriodicExecutor* _executor, ExecutorContainer* _container, clockid_t _clk_id);
static int ReturnTaskToContainer(ExecutorContainer* _container, Task* _task);

/*****************************************************************************/
                        /* PeriodicExecutorCreate */
/*****************************************************************************/
PeriodicExecutor* PeriodicExecutorCreate(clockid_t _clk_id)
{
	PeriodicExecutor* executor;
	ExecutorContainer* container;
	
	logger = LogOpen("executorConfig", "executorLog");
	
	executor = (PeriodicExecutor*)malloc(sizeof(PeriodicExecutor));
	if (!executor)
	{
		return NULL;
	}
	
	container = ExecutorContainerCreate();
	if (!container)
	{
		LOG_WRITE(logger, R, "creation of container failed");
		free(executor);
		return NULL;
	}
	
	InitializeExecutor(executor, container, _clk_id);
	LOG_WRITE(logger, I, "creation of executor");
	
	return executor;	
}

/*****************************************************************************/

static void InitializeExecutor(PeriodicExecutor* _executor, ExecutorContainer* _container, clockid_t _clk_id)
{
	_executor->m_container = _container;
	_executor->m_clk_id = _clk_id;
	_executor->m_paused = CONTINUE;
	_executor->m_runCounter = 0;
	_executor->m_magic = EXECUTOR_MAGIC;
}

/*****************************************************************************/
                        /* PeriodicExecutorDestroy */
/*****************************************************************************/
void PeriodicExecutorDestroy(PeriodicExecutor* _executor)
{

	LOG_WRITE(logger, I, "destroy command");
		
	if (!IS_EXECUTOR(_executor))
	{
		LOG_WRITE(logger, F, "error - executor not exist - Detroy");
		LogClose(logger);
		return;
	}
	
	ExecutorContainerDestroy(_executor->m_container);
	_executor->m_magic = 0;
	free(_executor);
	
	LOG_WRITE(logger, I, "destroy executor");
	LogClose(logger);
}

/*****************************************************************************/
                        /* PeriodicExecutorAdd */
/*****************************************************************************/
int PeriodicExecutorAdd(PeriodicExecutor* _executor, Func _func, void* _context, size_t _period_ms)
{
	Task* task;
	
	LOG_WRITE(logger, I, "add command");
		
	if (!IS_EXECUTOR(_executor) || !_func)
	{
		LOG_WRITE(logger, F, "error - executor not exist - Add");
		return 0;
	}
	
	task = TaskCreate(_func, _context, _period_ms, _executor->m_clk_id);
	if (!task)
	{
		LOG_WRITE(logger, R, "creation of task failed");
		return 0;
	}
	
	if (ExecutorContainerAdd(_executor->m_container, task))
	{
		LOG_WRITE(logger, R, "add task to container failed");
		return 0;
	}
	
	LOG_WRITE(logger, I, "added task");
	
	return 1;
}

/*****************************************************************************/
                        /* PeriodicExecutorRun */
/*****************************************************************************/
size_t PeriodicExecutorRun(PeriodicExecutor* _executor)
{
	Task* task;
	
	LOG_WRITE(logger, I, "run command");
		
	if (!IS_EXECUTOR(_executor))
	{
		LOG_WRITE(logger, F, "error - executor not exist - Run");
		return 0;
	}
	
	_executor->m_paused = CONTINUE;
	
	/* set run time and move tasks from storage vector to heap */
	if (ExecutorContainerInsert(_executor->m_container))
	{	
		LOG_WRITE(logger, R, "insert tasks to container failed");
		return 0;
	}
	
	while (!IS_PAUSED(_executor) && ExecutorContainerSize(_executor->m_container))
	{
		/* extract earliest task from container */
		if (! (task = ExecutorContainerExtract(_executor->m_container)))
		{
			LOG_WRITE(logger, R, "extract task from container failed");
			return 0;
		}
		
		/* return task to container if need to run again */
		if (! TaskRunWhenTimeIsDue(task))
		{	
			if (! ReturnTaskToContainer(_executor->m_container, task))
			{
				return 0;
			}
		}
		else
		{
			LOG_WRITE(logger, I, "TASK DESTROYED");
			TaskDestroy(task);
		}
		++(_executor->m_runCounter);
	}
	
	LOG_WRITE(logger, I, "finish run command");
		
	return _executor->m_runCounter;
}

/*****************************************************************************/

static int ReturnTaskToContainer(ExecutorContainer* _container, Task* _task)
{
	if (ExecutorContainerAdd(_container, _task))
	{
		LOG_WRITE(logger, R, "add task to container failed");
		return 0;
	}
	
	if (ExecutorContainerInsert(_container))
	{
		LOG_WRITE(logger, R, "insert tasks to container failed");
		return 0;
	}
	
	return 1;
}

/*****************************************************************************/
                        /* PeriodicExecutorPause */
/*****************************************************************************/
size_t PeriodicExecutorPause(PeriodicExecutor* _executor)
{
	LOG_WRITE(logger, I, "pause command");
		
	if (!IS_EXECUTOR(_executor))
	{
		LOG_WRITE(logger, F, "error - executor not exist - Pause");
		return 0;
	}
	
	_executor->m_paused = STOP;
	LOG_WRITE(logger, I, "pause");
	
	return _executor->m_runCounter;
}















