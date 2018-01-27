#include "periodicExecutor.h"
#include "mu_test.h"
#include <stdio.h> /* printf */

/*****************************************************************************/
/*****************************************************************************/

typedef size_t (*Pause)(PeriodicExecutor* _executor);

typedef struct Context
{
	Pause m_Pause;
	PeriodicExecutor* m_executor;
} Context;

/*****************************************************************************/
/*****************************************************************************/
                        /* Tasks */
/*****************************************************************************/
int printOne(void* _context)
{
	printf("one\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 0;
}

int printTwo(void* _context)
{
	printf("two\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 0;
}

int printThree(void* _context)
{
	printf("three\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 1;
}

int printFour(void* _context)
{
	printf("four\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 0;
}

int printFive(void* _context)
{
	printf("five\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 0;
}

int printSix(void* _context)
{
	printf("Six\n");
	
	if (_context)
	{
		((Context*)_context)->m_Pause(((Context*)_context)->m_executor);
	}
	
	return 0;
}

/*****************************************************************************/
/*****************************************************************************/

UNIT(PeriodicExecutorCreate_ok)
	
	PeriodicExecutor* executor;
	
	ASSERT_THAT(executor = PeriodicExecutorCreate(CLOCK_REALTIME));
	
	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorAdd_null)
	
	PeriodicExecutor* executor;
	
	ASSERT_THAT(! PeriodicExecutorAdd(NULL, printOne, NULL, 5));
	
	executor = PeriodicExecutorCreate(CLOCK_REALTIME);
	ASSERT_THAT(! PeriodicExecutorAdd(executor, NULL, NULL, 5));
	
	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorAdd_ok)
	
	PeriodicExecutor* executor;
	
	executor = PeriodicExecutorCreate(CLOCK_REALTIME);
	ASSERT_THAT(PeriodicExecutorAdd(executor, printOne, NULL, 5));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printTwo, NULL, 5));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printThree, NULL, 5));
	
	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorRun_null)
	
	ASSERT_THAT(! PeriodicExecutorRun(NULL));
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorRun_no_add)
	
	PeriodicExecutor* executor;
	
	executor = PeriodicExecutorCreate(CLOCK_REALTIME);
	ASSERT_THAT(0 == PeriodicExecutorRun(executor));
	
	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorRun_ok)
	
	PeriodicExecutor* executor;
	Context context;
	
	executor = PeriodicExecutorCreate(CLOCK_REALTIME);

	context.m_Pause = PeriodicExecutorPause;
	context.m_executor = executor;
	
	ASSERT_THAT(PeriodicExecutorAdd(executor, printOne, NULL, 5));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printTwo, NULL, 17));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printThree, (void*)(&context), 21));
	ASSERT_THAT(4 == PeriodicExecutorRun(executor));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printFour, NULL, 12));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printFive, NULL, 7));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printSix, (void*)(&context), 36));
	ASSERT_THAT(23 == PeriodicExecutorRun(executor));

	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/

UNIT(PeriodicExecutorDestroy_ok)
	
	PeriodicExecutor* executor;
	Context context;
	
	executor = PeriodicExecutorCreate(CLOCK_REALTIME);
	context.m_Pause = PeriodicExecutorPause;
	context.m_executor = executor;
	
	ASSERT_THAT(PeriodicExecutorAdd(executor, printOne, NULL, 5));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printTwo, NULL, 17));
	ASSERT_THAT(PeriodicExecutorAdd(executor, printThree, (void*)(&context), 42));
		
	PeriodicExecutorDestroy(executor);
	PeriodicExecutorDestroy(executor);
	
END_UNIT

/*****************************************************************************/
/*****************************************************************************/

TEST_SUITE(framework test)

	TEST(PeriodicExecutorCreate_ok)
	TEST(PeriodicExecutorAdd_null)
	TEST(PeriodicExecutorAdd_ok)
	TEST(PeriodicExecutorRun_null)
	TEST(PeriodicExecutorRun_no_add)
	TEST(PeriodicExecutorRun_ok)
	TEST(PeriodicExecutorDestroy_ok)
																															
END_SUITE

/*****************************************************************************/
	
	
