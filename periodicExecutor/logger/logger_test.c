#include "logger.h"
#include "mu_test.h"
#include <stdio.h>
/*#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>*/

#define NUM_OF_THREADS 2


void* PrintFunc(void* logger)
{	
	LOG_WRITE((Logger*)logger, F, "DDD");

	return NULL;
}
	
/*****************************************************************************/

UNIT(LogOpen_null)
	
	ASSERT_THAT(!LogOpen(NULL ,"log"));
	ASSERT_THAT(!LogOpen("config" ,NULL));
	
END_UNIT

/*****************************************************************************/

UNIT(LogOpen_ok)
	
	Logger* logger;
	
	ASSERT_THAT(logger = LogOpen("config" ,"log"));
	LogClose(logger);
		
END_UNIT

/*****************************************************************************/

UNIT(LogWrite_null)
	
	Logger* logger;
	
	ASSERT_THAT(logger = LogOpen("config" ,"log"));
	LOG_WRITE(NULL, F, "AAA");
	LOG_WRITE(logger, F, NULL);
	LogClose(logger);
	
END_UNIT

/*****************************************************************************/

UNIT(LogWrite_ok)
	
	

	Logger* logger;
	/*int i;
	pthread_t threads[NUM_OF_THREADS];*/
	
	ASSERT_THAT(logger = LogOpen("config" ,"log"));
	LOG_WRITE(logger, F, "AAA");
	LOG_WRITE(logger, R, "BBB");
	LOG_WRITE(logger, W, "CCC");
	
	/*for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_create(&threads[i],NULL ,&PrintFunc, (void*)logger);
	}*/
	
	LogClose(logger);
	
END_UNIT

/*****************************************************************************/

UNIT(LogClose_ok)
	
	Logger* logger;
	
	ASSERT_THAT(logger = LogOpen("config" ,"log"));
	LOG_WRITE(logger, F, "EEE");
	
	LogClose(logger);
	LogClose(logger);
	
END_UNIT

/*****************************************************************************/

TEST_SUITE(framework test)

	TEST(LogOpen_null)	
	TEST(LogOpen_ok)
	TEST(LogWrite_null)
	TEST(LogWrite_ok)
	TEST(LogClose_ok)		
																															
END_SUITE

/*****************************************************************************/
	
	
