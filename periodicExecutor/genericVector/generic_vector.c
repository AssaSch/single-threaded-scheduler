#include "generic_vector.h"
#include <stdio.h>
#include <stdlib.h>
#define MAGIC 0xABCDABCD
#define BLOCK_GAP 2

/****************************************************************************************/

struct Vector
{	
	size_t m_magic;
	void** m_items;
	size_t m_initialCapacity;
	size_t m_size;
	size_t m_nItems;
	size_t m_blockSize;
};

/*typedef void (*elementDestroy)(void* _item);*/

/****************************************************************************************/

static Vector* AllocateMemory(Vector* _pVector, size_t _initialCapacity);
static void InitialateData(Vector* pVector, size_t _initialCapacity, size_t _blockSize);
static Vector_Result IncreaseVecIfNedded(Vector* _vector);
static Vector_Result DecreaseVecIfNeeded(Vector* _vector);
static void FreeElements(Vector** _vector, elementDestroy _elDestroy);
static void FreeVector(Vector** _vector);
static void InsertItem(Vector* _vector, void* _item);
static void RemoveItem(Vector* _vector, void** _pItem);

/* for testing */
int TestForMembers(Vector* _vector, size_t _originalSize, size_t _size, size_t _nItems, size_t _blockSize, size_t _magic);

/****************************************************************************************/
/******************************Vector_Create*********************************************/
/****************************************************************************************/

Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
	Vector* pVector = NULL;
	
	if (!_initialCapacity && !_blockSize)
	{
		return NULL;
	}

	if (NULL == (pVector = AllocateMemory(pVector, _initialCapacity)))
	{
		return NULL;	
	}
	
	InitialateData(pVector, _initialCapacity, _blockSize);
	
	return pVector;
}

/****************************************************************************************/

static Vector* AllocateMemory(Vector* _pVector, size_t _initialCapacity)
{
	_pVector = (Vector*)malloc(sizeof(Vector));
	if (NULL == _pVector)
	{
		return NULL;
	}

	_pVector->m_items = (void**)malloc(_initialCapacity*sizeof(void*));
	if (NULL == _pVector->m_items)
	{
		free(_pVector);
		return NULL;
	}
	
	return _pVector;
}
/****************************************************************************************/

static void InitialateData(Vector* pVector, size_t _initialCapacity, size_t _blockSize)
{
	pVector->m_size = _initialCapacity;
	pVector->m_blockSize = _blockSize;
	pVector->m_nItems = 0;
	pVector->m_initialCapacity = _initialCapacity;
	pVector->m_magic = MAGIC;
}

/****************************************************************************************/
/*************************************VectorDestroy**************************************/
/****************************************************************************************/

void VectorDestroy(Vector** _vector, elementDestroy _elDestroy)
{	
	if (NULL == _vector || NULL == *_vector || MAGIC != (*_vector)->m_magic)
	{
		return;
	}

	FreeElements(_vector, _elDestroy);
	FreeVector(_vector);
}

/****************************************************************************************/

static void FreeElements(Vector** _vector, elementDestroy _elDestroy)
{
	size_t i;
		
	if (NULL != _elDestroy)
	{
		for (i = 0; i < (*_vector)->m_nItems; ++i)
		{
			(*_elDestroy)(*((*_vector)->m_items + i));
		}
	}
}

/****************************************************************************************/

static void FreeVector(Vector** _vector)
{
	(*_vector)->m_magic = 0;	
	free((*_vector)->m_items);
	free(*_vector);
	*_vector = NULL;	
} 

/****************************************************************************************/
/**********************************Vector_Append*****************************************/
/****************************************************************************************/

Vector_Result VectorAppend(Vector* _vector, void* _item)
{
	Vector_Result err;

	if (NULL == _vector)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (VECTOR_SUCCESS != (err = IncreaseVecIfNedded(_vector)))
	{
		return err;
	}
	
	InsertItem(_vector, _item);
		
	return VECTOR_SUCCESS;
}

/****************************************************************************************/

static void InsertItem(Vector* _vector, void* _item)
{
	*(_vector->m_items + _vector->m_nItems) = _item;
	++_vector->m_nItems;
}

/****************************************************************************************/

static Vector_Result IncreaseVecIfNedded(Vector* _vector)
{
	void** temp;
		
	if (_vector->m_size == _vector->m_nItems)
	{
		if (_vector->m_blockSize == 0)
		{
			return VECTOR_OVERFLOW_ERROR;
		}
	
		temp = (void**)realloc(_vector->m_items, (_vector->m_blockSize + _vector->m_size)*sizeof(void*));
		
		if (NULL == temp)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
		
		_vector->m_items = temp;
		_vector->m_size += _vector->m_blockSize;	
	}
	
	return VECTOR_SUCCESS;
}

/****************************************************************************************/
/********************************Vector_Remove*******************************************/
/****************************************************************************************/

Vector_Result VectorRemove(Vector* _vector, void** _pItem)
{
	Vector_Result err;

	if (NULL == _vector || NULL == _pItem)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (_vector->m_nItems == 0)
	{
		return VECTOR_UNDERFLOW_ERROR;
	}
	
	RemoveItem(_vector, _pItem);

	if (VECTOR_SUCCESS != (err = DecreaseVecIfNeeded(_vector)))
	{
		return err;
	}

	return VECTOR_SUCCESS;
}

/****************************************************************************************/

static void RemoveItem(Vector* _vector, void** _pItem)
{
	*_pItem = *(_vector->m_items +_vector->m_nItems - 1);
	--_vector->m_nItems;
} 

/****************************************************************************************/

static Vector_Result DecreaseVecIfNeeded(Vector* _vector)
{
	void** temp;
	size_t size;
	size_t nItems;
	size_t blockSize;
	size_t originSize;
	
	size = _vector->m_size;
	nItems = _vector->m_nItems;
	blockSize = _vector->m_blockSize;
	originSize = _vector->m_initialCapacity;
	
	if (size - nItems >= BLOCK_GAP * blockSize && size - blockSize >= originSize)
	{
		temp = (void**)realloc(_vector->m_items, (size - blockSize)*sizeof(void*));
	
		if (NULL == temp)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
		
		_vector->m_items = temp;
		_vector->m_size -= _vector->m_blockSize;
	}
	
	return VECTOR_SUCCESS;
}

/****************************************************************************************/
/********************************Vector_Get**********************************************/
/****************************************************************************************/

Vector_Result VectorGet(const Vector* _vector, size_t _index, void** _pItem)
{
	if (NULL ==_vector || NULL == _pItem)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (_index > _vector->m_nItems || !_index)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pItem = *(_vector->m_items + _index - 1);
	
	return VECTOR_SUCCESS;
}

/****************************************************************************************/
/********************************Vector_Set**********************************************/
/****************************************************************************************/

Vector_Result VectorSet(Vector* _vector, size_t _index, void*  _item)
{
	if (NULL == _vector)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (_index > _vector->m_nItems || !_index)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	
	*(_vector->m_items + _index - 1) = _item;

	return VECTOR_SUCCESS;
}

/****************************************************************************************/
/**********************************Vector_Size*******************************************/
/****************************************************************************************/

size_t VectorSize(const Vector* _vector)
{
	
	if (NULL == _vector)
	{
		return 0;
	}

	return _vector->m_nItems;
}

/****************************************************************************************/
/**********************************Vector_Capacity***************************************/
/****************************************************************************************/

size_t VectorCapacity(const Vector* _vector)
{
	if (NULL == _vector)
	{
		return 0;
	}

	return _vector->m_size;
}

/****************************************************************************************/
/*******************************Vector_ForEach*******************************************/
/****************************************************************************************/

size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	size_t i;
	size_t numOfElemets;
	void* pItem;
	
	if (NULL == _vector || NULL == _action)
	{
		return 0;
	}
	
	numOfElemets = VectorSize(_vector);
	
	for(i = 0; i < numOfElemets; ++i)
	{
		VectorGet(_vector, i, &pItem);
		if (!_action(pItem, i, _context))
		{
			break;
		}	
	}
	
	return i;
}

/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/
/* for testing */
int TestForMembers(Vector* _vector, size_t _originalSize, size_t _size, size_t _nItems, size_t _blockSize, size_t _magic)
{
	if (_vector->m_initialCapacity == _originalSize && _vector->m_size == _size && _vector->m_nItems == _nItems && _vector->m_blockSize == _blockSize && _vector->m_magic == _magic )
	{
		return 1;
	}
	else
	{
		return 0;
	} 
}

/****************************************************************************************/
/****************************************************************************************/
















