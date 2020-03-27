#include "./mem.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int heapSize;

typedef struct node_t {
int size;
int free;
struct node_t *next;
} node_t;


node_t *head, *start, *tail;
char *curr_ptr, *curr_ptr1, *curr_ptr2, *curr_ptr3, *curr_ptr0;






/************************* MAIN FUNCTION  **********************/
// We are testing the functions written by calling them in the main()
// First we ask user how much initial Heap size is required in bytes and then
// call Mem_Init() and allocate initial Heap in units of pagesize
// Then we ask the user to enter the size of memory needed within
// the initial Heap size.  For every input, we allocate memory in units of 8 bytes 
// by calling Mem_Alloc().  After allocating memory we display the remaining memory
// available by calling Mem_Available().

// In the present main() function first we allocate Initial Heap memory by taking input
// from the user and display how much Heap memory was allocated.
// Then we take memory requirement from the user, allocate it and then display 
// how much is allotted and how much is available memory.  This we do three times
// and each time we are saving the pointer to allocated memory block in pointers
// curr_ptr1, curr_ptr2 and curr_ptr3.

// Then we free the memory allotted by calling Mem_Free() routine three times
// using curr_ptr1, curr_ptr2 and curr_ptr3 and each time we display how much
// memory is remaining.  We can observe that as we are allotting memory three times, 
// our available memory is coming down and when we release the memory three times,
// the available memory increases to the original size.

// Finally we are calling Mem_Dump() which displays the data from memory chunks.

int main()
	{

	int pageSize;
	int sizeAllotted;
	int freed=0;


	pageSize=getpagesize();


				/****** Initial Heap Allocation  ********/
	do
		{
		printf("\nEnter sufficient size for initial memory requirement in bytes\n");
		scanf("%d",&heapSize);
		}
		while (heapSize < pageSize);
	heapSize=(heapSize/pageSize)*pageSize;		// Rounding the initial allotment to units of pagesize

	if (Mem_Init(heapSize))
		printf("Initial Heap allocation failed\n");
	else
		printf("You are allotted %d bytes rounded to pagesize for your program\n", heapSize);
	printf("\nAvailable memory is %d bytes", Mem_Available());


				/******* First Time allotting memory ********/

	printf("\nEnter present memory requirement : ");
	scanf("%d",&sizeAllotted);

	if (sizeAllotted < 8)			// Rounding the user requirement to units of 8 bytes
		sizeAllotted=8;
	else
		sizeAllotted=8*(sizeAllotted/8);

	curr_ptr0=(char *)Mem_Alloc(sizeAllotted);
	if (curr_ptr0!=NULL)
		{
		printf("\nPresent allocation of %d bytes success\n", sizeAllotted);
		}
	else
		{
		printf("\nSorry, memory not available\n");

		}
	printf("\nAvailable memory is %d bytes", Mem_Available());
	printf("\nAddress of memory allotted %p ", curr_ptr);

	curr_ptr1=curr_ptr;


				/******* Second Time allotting memory ********/


	printf("\nEnter present memory requirement : ");
	scanf("%d",&sizeAllotted);

	if (sizeAllotted < 8)
		sizeAllotted=8;
	else
		sizeAllotted=8*(sizeAllotted/8);

	curr_ptr0=(char *)Mem_Alloc(sizeAllotted);
	if (curr_ptr0!=NULL)
		{
		printf("\nPresent allocation of %d bytes success\n", sizeAllotted);
		}
	else
		{
		printf("\nSorry, memory not available\n");
		}
	printf("\nAvailable memory is %d bytes", Mem_Available());
	printf("\nAddress of memory allotted %p ", curr_ptr);
	
	curr_ptr2=curr_ptr;



				/******* Third Time allotting memory ********/


	printf("\nEnter present memory requirement : ");
	scanf("%d",&sizeAllotted);

	if (sizeAllotted < 8)
		sizeAllotted=8;
	else
		sizeAllotted=8*(sizeAllotted/8);

	curr_ptr0=(char *)Mem_Alloc(sizeAllotted);
	if (curr_ptr0!=NULL)
		{
		printf("\nPresent allocation of %d bytes success\n", sizeAllotted);
		}
	else
		{
		printf("\nSorry, memory not available\n");
		}
	printf("\nAvailable memory is %d bytes", Mem_Available());
	printf("\nAddress of memory allotted %p ", curr_ptr);

	curr_ptr3=curr_ptr;


				/******* Freeing memory three times ********/


	if ((freed=Mem_Free(curr_ptr1))==0)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());
	else
	printf(" Address not Available ");

	if ((freed=Mem_Free(curr_ptr2))==0)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());
	else
	printf(" Address not Available ");

	if ((freed=Mem_Free(curr_ptr3))==0)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());
	else
	printf(" Address not Available ");


	Mem_Dump();

	return 0;
	}


