#ifndef _my_allocator_h_ // include file only once
#define _my_allocator_h_

#include <stdio.h>

typedef void * Addr; 

#define SHOW_ERROR 0

#define HEADER_SIZE 8

#define MAGIC_NUMBER 0x69694200

// Macro to print debug messages, in debug mode.
#define DEBUG(...)\
if(SHOW_ERROR)\
{\
	printf("\033[0;31m [Debug message]: ");\
	printf("%s, %d ", __FUNCTION__, __LINE__);\
	printf(__VA_ARGS__);\
	printf("\n");\
	printf("\033[0m");\
}

#define DEBUG2(...)\
if(SHOW_ERROR)\
{\
	printf("\033[0;31m");\
	printf(__VA_ARGS__);\
	printf("\033[0m");\
}

// The array of free lists. Stores the head of each free list.
Addr* freeListArr;
// Main memory block acquired from heap for further allocations.
Addr mainMemoryBlock;
// Size of the main memory block.
unsigned int sizeOfMainMemoryBlock;
// Size of the free list array.
unsigned int sizeOfFreeListArr;
// Size of the basic block.
unsigned int sizeOfBasicBlock;

#define _BLOCK_SIZE(i) (sizeOfBasicBlock * (1 << i))

typedef struct _header
{
	Addr next;
} Header;

// typedef struct _block
// {
// 	Header header;
// 	Addr data;
// } Block;

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length); 
/* This function initializes the memory allocator and makes a portion of 
	’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	its minimal unit of allocation. The function returns the amount of 
	memory made available to the allocator. If an error occurred, 
	it returns 0. 
*/ 

int release_allocator(); 
/* This function returns any allocated memory to the operating system. 
	After this function is called, any allocation fails.
*/ 

Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
	address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
	using ’my_malloc’. Returns 0 if everything ok. */ 

#endif