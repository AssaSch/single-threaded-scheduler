#include "logger.h"
#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* getpid */
#include <sys/time.h> /* gettimeofday */
#include <pthread.h> /* localtime, strftime, pthread_self */

/** 
 * @brief logger for use in aplications. 
 * 
 * @author Asa Schneider
 */ 
 
#define IS_LOGGER(L) ((L) && LOGGER_MAGIC == (L)->m_magic)
#define LOGGER_MAGIC 0xDeadBeaf
#define DEFUALT_LEVEL "I"
#define FILE_START 0
#define BUFFER_SIZE 26
#define MILLI 1000

/****************************************************************************************/

struct Logger
{	
	size_t m_magic;
	FILE* m_log;
	Level m_verbLevel;
};

/****************************************************************************************/

static void InitializeLogger(FILE* _log, FILE* _config, Logger* _logger);
static Level Char2Level(char _letter);
static char Level2Char(Level _level);
static void GetDateAndTime(char (*_dateAndTime)[BUFFER_SIZE] , int* _millisec);

/*****************************************************************************/
/**							LogOpen											**/
/*****************************************************************************/
Logger* LogOpen(char const* _configFileName, char const* _logFileName)
{
	FILE* config;
	FILE* log;
	Logger* logger;
	
	if (!_configFileName || !_logFileName)
	{
		return NULL;
	}
	
	logger = (Logger*)malloc(sizeof(Logger));
	if (!logger)
	{
		return NULL;
	}
	
	config = fopen(_configFileName, "r+");
	if (!config)
	{	
		/* if not exist - create new config file */
		config = fopen(_configFileName, "a+");
		if (!config)
		{
			free(logger);
			return NULL;
		}
		fprintf(config, "%s", DEFUALT_LEVEL);
	}
	
	log = fopen(_logFileName, "a+");
	if (!log)
	{	
		free(logger);
		fclose(config);
		return NULL;
	}
	
	InitializeLogger(log, config, logger);
	fclose(config);
	logger->m_magic = LOGGER_MAGIC;
	
	return logger;
}

/*****************************************************************************/

static void InitializeLogger(FILE* _log, FILE* _config, Logger* _logger)
{
	char letter;
	Level level;	
		
	fseek(_config, FILE_START, SEEK_SET);
	fscanf(_config, "%c", &letter);
	level = Char2Level(letter);
	_logger->m_verbLevel = level;
	_logger->m_log = _log;
}

/*****************************************************************************/
/**							LogWrite										**/
/*****************************************************************************/
void LogWrite(Logger* _logger, Level _level, char const* _description, char const* _fileName, int _line)
{
	char dateAndTime[BUFFER_SIZE];
	int millisec;
	
	if (!IS_LOGGER(_logger) || !_description)
	{
		return;
	}
	
	/* print to file only if level of log is according to configurations */
	if (_level >= _logger->m_verbLevel)
	{
		GetDateAndTime(&dateAndTime, &millisec);
		
		/* print to log file */
		fprintf(_logger->m_log, "%s.%03d %d %lu %c %s %d %s\n", dateAndTime, millisec, getpid(), pthread_self(),
		Level2Char(_level), _fileName, _line, _description);
	}
}
	
/*****************************************************************************/

static void GetDateAndTime(char (*_dateAndTime)[BUFFER_SIZE] , int* _millisec)
{
	struct tm* tm_info;
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	*_millisec = (int)(tv.tv_usec/MILLI);
	
	if (*_millisec>=MILLI) 
	{ 
		*_millisec -=MILLI;
		tv.tv_sec++;
	}

	tm_info = localtime(&tv.tv_sec);

	strftime(*_dateAndTime, BUFFER_SIZE, "%Y:%m:%d %H:%M:%S", tm_info);
}

/*****************************************************************************/

static Level Char2Level(char _letter)
{
	switch (_letter)
	{
		case 'T':
			return T;
		case 'I':
			return I;
		case 'W':
			return W;
		case 'R':
			return R;
		case 'F':
			return F;
		case 'N':
			return N;
	}
	
	return 0;
}

/*****************************************************************************/

static char Level2Char(Level _level)
{
	switch (_level)
	{
		case T:
			return 'T';
		case I:
			return 'I';
		case W:
			return 'W';
		case R:
			return 'R';
		case F:
			return 'F';
		case N:
			return 'N';
	}
	
	return 0;
}

/*****************************************************************************/
/**							LogClose										**/
/*****************************************************************************/
void LogClose(Logger* _logger)
{
	if (!IS_LOGGER(_logger))
	{
		return;
	} 
	
	fclose(_logger->m_log);
	_logger->m_magic = 0;
	free(_logger);
}


