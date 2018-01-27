#ifndef __LOGGER_H__
#define __LOGGER_H__

/** 
 * @brief logger for use in aplications. 
 * 
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */


typedef struct Logger Logger;

extern Logger* logger;

/* if this enum changed - need to update Char2Level and Level2Char functions */
typedef enum Level {
	T,	/** TRACE */
	I,	/** INFO  */
	W,	/** WARNING */
	R,	/** RECOVERY_ERROR */
	F,	/** FATAL_ERROR */
	N	/** NONE */                 
} Level;

/**  
 * @brief Dynamically create a new logger and open files (if files doesnt exist- create them).  
 * @param[in] _configFileName - config file name. if doesnt exist - default level is F.
 * @param[in] _logFileName - log file name.
 * @return Logger* - on success / NULL on fail 
 */
Logger* LogOpen(char const* _configFileName, char const* _logFileName);

/**  
 * @brief macro - for use instead of LogWrite for abstraction reasons.  
 * @param[in] _LOGGER - struct of logger.
 * @params[in] _LEVEL : defined level for writing info into file.
 * @params[in] _DESCRIPTION : description of info line.
 */
#define LOG_WRITE(_LOGGER, _LEVEL, _DESCRIPTION) \
	LogWrite(_LOGGER, _LEVEL, _DESCRIPTION, __FILE__, __LINE__) 
	
/**  
 * @brief close logger at end of use.  
 * @param[in] _logger - struct of logger.
 */
void LogClose(Logger* _logger);

/**  
 * @brief write log information to file according to config level.  
 * @param[in] _logger - struct of logger.
 * @params[in] _level : defined level for writing info into file.
 * @params[in] _description : description of info line.
 * @params[in] _fileName : name of file where the logger wrote to log file.         
 * @params[in] _line : line in module where the logger wrote to log file.         
 */
void LogWrite(Logger* _logger, Level _level, char const* _description, char const* _fileName, int _line);

#endif /* __LOGGER_H__ */
