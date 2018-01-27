#include "genericHeap.h"
#include "mu_test.h"
#include <stdio.h>
#include <stdlib.h> /* for free*/

/*****************************************************************************/

int	LessThanCompare(const void *_left, const void *_right);
void StructPersonDestroy(void* _item);
int	HeapPrint(const void *_elem, void * _context);
int HeapGet(Heap* heap, size_t index);

/*****************************************************************************/

int	LessThanCompare(const void* _left, const void* _right)
{
	const int* first = _left;
	const int* second = _right;
	
	if (*first < *second)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/

int	HeapPrint(const void* _elem, void* _context)
{
	
	printf("%d - %s \n", *((int*)_elem), (char*)_context);
	
	return 1;
}

/*****************************************************************************/
/*****************************************************************************/

UNIT(HeapBuild_null)

	ASSERT_THAT(HeapBuild(NULL, NULL) == NULL);

END_UNIT

/*****************************************************************************/

UNIT(HeapBuild_ok)

	Heap* heap;
	Vector* vector;
	int item[4] = {10, 8, 12 ,4};
	int value[4];
	size_t i;
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	heap = HeapBuild(vector, LessThanCompare);
	
	for (i = 1; i <= 4; ++i)
	{
		value[i-1] = HeapGet(heap, i);
		printf("%d ", value[i-1]);
		printf("\n"); 
	}
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapPeek_null)
	
	ASSERT_THAT(HeapPeek(NULL) == NULL);

END_UNIT

/*****************************************************************************/

UNIT(HeapPeek_ok)
	
	Heap* heap;
	Vector* vector;
	int item[4] = {10, 8, 12 ,4};
	int* res;
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	heap = HeapBuild(vector, LessThanCompare);
	res = (int*)HeapPeek(heap);
	ASSERT_THAT(*res == 12);
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapSize_ok)
	
	Heap* heap;
	Vector* vector;
	int item[4] = {10, 8, 12 ,4};
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	heap = HeapBuild(vector, LessThanCompare);
	ASSERT_THAT(HeapSize(heap) == 4);
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapExtract_null)
	
	Heap* heap;
	Vector* vector;	
	
	vector = VectorCreate(1, 1);
	heap = HeapBuild(vector, LessThanCompare);
	ASSERT_THAT(HeapExtract(NULL) == NULL);
	ASSERT_THAT(HeapExtract(heap) == NULL);
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapExtract_ok)
	
	Heap* heap;
	Vector* vector;
	int item[4] = {10, 8, 12 ,4};
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	heap = HeapBuild(vector, LessThanCompare);
	ASSERT_THAT(*((int*)HeapExtract(heap)) == 12);
	ASSERT_THAT(*((int*)HeapExtract(heap)) == 10);	
	ASSERT_THAT(*((int*)HeapExtract(heap)) == 8);
	ASSERT_THAT(*((int*)HeapExtract(heap)) == 4);		
	ASSERT_THAT(HeapSize(heap) == 0);
		
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);


END_UNIT

/*****************************************************************************/

UNIT(HeapInsert_null)
	
	Heap* heap;
	Vector* vector;
	int num = 3;
	
	vector = VectorCreate(1, 1);
	heap = HeapBuild(vector, LessThanCompare);
	ASSERT_THAT(HeapInsert(NULL, &num) == HEAP_NOT_INITIALIZED);
	ASSERT_THAT(HeapInsert(heap, NULL) == HEAP_NOT_INITIALIZED);
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapInsert_ok)
	
	Heap* heap;
	Vector* vector;
	int item[5] = {10, 8, 12 ,4, 22};
	int* res;
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	heap = HeapBuild(vector, LessThanCompare);
	HeapInsert(heap, &item[4]);
	
	ASSERT_THAT(HeapSize(heap) == 5);
	res = (int*)HeapPeek(heap);
	ASSERT_THAT(*res == 22);
	ASSERT_THAT(HeapGet(heap, 2) == 12);
	ASSERT_THAT(HeapGet(heap, 3) == 10);
	ASSERT_THAT(HeapGet(heap, 4) == 4);
	ASSERT_THAT(HeapGet(heap, 5) == 8);
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapForEach_null)
	
	ASSERT_THAT((HeapForEach(NULL, NULL, NULL) == 0));

END_UNIT

/*****************************************************************************/

UNIT(HeapForEach_ok)
	
	Heap* heap;
	Vector* vector;
	int item[6] = {10, 5, 8 ,1, 17, 20};
	char* context = "hallo";
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);
	VectorAppend(vector, &item[4]);	
	VectorAppend(vector, &item[5]);		
	heap = HeapBuild(vector, LessThanCompare);
	ASSERT_THAT(6 == HeapForEach(heap, HeapPrint, (void*)context));
	 
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/

UNIT(HeapSort_ok)
	
	Heap* heap;
	Vector* vector;
	int item[8] = {2, 5, 8 ,1, 4, 7, 6, 3};
	size_t i;
	void* pItem;
	
	vector = VectorCreate(1, 1);
	
	for (i = 1; i <= 8; ++i)
	{
		VectorAppend(vector, &item[i-1]); 
	}
	
	HeapSort(vector, LessThanCompare);
	
	for (i = 1; i <= 8; ++i)
	{
		VectorGet(vector, i, &pItem);
		printf("%d ", *((int*)pItem));
		printf("\n"); 
	}
	 
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);

END_UNIT

/*****************************************************************************/


UNIT(HeapDestroy_ok)
	
	Heap* heap;
	Vector* vector;
	int item[5] = {2, 5, 3 ,1, 4};
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, &item[0]);
	VectorAppend(vector, &item[1]);
	VectorAppend(vector, &item[2]);
	VectorAppend(vector, &item[3]);	
	VectorAppend(vector, &item[4]);	
	heap = HeapBuild(vector, LessThanCompare);
	
	VectorDestroy(&vector, NULL);
	HeapDestroy(&heap);
	HeapDestroy(&heap);
	
	
END_UNIT

/*****************************************************************************/

TEST_SUITE(framework test)

	TEST(HeapBuild_null)
	TEST(HeapBuild_ok)
	TEST(HeapPeek_null)
	TEST(HeapPeek_ok)
	TEST(HeapSize_ok)
	TEST(HeapExtract_null)
	TEST(HeapExtract_ok)
	TEST(HeapInsert_null)
	TEST(HeapInsert_ok)
	TEST(HeapForEach_null)
	TEST(HeapForEach_ok)
	TEST(HeapSort_ok)
	TEST(HeapDestroy_ok)
	
END_SUITE

/*****************************************************************************/
	
	

