# MemoryAllocator

	This application demonstrates how a computer system allocates and deallocates memory.

	In this application we have function to allocate, deallocate and display the available
	memory.  The functions are tested by calling them in the main().

	First we ask user how much initial Heap size is required in bytes and then
	call Mem_Init() and allocate initial Heap in units of pagesize
	Then we ask the user to enter the size of memory needed within
	the initial Heap size.  For every input, we allocate memory in units of 8 bytes 
	by calling Mem_Alloc().  After allocating memory we display the remaining memory
	available by calling Mem_Available().

	In the present main() function first we allocate Initial Heap memory by taking input
	from the user and display how much Heap memory was allocated.
	Then we take memory requirement from the user, allocate it and then display 
	how much is allotted and how much is available memory.  This we do three times
	and each time we are saving the pointer to allocated memory block in pointers
	curr_ptr1, curr_ptr2 and curr_ptr3.

	Then we free the memory allotted by calling Mem_Free() routine three times
	using curr_ptr1, curr_ptr2 and curr_ptr3 and each time we display how much
	memory is remaining.  We can observe that as we are allotting memory three times, 
	our available memory is coming down and when we release the memory three times,
	the available memory increases to the original size.

	Finally we are calling Mem_Dump() which displays the data from memory chunks.

# Technologies

- C
- Unix/Linux


# Live Demo

Live demo not available for this application

# Contribute
Feel free to contribute if you want to make it better.

# Author
* [Rajasekhar K ](https://github.com/IBTechRaj)

Github: https://github.com/IBTechRaj
* Email : krs30018@gmail.com 
* raj_shk@rediffmail.com

# Follow Me

* [LinkedIn](https://www.linkedin.com/in/rajkatakamsetty/)
* [Twitter](https://twitter.com/IBTechRaj)