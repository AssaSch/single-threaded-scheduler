#include "execute_container.h"

#include <stdlib.h> /* malloc & NULL & free */
#include "task.h"
#include "genericHeap.h"
#include "generic_vector.h"

#define CONTAINER_MAGIC 0xDeaDBeeF
#define IS_CONTAINER(C) ((C) && CONTAINER_MAGIC == (C)->m_magic)
#define VECTOR_SIZE 1
#define VECTOR_BLOCK 1

/** 
 * @brief functions of container for Executor. 
 * 
 * @created 07.10.17 
 * @author Asa Schneider
 */ 

struct ExecutorContainer
{
   size_t m_magic; 
   Heap* m_heap;
   Vector* m_vector;
};

/*****************************************************************************/

static void VectorAndTasksDestroy(Vector* _vector);

/*****************************************************************************/
                        /* ExecutorContainerCreate */
/*****************************************************************************/
ExecutorContainer* ExecutorContainerCreate()
{
	ExecutorContainer* container;
	Heap* heap;
	Vector* heapVector;
	Vector* vector;

	vector = VectorCreate(VECTOR_SIZE, VECTOR_BLOCK);
	if (!vector)
	{
		return NULL;
	}
	
	heapVector = VectorCreate(VECTOR_SIZE, VECTOR_BLOCK);
	if (!heapVector)
	{
		VectorDestroy(&vector,NULL);
		return NULL;
	}
	
	heap = HeapBuild(heapVector, TaskCompare);
	if (!heap)
	{
		VectorDestroy(&vector, NULL);
		VectorDestroy(&heapVector, NULL);
		return NULL;
	}
	
	container = (ExecutorContainer*)malloc(sizeof(ExecutorContainer));
	if (!container)
	{
		VectorDestroy(&vector, NULL);
		VectorDestroy(&heapVector, NULL);
		HeapDestroy(&heap);
		return NULL;
	}
	
	container->m_heap = heap;
	container->m_vector = vector;
	container->m_magic = CONTAINER_MAGIC;
	
	return container;
}

/*****************************************************************************/
                        /* ExecutorContainerDestroy */
/*****************************************************************************/
void ExecutorContainerDestroy(ExecutorContainer* _container)
{
	Vector* vector;
	
	if (!IS_CONTAINER(_container))
	{
		return;
	}
	
	/* free all tasks in storage vector and destroy vector */
	VectorAndTasksDestroy(_container->m_vector);
	
	vector = HeapDestroy(&(_container->m_heap));
	
	/* free all tasks in heap vector and destroy vector */
	VectorAndTasksDestroy(vector);
	_container->m_magic = 0;
	free(_container);
}

/*****************************************************************************/

static void VectorAndTasksDestroy(Vector* _vector)
{
	void* pItem;
	
	while (VectorSize(_vector))
	{
		if (VectorRemove(_vector, &pItem))
		{
			return;
		}
		TaskDestroy((Task*)pItem);
	}
	VectorDestroy(&(_vector), NULL);
}

/*****************************************************************************/
                        /* ExecutorContainerAdd */
/*****************************************************************************/
int ExecutorContainerAdd(ExecutorContainer* _container, void* _element)
{
	if (!IS_CONTAINER(_container))
	{
		return 1;
	}
	
	if (VectorAppend(_container->m_vector, _element))
	{
		return 1;
	}
	
	return 0;
}

/*****************************************************************************/
                        /* ExecutorContainerInsert */
/*****************************************************************************/
int ExecutorContainerInsert(ExecutorContainer* _container)
{
	void* pItem;
	
	if (!IS_CONTAINER(_container))
	{
		return 1;
	}
	
	/* set run time and move tasks from storage vector to heap */
	while (VectorSize(_container->m_vector))
	{
		if (VectorRemove(_container->m_vector, &pItem))
		{
			return 1;
		}
		
		TaskSetRunTime(pItem);
		
		if (HeapInsert(_container->m_heap, pItem))
		{
			return 1;
		}
	}
	
	return 0;
}

/*****************************************************************************/
                        /* ExecutorContainerExtract */
/*****************************************************************************/
void* ExecutorContainerExtract(ExecutorContainer* _container)
{
	if (!IS_CONTAINER(_container))
	{
		return NULL;
	}
	
	return HeapExtract(_container->m_heap);
}

/*****************************************************************************/
                        /* ExecutorContainerSize */
/*****************************************************************************/
size_t ExecutorContainerSize(const ExecutorContainer* _container)
{
	if (!IS_CONTAINER(_container))
	{
		return 0;
	}
	
	return HeapSize(_container->m_heap);
}








