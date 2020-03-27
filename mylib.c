#include "mem.h"
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

int Mem_Init(int sizeOfRegion)
	{
		int fd=open("/dev/zero", O_RDWR);
		head = mmap(NULL, sizeOfRegion, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (head == MAP_FAILED)
		{
			perror("mmap");
			return 1;
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
	//int freeMemory=0;

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

	if (f_ptr == NULL)
	return -1;

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
		return 0;
		}

	if (!found)				// node to be freed is from second position onwards
		{
		prev_ptr=head;			// address of first node is saved in 'prev_ptr'
		for(temp=head->next;temp!=NULL;temp=temp->next)	// search through the node list
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
				if (prev_ptr->free !=1 && temp->next->free == 1)	// previous node not free and next node is free
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
			found=1;
			return 0;
			}
		
		}
	return -1;	
	}




/************************* MEM_DUMP FUNCTION  **********************/



void Mem_Dump()
	{
	node_t* temp=start;
	printf("\nMemory Status : core dump ");

	while (temp!=NULL)
		{
		printf("Starting address of the chunk: %p size available: %d\n",(void *)temp+sizeof(node_t),temp->size);
		temp=temp->next;
		}

	}





