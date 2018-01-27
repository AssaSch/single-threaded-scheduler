#include "genericHeap.h"
#include "generic_vector.h"
#include <stdlib.h> /* for malloc & NULL & free */
#include <stdio.h>

#define HEAP_MAGIC 0xDeaDBeeF

#define PARANT(i) ((i))/2
#define LEFT(i) (2*(i))
#define RIGHT(i) (2*(i)+1)
#define YOUNGEST_PARENT(N) (N/2)

#define HEAP_VECTOR(H) (H)->m_vec
#define IS_HEAP(N) ((N) && (N)->m_magic == HEAP_MAGIC)
#define LESS_FUNC(N) (N)->m_less

#define FIRST_ELEMENT 1

/*****************************************************************************/
/*****************************************************************************/

/*functiond for generic binary heap data structre */
/*created by Asa Schneider */
/* 13.09.2017 */

/*****************************************************************************/
/*****************************************************************************/

struct Heap
{
   size_t m_magic; 
   Vector* m_vec;
   LessThanComparator m_less;
   size_t m_heapSize;
};

/*****************************************************************************/

int HeapGet(Heap* heap, size_t index);
static void Heapify(Heap* _heap, size_t _index);
static void SetIndexesAndValues(Heap* _heap, size_t _index, size_t* _left, size_t* _right, size_t* _largest,
void** _valueLeft, void** _valueRight, void** _valueIndex, void** _valueLargest); 
static void InitializeHeap(Heap* _heap, Vector* _vector, LessThanComparator _pfLess, size_t _numOfElements);
static void SwapFirstLast(Heap* _heap, void* _firstValue, void* _lastValue);
static void BubbleUp(Heap* _heap, size_t _index, size_t _ParantIndex);
static size_t DoActionOnHeap(const Heap* _heap, ActionFunction _act, void* _context, size_t _numOfElemets);

/*****************************************************************************/
                        /* HeapBuild */
/*****************************************************************************/
Heap* HeapBuild(Vector* _vector, LessThanComparator _pfLess)
{
	Heap* heap;
	size_t numOfElements;
	size_t index;

	if (!_vector || !_pfLess)
	{
		return NULL;
	}
	
	heap = (Heap*)malloc(sizeof(Heap));
	if (!heap)
	{
		return NULL;
	}
	
	numOfElements = VectorSize(_vector);	
	InitializeHeap(heap, _vector,_pfLess, numOfElements);
	
	for (index = YOUNGEST_PARENT(numOfElements); index >= FIRST_ELEMENT; index--)
	{
		Heapify(heap, index);
	}
	
	heap->m_magic = HEAP_MAGIC;
	
	return heap;
}

/*****************************************************************************/

static void Heapify(Heap* _heap, size_t _index)
{
	size_t left;
	size_t right;
	size_t largest;
	void* valueLeft;
	void* valueRight;
	void* valueLargest;
	void* valueIndex;
	size_t numOfElements;
	
	numOfElements = _heap->m_heapSize;
	if (_index > numOfElements)
	{
		return;
	}
	
	SetIndexesAndValues(_heap, _index, &left, &right, &largest, &valueLeft, &valueRight, &valueIndex, &valueLargest);
	
	/*if valueIndex < valueLeft save left as largest */
	if (left <= numOfElements && LESS_FUNC(_heap)(valueIndex, valueLeft))
	{	
		valueLargest = valueLeft;
		largest = left;
	}
	
	/*if valueLargest < valueRight save right as largest */
	if (right <= numOfElements && LESS_FUNC(_heap)(valueLargest, valueRight))
	{
		valueLargest = valueRight;
		largest = right;
	}
	
	/*if index is not the largest then swap with the largest and heapify again */
	if (largest != _index)
	{
		VectorSet(HEAP_VECTOR(_heap), _index, valueLargest);
		VectorSet(HEAP_VECTOR(_heap), largest, valueIndex);
		Heapify(_heap, largest);
	}
	else
	{
		return;
	}
}

/*****************************************************************************/

static void SetIndexesAndValues(Heap* _heap, size_t _index, size_t* _left, size_t* _right, size_t* _largest,
void** _valueLeft, void** _valueRight, void** _valueIndex, void** _valueLargest) 
{
	*_left = LEFT(_index);
	*_right = RIGHT(_index);
	*_largest = _index;
	
	VectorGet(HEAP_VECTOR(_heap), *_left, _valueLeft);
	VectorGet(HEAP_VECTOR(_heap), *_right, _valueRight);
	VectorGet(HEAP_VECTOR(_heap), _index, _valueIndex);
	VectorGet(HEAP_VECTOR(_heap), *_largest, _valueLargest);
}


/*****************************************************************************/

static void InitializeHeap(Heap* _heap, Vector* _vector, LessThanComparator _pfLess, size_t _numOfElements)
{
	HEAP_VECTOR(_heap) = _vector;
	_heap->m_heapSize = _numOfElements;
	_heap->m_less = _pfLess;
}

/*****************************************************************************/
                        /* HeapDestroy */
/*****************************************************************************/
Vector* HeapDestroy(Heap** _heap)
{
	Vector* vector;

	if (!_heap || !IS_HEAP(*_heap))
	{	
		return NULL;
	}
	
	vector = HEAP_VECTOR(*_heap);
	
	(*_heap)->m_magic = (size_t)(-1);
	free(*_heap);
	*_heap = NULL;
	
	return vector;
}

/*****************************************************************************/
                        /* HeapPeek */
/*****************************************************************************/
const void* HeapPeek(const Heap* _heap)
{	
	void* data;
	
	if (!IS_HEAP(_heap) || !(_heap->m_heapSize))
	{
		return NULL;
	}
	
	VectorGet(HEAP_VECTOR(_heap), FIRST_ELEMENT, &data);
	
	return data;
}

/*****************************************************************************/
                        /* HeapSize */
/*****************************************************************************/
size_t HeapSize(const Heap* _heap)
{
	if (!IS_HEAP(_heap))
	{
		return 0;
	}
	
	return _heap->m_heapSize;
}

/*****************************************************************************/
                        /* HeapExtract */
/*****************************************************************************/
void* HeapExtract(Heap* _heap)
{
	void* data = NULL;
	void* firstValue = NULL;
	void* lastValue = NULL;

	if (!IS_HEAP(_heap) || !(_heap->m_heapSize))
	{
		return NULL;
	}
	
	SwapFirstLast(_heap, firstValue, lastValue);
	
	if (VECTOR_SUCCESS != VectorRemove(HEAP_VECTOR(_heap), &data))
	{
		return NULL;
	}
	 
	--(_heap->m_heapSize);
	Heapify(_heap, FIRST_ELEMENT);
	
	return data;
}

/*****************************************************************************/

static void SwapFirstLast(Heap* _heap, void* _firstValue, void* _lastValue)
{
	VectorGet(HEAP_VECTOR(_heap), FIRST_ELEMENT, &_firstValue);
	VectorGet(HEAP_VECTOR(_heap), _heap->m_heapSize, &_lastValue);
	VectorSet(HEAP_VECTOR(_heap), FIRST_ELEMENT, _lastValue);
	VectorSet(HEAP_VECTOR(_heap), _heap->m_heapSize, _firstValue);
}

/*****************************************************************************/
                        /* HeapInsert */
/*****************************************************************************/
Heap_Result HeapInsert(Heap* _heap, void* _element)
{
	size_t index; 
	size_t ParantIndex;
	
	if (!IS_HEAP(_heap) || !_element)
	{
		return HEAP_NOT_INITIALIZED;
	}
	
	if ((VectorAppend(HEAP_VECTOR(_heap), _element)) != VECTOR_SUCCESS)
	{
		return HEAP_REALLOCATION_FAILED;
	}
	++(_heap->m_heapSize);
		
	index = _heap->m_heapSize;
	ParantIndex = PARANT(index);
	BubbleUp(_heap, index, ParantIndex);
	
	return HEAP_SUCCESS;
}

/*****************************************************************************/

static void BubbleUp(Heap* _heap, size_t _index, size_t _ParantIndex)
{
	void* valueParant;
	void* valueIndex;

	while (_ParantIndex >= FIRST_ELEMENT)
	{
		VectorGet(HEAP_VECTOR(_heap), _ParantIndex, &valueParant);
		VectorGet(HEAP_VECTOR(_heap), _index, &valueIndex);		
		
		/*if valueParant < valueIndex swap them */
		if (LESS_FUNC(_heap)(valueParant, valueIndex))
		{
			VectorSet(HEAP_VECTOR(_heap), _ParantIndex, valueIndex);
			VectorSet(HEAP_VECTOR(_heap), _index, valueParant);			
		}
		
		--_index;
		_ParantIndex = PARANT(_index);	
	}  
} 

/*****************************************************************************/
                        /* HeapForEach */
/*****************************************************************************/
size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
	size_t numOfElemets;
	
	if (!IS_HEAP(_heap) || !_act)
	{
		return 0;
	}
	
	numOfElemets = HeapSize(_heap);
	
	return DoActionOnHeap(_heap, _act, _context, numOfElemets);
}

/*****************************************************************************/

static size_t DoActionOnHeap(const Heap* _heap, ActionFunction _act, void* _context, size_t _numOfElemets)
{
	size_t i;
	void* pItem;	
	
	for(i = 1; i <= _numOfElemets; ++i)
	{
		VectorGet(HEAP_VECTOR(_heap), i, &pItem);
		if (!_act(pItem, _context))
		{
			break;
		}	
	}
	
	return i-1;
}

/*****************************************************************************/
                        /* HeapSort */
/*****************************************************************************/
void HeapSort(Vector* _vec, LessThanComparator _pfLess)
{
	Heap* heap;
	void* data;
	size_t i; 
	size_t numOfElements; 
	
	if (!(heap = HeapBuild(_vec, _pfLess)))
	{
		return;
	}
	
	numOfElements = heap->m_heapSize; 
	
	/* remove max element and add it to end of vector */
	for (i = FIRST_ELEMENT; i <= numOfElements; ++i)
	{
		data = HeapExtract(heap);
		VectorAppend(_vec, data);
	}
	
	HeapDestroy(&heap);
}


/*****************************************************************************/

int HeapGet(Heap* heap, size_t index)
{
	void* value;
	
	VectorGet(heap->m_vec, index, &value);
	
	return *((int*)(value));  
}

/*****************************************************************************/
















