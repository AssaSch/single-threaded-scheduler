#ifndef __EXECUTOR_CONTAINER_H__
#define __EXECUTOR_CONTAINER_H__

/** 
 * @brief API of container for Executor. 
 * 
 * @created 07.10.17 
 * @author Asa Schneider
 */ 

#include <stddef.h>  /* size_t */

typedef struct ExecutorContainer ExecutorContainer;

/**  
 * @brief Dynamically create a new Executor Container.  
 * @return ExecutorContainer* - on success / NULL on fail. 
 */
ExecutorContainer* ExecutorContainerCreate();

/**  
 * @brief Dynamically deallocate a previously allocated Container .  
 * @param[in] _container - Container to be deallocated.
 */
void ExecutorContainerDestroy(ExecutorContainer* _container);

/**  
 * @brief Adds element to temporary storage.  
 * @param[in] _container - container pointer.
 * @param[in] _element - element to be saved in container. 
 * @return 0 on seccess / 1 if failed.  
 */
int ExecutorContainerAdd(ExecutorContainer* _container, void* _element);

/**  
 * @brief update run time and insert all tasks from temporary storage to the container.  
 * @param[in] _container - container pointer.
 * @return 0 on seccess / 1 if failed.
 */
int ExecutorContainerInsert(ExecutorContainer* _container);

/**  
 * @brief Extract element on top of heap and remove it.   
 * @param[in] _container - container pointer.
 * @return element pointer - on success / NULL on fail.          
 */
void* ExecutorContainerExtract(ExecutorContainer* _container);

/**  
 * @brief Get the current size (number of elements) in the container. 
 * @param[in] _container - container pointer.
 * @return number of elements or zero if empty.        
 */
size_t ExecutorContainerSize(const ExecutorContainer* _container);

#endif /* __EXECUTOR_CONTAINER_H__ */
