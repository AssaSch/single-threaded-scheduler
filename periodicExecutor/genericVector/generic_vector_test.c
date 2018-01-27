#include "generic_vector.h"
#include "mu_test.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct Person
{
	int* m_arr;
	
}Person;

/*****************************************************************************/

int TestForMembers(Vector* _vector, size_t _originalSize, size_t _size, size_t _nItems, size_t _blockSize, size_t _magic);

void StructPersonDestroy(void* _item);

/*****************************************************************************/
/*****************************************************************************/

void StructPersonDestroy(void* _item)
{	
	free(((Person*)_item)->m_arr);
}

/*****************************************************************************/

int VectorPrint(void* _element, size_t _index, void* _context)
{	
	printf("%lu: %c %s \n", _index, *((char*)_element), (char*)_context);
	return 1;	
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

UNIT(create_null)
	
	ASSERT_THAT(!VectorCreate(0, 0));
	
END_UNIT

/*****************************************************************************/

UNIT(create_ok)
	
	Vector* vector;
	
	vector = VectorCreate(2, 3);
	ASSERT_THAT(vector);
	ASSERT_THAT(TestForMembers(vector, 2, 2, 0, 3, 0xABCDABCD));
	VectorDestroy(&vector, NULL);
	
END_UNIT

/*****************************************************************************/

UNIT(Append_null)

	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorAppend(NULL, NULL));
	
END_UNIT

/*****************************************************************************/

UNIT(Append_overflow)
	
	Vector* vector;
	int item;
	
	item = 4;
	vector = VectorCreate(1, 0);
	VectorAppend(vector, (void*)&item);
	ASSERT_THAT(VECTOR_OVERFLOW_ERROR == VectorAppend(vector, (void*)&item));

	VectorDestroy(&vector, NULL);
	
END_UNIT

/*****************************************************************************/

UNIT(Append_alocation_error)
	
	Vector* vector;
	int item;
	
	item = 4;
	vector = VectorCreate(1, 9999999999);
	VectorAppend(vector, (void*)&item);
	ASSERT_THAT(VECTOR_ALLOCATION_ERROR == VectorAppend(vector, (void*)&item));

	VectorDestroy(&vector, NULL);
	
END_UNIT

/*****************************************************************************/

UNIT(Append_ok)
	
	Vector* vector;
	int item;
	void* ptr;
	
	item = 4;
	vector = VectorCreate(1, 1);
	ASSERT_THAT(VECTOR_SUCCESS == VectorAppend(vector, (void*)&item));
	VectorGet(vector, 1, &ptr);
	ASSERT_THAT(*((int*)ptr) == 4);	 
	VectorDestroy(&vector, NULL);
	
END_UNIT

/*****************************************************************************/

UNIT(Remove_null)
	
	void* ptr;
	Vector* vector;
	
	vector = VectorCreate(1, 1);	
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorRemove(NULL, &ptr));
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorRemove(vector, NULL));	
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorRemove(NULL, NULL));
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Remove_underflow)
	
	Vector* vector;
	void* ptr;
		
	vector = VectorCreate(1, 1);	
	ASSERT_THAT(VECTOR_UNDERFLOW_ERROR == VectorRemove(vector, &ptr));
	
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Remove_ok)
	
	Vector* vector;
	void* ptr;
	char item1;
	char item2;
	char item3;
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
			
	vector = VectorCreate(1, 1);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);			
	ASSERT_THAT(VECTOR_SUCCESS == VectorRemove(vector, &ptr));
	ASSERT_THAT(*((char*)ptr) == 'c');	 
	VectorRemove(vector, &ptr);
	ASSERT_THAT(*((char*)ptr) == 'b');
	ASSERT_THAT(TestForMembers(vector, 1, 2, 1, 1, 0xABCDABCD));	 	
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(get_null)
	
	void* ptr;
	Vector* vector;
	char item1;

	item1 = 'a';	
	vector = VectorCreate(1, 1);	
	VectorAppend(vector, (void*)&item1);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorGet(NULL, 1, &ptr));
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorGet(vector, 1, NULL));	
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorGet(NULL, 1, NULL));
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(get_wrong_index)
	
	void* ptr;
	Vector* vector;
	char item1;

	item1 = 'a';	
	vector = VectorCreate(1, 1);	
	VectorAppend(vector, (void*)&item1);
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == VectorGet(vector, 0, &ptr));
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == VectorGet(vector, 2, &ptr));	
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Get_ok)
	
	Vector* vector;
	void* ptr;
	char item1;
	char item2;
	char item3;
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
			
	vector = VectorCreate(1, 1);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);			
	ASSERT_THAT(VECTOR_SUCCESS == VectorGet(vector, 2, &ptr));
	ASSERT_THAT(*((char*)ptr) == 'b');	 
	VectorGet(vector, 3, &ptr);
	ASSERT_THAT(*((char*)ptr) == 'c');
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Set_null)
	
	Vector* vector;
	char item1;

	item1 = 'a';	
	vector = VectorCreate(1, 1);	
	VectorAppend(vector, (void*)&item1);
	ASSERT_THAT(VECTOR_UNITIALIZED_ERROR == VectorSet(NULL, 1, (void*)&item1));
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Set_wrong_index)
	
	Vector* vector;
	char item1;

	item1 = 'a';	
	vector = VectorCreate(1, 1);	
	VectorAppend(vector, (void*)&item1);
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == VectorSet(vector, 0, (void*)&item1));
	ASSERT_THAT(VECTOR_INDEX_OUT_OF_BOUNDS_ERROR == VectorSet(vector, 2, (void*)&item1));	
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Set_ok)
	
	Vector* vector;
	void* ptr;
	char item1;
	char item2;
	char item3;
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);		
	VectorSet(vector, 1, (void*)&item3);
	VectorSet(vector, 2, (void*)&item1);				
	ASSERT_THAT(VECTOR_SUCCESS == VectorSet(vector, 3, (void*)&item2));
	VectorGet(vector, 1, &ptr);
	ASSERT_THAT(*((char*)ptr) == 'c');	 
	VectorGet(vector, 2, &ptr);
	ASSERT_THAT(*((char*)ptr) == 'a');
	VectorGet(vector, 3, &ptr);
	ASSERT_THAT(*((char*)ptr) == 'b');
	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(Size_null)
	
	ASSERT_THAT(0 == VectorSize(NULL));
	
END_UNIT

/*****************************************************************************/

UNIT(Size_ok)
	
	Vector* vector;
	char item1;
	char item2;
	char item3;
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
	
	vector = VectorCreate(1, 1);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);						
	ASSERT_THAT(3 == VectorSize(vector));

	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(capacity_null)
	
	ASSERT_THAT(0 == VectorCapacity(NULL));
	
END_UNIT

/*****************************************************************************/

UNIT(capacity_ok)
	
	Vector* vector;
	char item1;
	char item2;
	char item3;
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
	
	vector = VectorCreate(1, 3);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);						
	ASSERT_THAT(4 == VectorCapacity(vector));

	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(for_each_null)
	
	int cont = 1;
	Vector* vector;
	
	vector = VectorCreate(1, 3);
	ASSERT_THAT(0 == VectorForEach(NULL, VectorPrint, (void*)&cont));
	ASSERT_THAT(0 == VectorForEach(vector, NULL, (void*)&cont));	
	ASSERT_THAT(0 == VectorForEach(NULL, NULL, (void*)&cont));
	VectorDestroy(&vector, NULL);	
		
END_UNIT

/*****************************************************************************/

UNIT(for_each_ok)
	
	Vector* vector;
	char item1;
	char item2;
	char item3;
	char* context = "hallo";
		
	item1 = 'a';
	item2 = 'b';
	item3 = 'c';
	
	vector = VectorCreate(1, 3);
	VectorAppend(vector, (void*)&item1);
	VectorAppend(vector, (void*)&item2);	
	VectorAppend(vector, (void*)&item3);						
	ASSERT_THAT(3 == VectorForEach(vector, VectorPrint, (void*)context));

	VectorDestroy(&vector, NULL);	
	
END_UNIT

/*****************************************************************************/

UNIT(destroy_ok)
	
	Vector* vector;
	Person person1;
	Person person2;
	
	person1.m_arr = (int*)malloc(sizeof(int));
	person2.m_arr = (int*)malloc(sizeof(int));
	
	*(person1.m_arr) = 3;
	*(person2.m_arr) = 11;
		
	vector = VectorCreate(1, 3);
	VectorAppend(vector, (void*)&person1);
	VectorAppend(vector, (void*)&person2);	

	VectorDestroy(&vector, StructPersonDestroy);
	VectorDestroy(&vector, StructPersonDestroy);		
	
END_UNIT

/*****************************************************************************/

TEST_SUITE(framework test)

	TEST(create_null)
	TEST(create_ok)
	TEST(Append_null)	
	TEST(Append_overflow)
	TEST(Append_alocation_error)		
	TEST(Append_ok)	
	TEST(Remove_null)	
	TEST(Remove_underflow)
	TEST(Remove_ok)
	TEST(get_null)		
	TEST(get_wrong_index)
	TEST(Get_ok)
	TEST(Set_null)
	TEST(Set_wrong_index)
	TEST(Set_ok)
	TEST(Size_null)																												
	TEST(Size_ok)
	TEST(capacity_null)																												
	TEST(capacity_ok)
	TEST(for_each_null)																												
	TEST(for_each_ok)
	TEST(destroy_ok)
																															
END_SUITE

/*****************************************************************************/
	
	
