#include <stdlib.h>
#include "../include/my_allocator.h"

/**
 * @brief Caclulates log2 of the given integer. Assumes x >= 0. Returns ceil(log2(x)).
 * 
 * @param x Number to calculate log2 of.
 * @return unsigned int Result of log2(x).
 */
unsigned int intLog2(unsigned int x)
{
	unsigned int ans = 0;
	while (x >>= 1) ans++;
	return ans;
}

/**
 * @brief Initializes the allocator. Allocates a big block of memory, to be used for allocations by malloc. Initializes the free list array as well. This function must be called before any other allocator function.
 * 
 * @param _basic_block_size Smallest possible block size.
 * @param _length Length of the memory to make available to the program.
 * @return unsigned int Amount of memory made available. Zero if an error is encountered.
 */
unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)
{
	int sizeOfArr = intLog2(_length / _basic_block_size) + 1;
	freeListArr = (Addr)malloc(sizeOfArr * sizeof(Addr));
	
	mainMemoryBlock = (Addr)malloc(_length);
	if (mainMemoryBlock == NULL)
	{
		DEBUG("Memory allocation for main memory block failed");
		free(freeListArr);
		return 0;
	}
	else if(freeListArr == NULL)
	{
		DEBUG("Memory allocation for free list array failed");
		free(mainMemoryBlock);
		return 0;
	}
	else
	{
		for (int i = 0; i < sizeOfArr; i++)
		{
			freeListArr[i] = NULL;
		}
		return _length;
	}
}

// find address of the buddy of the block whose address is given
void* find_buddy(void* _block)
{
	unsigned long block_addr = (unsigned long)_block;
	
}

/**
 * @brief Allocates a block of memory of size _length. The allocated block is the smallest block that can hold _length bytes. Returns a pointer to the allocated block, or NULL if the request cannot be satisfied.
 * 
 * @param _length Size of the block to be allocated.
 * @return Addr Pointer to the allocated block.
 */
Addr my_malloc(unsigned int _length)
{
	// Step 1. Look in the free list, if any free block is available of size >= _length + HEADER_SIZE.


	return malloc((size_t)_length);
}

/**
 * @brief Releases a block of memory previously allocated using my_malloc. Returns the block to the free list.
 * 
 * @param _a Pointer to the block to be freed.
 * @return int 0 if successful, -1 otherwise.
 */
int my_free(Addr _a)
{
	free(_a);
	return 0;
}

/**
 * @brief Releases all memory acquired by the allocator. This function must be called after all other allocator functions.
 * 
 * @return int 1 if successful, 0 if an error is encountered.
 */
int release_allocator()
{
	free(mainMemoryBlock);
	free(freeListArr);
	
	return 1;	
}