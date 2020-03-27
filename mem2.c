#include <mem.h>
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





/*********** MEM_INIT FUNCTION ************************/

int Mem_init(int sizeOfRegion)
	(
		int fd=open("/dev/zero", O_RDWR);
		head = mmap(NULL, sizeOfRegion, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (head == MAP_FAILED)
		{
			perror("mmap");
			exit(1);
		}
		close(fd);

		head->size = heapSize - sizeof(node_t); // Heap size is set after deducting header size
		head->free=1;				// free value 1=Available; 0= Not Available(allotted)
		head->next = NULL;
		curr_ptr=(char *)(head+1);		// curr_ptr points to the free space after header
	
		start=head;	//Saving address of beginning of heap in 'start'

		return 0;
	}




/*****************  MEM_ALLOC FUNCTION  *******************/


void *Mem_Alloc(int sizeAllotted)
	{

	node_t *temp;
	freeMemory=0;

	//search for free space within the Heap allocation


	for(temp=start;temp!=NULL;temp=temp->next)
	{
	if ((temp->free==1) && (temp->size > (sizeAllotted + sizeof(node_t)) ))//if the node is not allotted and node size is big enough
		{
		curr_ptr=(void *)(temp + 1);
		temp->next=temp+ 1 + (sizeAllotted/sizeof(node_t));	// Splitting the node by creating another header after present allocation
		temp->next->size=temp->size - (sizeAllotted + sizeof(node_t));
		temp->next->free=1;
		temp->next->next=NULL;

		temp->size=sizeAllotted;			// first part of the split node is allotted
		temp->free=0;					// free = 0 means this is not free
		return curr_ptr;
		}
	}
	return NULL;
	}		





/********************* MEM_AVAILABLE FUNCTION   ****************/



int Mem_Available()
	{
	int freeSpace=0;
	node_t *temp;

	for(temp=start;temp!=NULL;temp=temp->next)	// search each node and freespace if free flag = 1
		{
		if (temp->free ==1)
		freeSpace=freeSpace+(temp->size - sizeof(node_t));
		}
	return freeSpace;
	}





/***************************    MEM_FREE FUNCTION     ****************/


int Mem_Free(void *f_ptr)
	{

	node_t *temp, *prev_ptr;
	int found=0;

	if(head +1 ==f_ptr)			// if first node is to be freed 
		{				// and second node is already free (not allotted)
		if(head->next->free == 1)	// merge (coalesce) both nodes
			{
			head->size=head->size + head->next->size + sizeof(node_t);
			head->free =1;
			head->next=head->next->next;
			}
		else				// second node is not free and only first node is to be 
			{			// marked as free (free=1) and size is modified
			head->free=1;
			head->size=head->size + sizeof(node_t);
			}
		found=1;
		return found;
		}

	if (!found)				// node to be freed is from second position onwards
		{
		prev_ptr=head;			// address of first node is saved in 'prev_ptr'
		for(temp=head->next;temp!=NULLO;temp=temp->next)	// search through the node list
			{
			if((temp + 1) == f_ptr)			// when node to be freed is found
				{

				if (prev_ptr->free == 1 && temp->next->free == 1)	// previous and next nodes are already free
					{						// hence, all three nodes to be merged
					prev_ptr->size = prev_ptr->size + temp->size + temp->next->size + sizeof(node_t) + 	sizeof(node_t);
					prev_ptr->free = 1;
					prev_ptr->next=temp->next->next;		// prev_ptr should point to fourth node from prev
					}
				else
				(if prev_ptr->free !=1 && temp->next->free == 1)	// previous node not free and next node is free
					{						// hence, merge present and next nodes
					temp->size = temp->size + temp->next->size + sizeof(node_t);
					temp->next=temp->next->next;
					temp->free = 1;
					}
				else
				if (prev_ptr->free == 1 && temp->next->free != 1)	// previous node free, next node not free
					{
					prev_ptr->size = prev_ptr->size + temp->size + sizeof(node_t);
					prev_ptr->free =1;
					prev_ptr->next=temp->next;
					}

				}
			return found;
			}

		}

	}




/************************* MEM_DUMP FUNCTION  **********************/



void Mem_Dump()
	{
	node _t* temp=start;
	printf("\nMemory Status : core dump ");

	while (temp!=NULL)
		{
		printf("Starting address of the chunk: %p size available: %d\n",(boid *)temp+sizeof(node_t),temp->size);
		temp=temp->next;
		}

	}



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
// memory is remaining.

// Finally we are calling Mem_Dump().

void main()
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

	if (curr_ptr0=(char *)Mem_Alloc(sizeAllotted)!=NULL)
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

	if (curr_ptr0=(char *)Mem_Alloc(sizeAllotted)!=NULL)
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

	if (curr_ptr0=(char *)Mem_Alloc(sizeAllotted)!=NULL)
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


	if ((freed=Mem_Free(curr_ptr1))==1)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());

	if ((freed=Mem_Free(curr_ptr2))==1)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());

	if ((freed=Mem_Free(curr_ptr3))==1)
	printf("\nNow Available Memory is %d bytes\n", Mem_Available());


	Mem(Dump();

	}


