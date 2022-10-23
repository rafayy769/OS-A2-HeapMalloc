#include <stdlib.h>
#include "../include/my_allocator.h"

/**
 * @brief Caclulates log2 of the given integer. Assumes x >= 0.
 * 
 * @param x Number to calculate log2 of.
 * @return unsigned int Result of log2(x).
 */
unsigned int intLog2(unsigned int x)
{
	if (x == 0)
	{
		return 0;
	}
    short f = x && !(x & (x - 1));
	unsigned int ans = 0;
	while (x >>= 1) ans++;
	return ans + !f;
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
	
	freeListArr     = (Addr)malloc(sizeOfArr * sizeof(Addr));
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
		DEBUG("Memory allocation for main memory block and free list array successful");
		DEBUG("Address of main memory block: %p", mainMemoryBlock);
		for (int i = 0; i < sizeOfArr; i++)
		{
			freeListArr[i] = NULL;
		}
		freeListArr[sizeOfArr-1] = mainMemoryBlock;
		
		sizeOfFreeListArr     = sizeOfArr;
		sizeOfMainMemoryBlock = _length;
		sizeOfBasicBlock      = _basic_block_size;
		
		// Add header to the main block
		Header* header = (Header*)mainMemoryBlock;
		header->next = NULL;

		return _length;
	}
}

void printFreeList()
{
	DEBUG("Free list:");
	for (int i = 0; i < sizeOfFreeListArr; i++)
	{
		DEBUG("Index %d: %p",i, freeListArr[i]);
	}
}
// find address of the buddy of the block whose address is given
Addr findBuddy(Addr _block, int i)
{
	unsigned int sizeOfBlock = _BLOCK_SIZE(i);
	Addr buddy = NULL;
	if (_BLOCK_SIZE(i) == sizeOfMainMemoryBlock)
	{
		return NULL;
	}
	if (((long)_block - (long)mainMemoryBlock) % (2 * sizeOfBlock) == 0)
	{
		buddy = _block + sizeOfBlock;
	}
	else
	{
		buddy = _block - sizeOfBlock;
	}
	DEBUG("Buddy of block %p is %p", _block, buddy);
	return buddy;
}

/**
 * @brief Recursively splits the given block until it reaches the required size and returns the address of the block. Buddies are also split and added to the free list.
 * 
 * @param _block Address of the block to split.
 * @param i Size order of the block to split.
 * @param j Size order of the block to split to.
 * @return Addr Address of the block of size 2^j.
 */
Addr split_block(Addr _block, unsigned int i, unsigned int j)
{
	DEBUG("Splitting block of size %d to size %d", _BLOCK_SIZE(i), _BLOCK_SIZE(j));
	// if the block is of the required size, return the address of the block
	if(_BLOCK_SIZE(i) == _BLOCK_SIZE(j))
	{
		return _block;
	}
	else
	{	
		// find the buddy
		Addr buddy = findBuddy(_block, i-1);
		// add the buddy to the list
		freeListArr[i-1] = buddy;
		// add the header to the buddy, since the free list was empty, therefore the next pointer is NULL
		Header* header = (Header*)buddy;
		header->next = NULL;
		// recursively split the block
		return split_block(_block, i-1, j);
	}
}


/**
 * @brief Allocates a block of memory of size _length. The allocated block is the smallest block that can hold _length bytes. Returns a pointer to the allocated block, or NULL if the request cannot be satisfied.
 * 
 * @param _length Size of the block to be allocated.
 * @return Addr Pointer to the allocated block.
 */
Addr my_malloc(unsigned int _length)
{
	DEBUG("my_malloc called with length %d", _length);
	
	Addr block = NULL;

	// Required size will be greater than _length in order to place the header in the block.
	unsigned int requiredSize = _length + HEADER_SIZE;
	DEBUG("my_malloc needs a block of size %d", requiredSize);

	// Basic block size
	unsigned int basicBlockSize = sizeOfBasicBlock; // basic_block_size = (main_memory_size / 2^(size_of_free_list_array - 1))
	
	// Step 1. Look in the free list, if any free block is available of size >= _length + HEADER_SIZE.
	int i = intLog2((requiredSize + basicBlockSize - 1) / basicBlockSize);
	DEBUG("my_malloc will look for a block of order %d size %d in the free list", i, _BLOCK_SIZE(i));

	// check if the required size is larger than the size of the main memory block
	if (i >= sizeOfFreeListArr)
	{
		DEBUG("Required size is greater than the size of the main memory block");
		return NULL;
	}
	// Check if that required size block is available
	else if(freeListArr[i] != NULL)
	{
		DEBUG("Found a block of size %d in the free list", _BLOCK_SIZE(i));
		block = freeListArr[i];
		Addr head = freeListArr[i];

		// Update the free list array with the next free block
		// freeListArr[i] = *(Addr*)head;
		freeListArr[i] = ((Header*)head)->next;
		printFreeList();
		return block + HEADER_SIZE;	
		// return block;
	}
	// When the required size is not available, find the next biggest block and keep splitting
	else
	{
		DEBUG("Did not find a block of size %d in the free list", _BLOCK_SIZE(i));
		// get the block by splitting, and use its buddy as the head in the free_list_array
		int j = i;
		printFreeList();
		while(freeListArr[j] == NULL)
		{
			j++;
		}
		DEBUG("i = %d, j = %d", i, j);
		// i represents the Block size which will be used to split
		// Split the block and update the free list array
		// size of block for index i = 1 << (i + log2(basicBlockSize))
		// block = split_block(freeListArr[j], (1 << (i + intLog2(basicBlockSize))));
		block = split_block(freeListArr[j], j, i);
		freeListArr[sizeOfFreeListArr-1] = NULL;
		printFreeList();
		return block + HEADER_SIZE;
		// return block;
	}

	// return freeBlock + HEADER_SIZE;
	// return malloc((size_t)_length);
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