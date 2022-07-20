/*============================LIBRARYS && MACROS =============================*/
#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include <stdlib.h> /*malloc && free*/

#include "../include/stack.h"

#define EMPTY_STACK 0
#define ONE_IN_STACK 1 
/*===================== STRUCT && FUNCTION DEFINITION ======================*/

struct stack
{
	size_t size_of_element;
	size_t max_num_of_elements;
	size_t curr_num_of_elements;
	char *top;
};
/*typedef struct stack *stack_ptr_t;*/

/*
	time  complexity O(1)
	space complexity O(sizeof(struct stack) + (capacity * element_size))
*/
stack_ptr_t StackCreate(size_t capacity, size_t element_size)
{
	stack_ptr_t stack = (stack_ptr_t)malloc(sizeof(struct stack) + (capacity * element_size));
	stack->top = (char *)(sizeof(struct stack) + &stack);

	stack->size_of_element = element_size;
	stack->max_num_of_elements = capacity;
	stack->curr_num_of_elements = 0;

	return stack;
}


/*
	time  complexity O(1)
	space complexity O(1)
*/
void StackDestroy(stack_ptr_t stack)
{
	assert(NULL != stack);
	free(stack);
	stack = NULL;
}

/*
	time  complexity O(n) - memcpy
	space complexity O(1)
*/
void StackPush(stack_ptr_t stack, const void *element_to_push)
{
	assert(NULL != stack);
	assert(NULL != element_to_push);
	

	if(stack->curr_num_of_elements > EMPTY_STACK)
	{
		stack->top += stack->size_of_element;
	}

	stack->curr_num_of_elements += 1;
	stack->top = memcpy(stack->top, element_to_push, stack->size_of_element);
	
}

/*
	time  complexity O(1)
	space complexity O(1)
*/
void *StackPop(stack_ptr_t stack)
{
	assert(NULL != stack);

	if(ONE_IN_STACK < stack->curr_num_of_elements)
	{
		stack->top -= stack->size_of_element;
	}

	stack->curr_num_of_elements -= 1;
	return (void *)((stack->top) + stack->size_of_element);
}

/*
	time  complexity O(1)
	space complexity O(1)
*/
int StackIsEmpty(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return  (EMPTY_STACK == stack->curr_num_of_elements);	
}

/*
	time  complexity O(1)
	space complexity O(1)
*/
size_t StackGetSize(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return (stack->curr_num_of_elements);	
}

/*
	time  complexity O(1)
	space complexity O(1)
*/
size_t StackGetCapacity(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return (stack->max_num_of_elements);
}

/*
	time  complexity O(1)
	space complexity O(1)
*/
void *StackPeek(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return ((void *)stack->top); 
}
