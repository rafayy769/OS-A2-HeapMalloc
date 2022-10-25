#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/ackermann.h"
#include "../include/my_allocator.h"

int main(int argc, char **argv)
{
  // input parameters (basic block size, memory length)
  int option;

  extern char *optarg;
  extern int optind, opterr, optopt;

  int basic_block_size = 16;
  int memory_length = (512*1024);

  while ((option = getopt(argc, argv, "b:s:")) != -1)
  {
	switch (option)
	{
	case 'b':
	  printf("basic block size: %s", optarg);
	  basic_block_size = atoi(optarg);
	  break;
	case 's':
	  printf("memory length: %s", optarg);
	  memory_length = atoi(optarg);
	  break;
	default:
	  printf("--usage\n");
	  printf("memtest [-b <blocksize>] [-s <memsize>]\n");
	  printf("-b <blocksize> defines the block size, in bytes. Default is 128 bytes.\n");
	  printf("-s <memsize> defines the size of the memory to be allocated, in bytes. Default is 512 kB.\n");
	  return -1;
	}
  }

  // init_allocator(basic block size, memory length)
  init_allocator(basic_block_size, memory_length);

  ackermann_main();

  // release_allocator()
  atexit(release_allocator);

  return 0;
}